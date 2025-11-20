/**
 * @file: display.c
 * @desc: Display system implementation - ST7735S LCD graphics rendering
 * @auth: James Farrelly (C24402114)
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f031x6.h>

#include <nucleo_f031k6/common.h>

#include <util.h>
#include <debug.h>
#include <font5x7.h>
#include <st7735s.h>
#include <display.h>

//================================ DOUBLE BUFFERING

/* 
 * Tile-based double buffering for memory-constrained systems
 * 
 * Strategy: Instead of buffering the entire screen (40KB), we use a tile buffer
 * that holds small regions. The screen is divided into tiles, and each tile
 * is rendered to the buffer before being sent to the display.
 * 
 * Memory usage: 1 tile buffer = 32x32 pixels x 2 bytes = 2KB
 * This fits comfortably within the 4KB SRAM budget.
 */

#define TILE_WIDTH  32
#define TILE_HEIGHT 32
#define TILE_SIZE   (TILE_WIDTH * TILE_HEIGHT)

/* Tile buffer for double-buffered rendering (~2KB) */
static uint16_t s_tile_buffer[TILE_SIZE];
static bool s_frame_active = false;
static bool s_tile_dirty = false;

/* Current tile being rendered */
static uint8_t s_current_tile_x = 0;
static uint8_t s_current_tile_y = 0;

/**
 * Flush current tile buffer to display
 */
static void flush_tile_buffer(void)
{
    if (s_tile_dirty)
    {
        uint8_t x_start = s_current_tile_x * TILE_WIDTH;
        uint8_t y_start = s_current_tile_y * TILE_HEIGHT;
        uint8_t x_end = x_start + TILE_WIDTH - 1;
        uint8_t y_end = y_start + TILE_HEIGHT - 1;
        
        /* Clamp to screen bounds */
        if (x_end >= SCREEN_W) x_end = SCREEN_W - 1;
        if (y_end >= SCREEN_H) y_end = SCREEN_H - 1;
        
        open_aperture(x_start, y_start, x_end, y_end);
        st7735s_ramwr();
        
        st7735s_bufw_beg();
        uint16_t tile_pixels = (x_end - x_start + 1) * (y_end - y_start + 1);
        for (uint16_t i = 0; i < tile_pixels; i++)
        {
            st7735s_bufw16(s_tile_buffer[i], 1);
        }
        st7735s_bufw_end();
        
        s_tile_dirty = false;
    }
}

/**
 * Begin a frame for double-buffered rendering
 */
void display_frame_begin(void)
{
    s_frame_active = true;
    s_current_tile_x = 0;
    s_current_tile_y = 0;
    s_tile_dirty = false;
}

/**
 * End a frame and present all buffered changes
 */
void display_frame_end(void)
{
    /* Flush any remaining buffered content */
    flush_tile_buffer();
    s_frame_active = false;
    
    /* Small delay to reduce tearing - allow display refresh to complete */
    nucleo_f031k6_delay(1);
}

//================================ DRAWING

void open_aperture(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    DBG_TRACE("Opening aperture x1 = %d, y1 = %d, x2 = %d, y2 = %d", x1, y1, x2, y2);

    st7735s_caset(x1, x2);
    st7735s_raset(y1, y2);
}

void fill_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t colour)
{
    uint32_t px = height * width;
    
    /* 
     * For tile-sized or smaller rectangles during active frame,
     * use buffering to reduce tearing
     */
    if (s_frame_active && width <= TILE_WIDTH && height <= TILE_HEIGHT && px <= TILE_SIZE)
    {
        /* Fill tile buffer */
        for (uint16_t i = 0; i < px; i++)
        {
            s_tile_buffer[i] = colour;
        }
        
        /* Set tile position and mark as dirty */
        s_current_tile_x = x / TILE_WIDTH;
        s_current_tile_y = y / TILE_HEIGHT;
        s_tile_dirty = true;
        
        /* Flush immediately for now (can be optimized later) */
        flush_tile_buffer();
        return;
    }
    
    /* Default: direct rendering for large operations or non-buffered mode */
    open_aperture(x, y, x + width - 1, y + height - 1);

    st7735s_ramwr();

    st7735s_bufw_beg();
    st7735s_bufw16(colour, px);
    st7735s_bufw_end();
}

void put_pixel(uint8_t x, uint8_t y, uint16_t colour)
{
    open_aperture(x, y, x + 1, y + 1);
    
    st7735s_ramwr();

    st7735s_bufw_beg();
    st7735s_bufw16(colour, 1);
    st7735s_bufw_end();
}

