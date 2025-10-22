#include <stm32f031x6.h>

#include <nucleo_f031k6/common.h>

#include <debug.h>
#include <font5x7.h>
#include <st7735s.h>
#include <display.h>

uint16_t rgb_to_word(uint16_t r, uint16_t g, uint16_t b)
{
	uint16_t rvalue = 0;
    rvalue += g >> 5;
    rvalue += (g & (7)) << 13;
    rvalue += (r >> 3) << 8;
    rvalue += (b >> 3) << 3;
    
    return rvalue;
}

//================================ DRAWING

void open_aperture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    DBG_TRACE("Opening aperture x1 = %d, y1 = %d, x2 = %d, y2 = %d", x1, y1, x2, y2);

    st7735s_caset(x1, x2);
    st7735s_raset(y1, y2);
}

void fill_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t colour)
{
    uint32_t px = height * width;
    open_aperture(x, y, x + width - 1, y + height - 1);

    st7735s_ramwr();
    st7735s_bufw16(colour, px);
    st7735s_bufw_end();
}

void put_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
    open_aperture(x, y, x + 1, y + 1);
    
    st7735s_ramwr();
    st7735s_bufw16(colour, 1);
    st7735s_bufw_end();
}

void put_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *imageBuf, int hOrientation, int vOrientation)
{
    open_aperture(x, y, x + width - 1, y + height - 1);
    st7735s_ramwr();

    for (y = 0; y < height; y++)
    {
        uint32_t offsetY = 0;
        if (vOrientation == 0)
            offsetY = y * width;
        else
            offsetY = (height - (y + 1)) * width;

        for (x = 0; x < width; x++)
        {
            uint32_t offsetX = 0;
            if (hOrientation == 0)
                offsetX = x;
            else
                offsetX = width - x - 1;

            uint16_t colour = imageBuf[offsetY + offsetX];
            st7735s_bufw16(colour, 1);
        }
    }

   st7735s_bufw_end(); 
}

void print_text(const char *text, const uint32_t len, uint8_t scale, uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour)
{
    if (scale <= 0) scale = 1;

    if (fgColour == bgColour)
    {
        fill_rect(x, y, FONT_WIDTH * scale * len, FONT_HEIGHT * scale, fgColour);

        return;
    }

    uint16_t textBox[FONT_WIDTH * scale + FONT_HEIGHT * scale];

    for (int i = 0; i < len; i++)
    {
        DBG_TRACE("Writing character %c", text[i]);

        const uint8_t* charCode = &font5x7[FONT_WIDTH * (text[i] - 32)];
        
        uint8_t col = 0;
        while (col < FONT_WIDTH)
        {
            uint8_t row = 0;
            while (row < FONT_HEIGHT)
            {
                if (charCode[col] & (1 << row))
                {
                    textBox[((row*scale) * FONT_WIDTH * scale) + (col*scale)] = fgColour;

                    if (scale > 1) 
                    {
                        textBox[((row*scale) * FONT_WIDTH * scale) + (col*scale) + 1] = fgColour;
                        textBox[(((row*scale) + 1) * FONT_WIDTH * scale) + (col*scale)] = fgColour;
                        textBox[(((row*scale) + 1) * FONT_WIDTH * scale) + (col*scale) + 1] = fgColour;
                    }
                }
                else
                {
                    textBox[((row*scale) * FONT_WIDTH * scale) + (col*scale)] = bgColour;

                    if (scale > 1) 
                    {
                        textBox[((row*scale) * FONT_WIDTH * scale) + (col*scale) + 1] = bgColour;
                        textBox[(((row*scale) + 1) * FONT_WIDTH * scale) + (col*scale)] = bgColour;
                        textBox[(((row*scale) + 1) * FONT_WIDTH * scale) + (col*scale) + 1] = bgColour;
                    }
                }

                row++;
            }

            col++;
        }

        put_image(x, y, FONT_WIDTH * scale, FONT_HEIGHT * scale, textBox, 0, 0);
        x += FONT_WIDTH * scale + 2;
    }
}

void print_centered_text(const char *text, const uint32_t len, uint8_t scale, int16_t offsetX, int16_t offsetY, uint16_t fgColour, uint16_t bgColour)
{
    const size_t textSizePixels = len * FONT_WIDTH * scale;

    uint16_t x = (SCREEN_W - textSizePixels) / 2;
    const uint16_t y = (SCREEN_H - FONT_HEIGHT*scale) / 2;

    print_text(text, len, scale, x + offsetX, y + offsetY, fgColour, bgColour);
}

void init_display()
{
    DBG_INFO("Initializing display...");

    init_st7735s(); 

    DBG_INFO("Filling screen with black...");

    fill_rect(0, 0, SCREEN_W, SCREEN_H, 0x00);
}