void put_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint16_t *imageBuf, uint8_t orientation)
{
    uint8_t orig_x = x;
    uint8_t orig_y = y;
    uint32_t px = width * height;
    
    /*
     * For sprites that fit in tile buffer during active frame,
     * use buffered rendering for smoother presentation
     */
    if (s_frame_active && px <= TILE_SIZE)
    {
        /* Copy sprite to tile buffer with proper orientation */
        uint16_t buf_idx = 0;
        for (y = 0; y < height; y++)
        {
            uint32_t offsetY = 0;
            if ((orientation & ORIENTATION_VERTICAL) == 0)
                offsetY = y * width;
            else
                offsetY = (height - (y + 1)) * width;

            for (x = 0; x < width; x++)
            {
                uint32_t offsetX = 0;
                if ((orientation & ORIENTATION_HORIZONTAL) == 0)
                    offsetX = x;
                else
                    offsetX = width - x - 1;

                s_tile_buffer[buf_idx++] = imageBuf[offsetY + offsetX];
            }
        }
        
        /* Set tile position and flush */
        s_current_tile_x = orig_x / TILE_WIDTH;
        s_current_tile_y = orig_y / TILE_HEIGHT;
        s_tile_dirty = true;
        
        /* Direct flush with correct dimensions */
        open_aperture(orig_x, orig_y, orig_x + width - 1, orig_y + height - 1);
        st7735s_ramwr();
        st7735s_bufw_beg();
        for (uint16_t i = 0; i < px; i++)
        {
            st7735s_bufw16(s_tile_buffer[i], 1);
        }
        st7735s_bufw_end();
        
        s_tile_dirty = false;
        return;
    }
    
    /* Default: direct rendering for large sprites or non-buffered mode */
    open_aperture(orig_x, orig_y, orig_x + width - 1, orig_y + height - 1);
    st7735s_ramwr();
    st7735s_bufw_beg();

    for (y = 0; y < height; y++)
    {
        uint32_t offsetY = 0;
        if ((orientation & ORIENTATION_VERTICAL) == 0)
            offsetY = y * width;
        else
            offsetY = (height - (y + 1)) * width;

        for (x = 0; x < width; x++)
        {
            uint32_t offsetX = 0;
            if ((orientation & ORIENTATION_HORIZONTAL) == 0)
                offsetX = x;
            else
                offsetX = width - x - 1;

            uint16_t colour = imageBuf[offsetY + offsetX];
            st7735s_bufw16(colour, 1);
        }
    }

   st7735s_bufw_end(); 
}

void print_text(const char *text, uint8_t len, uint8_t scale, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
    if (scale <= 0) scale = 1;

    if (fgColour == bgColour)
    {
        fill_rect(x, y, FONT_WIDTH * scale * len, FONT_HEIGHT * scale, fgColour);

        return;
    }

    if (len == 0) len = strlen_custom(text);

    uint16_t textBox[FONT_WIDTH * scale * FONT_HEIGHT * scale];

    for (uint8_t i = 0; i < len; i++)
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

        put_image(x, y, FONT_WIDTH * scale, FONT_HEIGHT * scale, textBox, 0);
        x += FONT_WIDTH * scale + 2;
    }
}

void print_number(uint16_t number, uint8_t scale, uint8_t x, uint8_t y, uint16_t fgColour, uint16_t bgColour)
{
    char buf[6];
    itoa(number, buf, 10);

    print_text(buf, 0, scale, x, y, fgColour, bgColour);
}

static void draw_line_low_slope(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t colour)
{
    // Reference : https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm    
    uint8_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    int16_t yi = 1;
  
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
  
    int16_t d = 2*dy - dx;
  
    uint8_t y = y0;
    for (uint8_t x=x0; x <= x1; x++)
    {
        put_pixel(x, y, colour);

        if (d > 0)
        {
            y += yi;
            d -= 2 * dx;
        }

        d += 2 * dy;
    }
}

static void draw_line_high_slope(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t colour)
{
    // Reference : https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm    
    int16_t dx = x1 - x0;
    uint8_t dy = y1 - y0;
    int16_t xi = 1;
  
    if (dy < 0)
    {
        xi = -1;
        dx = -dx;
    }
  
    int16_t d = 2 * dx - dy;
  
    uint8_t x = x0;
    for (uint8_t y = y0; y <= y1; y++)
    {
        put_pixel(x, y, colour);

        if (d > 0)
        {
            x += xi;
            d -= 2 * dy;
        }

        d += 2 * dx;
    }
}

void draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t colour)
{
	// Reference : https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    if ( abs(y1 - y0) < abs(x1 - x0) )
    {
        if (x0 > x1)
            draw_line_low_slope(x1, y1, x0, y0, colour);
        else
            draw_line_low_slope(x0, y0, x1, y1, colour);
    }
    else
    {
        if (y0 > y1) 
            draw_line_high_slope(x1, y1, x0, y0, colour);
        else
            draw_line_high_slope(x0, y0, x1, y1, colour);
    }
}

void draw_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t colour)
{
	draw_line(x, y, x + w, y, colour);
    draw_line(x, y, x, y + h, colour);
    draw_line(x + w, y, x + w, y + h, colour);
    draw_line(x, y + h, x + w, y + h, colour);
}

void fill_circle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t colour)
{
    // Reference : https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
	// Similar to drawCircle but fills the circle with lines instead

    uint8_t x = radius-1;
    uint8_t y = 0;
    uint8_t dx = 1;
    uint8_t dy = 1;
    int16_t err = dx - (radius << 1);

    if (radius > x0)
        return; // don't draw even parially off-screen circles
    if (radius > y0)
        return; // don't draw even parially off-screen circles
        
    if ((x0+radius) > SCREEN_W)
        return; // don't draw even parially off-screen circles
    if ((y0+radius) > SCREEN_H)
        return; // don't draw even parially off-screen circles    
        
    while (x >= y)
    {
        draw_line(x0 - x, y0 + y,x0 + x, y0 + y, colour);        
        draw_line(x0 - y, y0 + x,x0 + y, y0 + x, colour);        
        draw_line(x0 - x, y0 - y,x0 + x, y0 - y, colour);        
        draw_line(x0 - y, y0 - x,x0 + y, y0 - x, colour);        

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void init_display()
{
    DBG_INFO("Initializing display...");

    init_st7735s(); 

    DBG_INFO("Filling screen with black...");

    fill_rect(0, 0, SCREEN_W, SCREEN_H, COLOUR_BLACK);
}

