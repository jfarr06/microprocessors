/**
 * @file: st7735s.c
 * @desc: ST7735S LCD controller driver implementation
 * @auth: James Farrelly (C24402114)
 */

#include <debug.h>
#include <st7735s.h>
#include <nucleo_f031k6/common.h>


#define RST GPIO_ODR_3 /* Reset */
#define CS  GPIO_ODR_4 /* Chip Select */
#define DC  GPIO_ODR_6 /* Data/Command */

#define RST_PINOUT GPIO_MODER_MODER3_Pos /* Reset */
#define CS_PINOUT  GPIO_MODER_MODER4_Pos /* Chip Select */
#define DC_PINOUT  GPIO_MODER_MODER6_Pos /* Data/Command */

#define SPI1_SCK_PINOUT  GPIO_MODER_MODER5_Pos
#define SPI1_MOSI_PINOUT GPIO_MODER_MODER7_Pos

uint8_t st7735s_transfer_spi8(uint8_t data)
{
    uint32_t timeout = 1000000;
    uint8_t result = 0;

    volatile uint8_t* reg8 = (volatile uint8_t*)&SPI1->DR;

    while ((SPI1->SR & SPI_SR_BSY) != 0 && timeout--); // Wait for SPI not busy.

    *reg8 = data; // write data.
    timeout = 1000000; // reset timeout

    while ((SPI1->SR & SPI_SR_BSY) != 0 && timeout--) // While SPI busy
        result = *reg8;

    return result;
}

uint16_t st7735s_transfer_spi16(uint16_t data)
{
    uint32_t timeout = 1000000;
    uint8_t result = 0;

    while ((SPI1->SR & SPI_SR_BSY) != 0 && timeout--); // Wait for SPI not busy.

    SPI1->DR = data; // write data.
    timeout = 1000000; // reset timeout

    while ((SPI1->SR & SPI_SR_BSY) != 0 && timeout--) // While SPI busy
        result = SPI1->DR;

    return result;
}

void init_spi()
{
    DBG_INFO("Initializing SPI with PA5 = SCK and PA7 = MOSI as ports...");

    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI

    // Pins 5 & 7 for SPI1.
    set_nucleo_f031k6_pin_mode(GPIOA, SPI1_SCK_PINOUT, PINMODE_ALTERNATE);
    set_nucleo_f031k6_pin_mode(GPIOA, SPI1_MOSI_PINOUT, PINMODE_ALTERNATE);
    GPIOA->AFR[0] &= 0x0f0fffff; // AF0 on AFSEL7 and AFSEL5

    uint32_t _dummy_sr = SPI1->SR; // Dummy read.

    SPI1->CR1 |= SPI_CR1_SSM |
                 SPI_CR1_SSI |
                 SPI_CR1_SPE |
                 SPI_CR1_MSTR|
                 SPI_CR1_CPOL|
                 SPI_CR1_CPHA;
    SPI1->CR2 |= SPI_CR2_8BIT;

    for (uint32_t i = 0; i < sizeof(uint32_t); i++)
        _dummy_sr = st7735s_transfer_spi8(ST7735S_CMD_NOP);

    F031K6_UNUSED(_dummy_sr);
}

//====== RST

// Values (used for comp)
volatile bool _rst_val = false;
volatile bool _cs_val = false;
volatile bool _dc_val = false;

void rst_lo()
{
    if (!_rst_val) return;
    _rst_val = false;

    DBG_TRACE("Set RST low");

    toggle_nucleo_f031k6_odr_bit(GPIOA, RST, false);
}

void rst_hi()
{
    if (_rst_val) return;
    _rst_val = true;

    DBG_TRACE("Set RST high");    

    toggle_nucleo_f031k6_odr_bit(GPIOA, RST, true);
}

//====== CS

void cs_lo()
{
    if (!_cs_val) return;
    _cs_val = false; 
    
    DBG_TRACE("Set CS low");    
 
    toggle_nucleo_f031k6_odr_bit(GPIOA, CS, false);
}

void cs_hi()
{
    if (_cs_val) return;
    _cs_val = true; 

    DBG_TRACE("Set CS high");    
 
    toggle_nucleo_f031k6_odr_bit(GPIOA, CS, true);
}

//======= DC

void dc_lo()
{
    if (!_dc_val) return;
    _dc_val = false;
 
    DBG_TRACE("Set D/C low");    
 
    toggle_nucleo_f031k6_odr_bit(GPIOA, DC, false);
}

void dc_hi()
{
    if (_dc_val) return;
    _dc_val = true; 

    DBG_TRACE("Set D/C high");    

    toggle_nucleo_f031k6_odr_bit(GPIOA, DC, true);
}

void st7735s_bufw8(uint8_t data, size_t n)
{
    cs_lo();
    dc_hi(); // Set data mode.

    while (n--) st7735s_transfer_spi8(data);
}

void st7735s_bufw16(uint16_t data, size_t n)
{
    cs_lo();
    dc_hi(); // Set data mode.

    while (n--) st7735s_transfer_spi16(data);
}

void st7735s_bufw_end()
{
    cs_hi();
}

void st7735s_cmd(uint8_t cmd, uint8_t *data, size_t dataLen)
{
    DBG_TRACE("Sending ST7735S command '%x', data = %p, dataLen = %d", cmd, data, dataLen);

    cs_lo(); // Set chip select low
    dc_lo(); // Set command mode.

    st7735s_transfer_spi8(cmd); // Write command byte

    if (data)
    {
        dc_hi(); // Set data mode.

        for (int i = 0; i < dataLen; i++)
            st7735s_transfer_spi8(data[i]);
    }

    cs_hi(); // Set chip select high
}

void st7735s_rstcycl()
{
    DBG_INFO("Doing ST7735S reset cycle...");

    cs_hi();
    rst_lo();
    nucleo_f031k6_delay(100); // Delay 100ms
    rst_hi();
    nucleo_f031k6_delay(100);
}

void st7735s_swreset()
{
    st7735s_cmd(ST7735S_CMD_SWRESET, NULL, 0);

    nucleo_f031k6_delay(120); // Wait for 120ms -- according to spec.
}

void st7735s_slpout()
{
    st7735s_cmd(ST7735S_CMD_SLPOUT, NULL, 0);

    nucleo_f031k6_delay(120); // Wait for 120ms -- according to spec
}

void st7735s_frmctr1(uint8_t rtna, uint8_t fpa, uint8_t bpa)
{
    uint8_t data[] = 
    {
        rtna,
        fpa,
        bpa
    };

    st7735s_cmd(ST7735S_CMD_FRMCTR1, data, sizeof(data));
}

void st7735s_frmctr2(uint8_t rtnb, uint8_t fpb, uint8_t bpb)
{
    uint8_t data[] = 
    {
        rtnb,
        fpb,
        bpb
    };

    st7735s_cmd(ST7735S_CMD_FRMCTR2, data, sizeof(data));
}

void st7735s_frmctr3(uint8_t rtnc, uint8_t fpc, uint8_t bpc, uint8_t rtnd, uint8_t fpd, uint8_t bpd)
{
    uint8_t data[] = 
    {
        rtnc,
        fpc,
        bpc,
        rtnd,
        fpd,
        bpd
    };

    st7735s_cmd(ST7735S_CMD_FRMCTR3, data, sizeof(data));
}

void st7735s_invctr(uint8_t nl)
{
    uint8_t data[] = { nl };

    st7735s_cmd(ST7735S_CMD_INVCTR, data, sizeof(data));
}

void st7735s_madctl(uint8_t m)
{
    uint8_t data[] = { m };

    st7735s_cmd(ST7735S_CMD_MADCTL, data, sizeof(data));
}

void st7735s_colmod(uint8_t ifpf)
{
    uint8_t data[] = { ifpf };

    st7735s_cmd(ST7735S_CMD_COLMOD, data, sizeof(data));
}

void st7735s_dispon()
{
    st7735s_cmd(ST7735S_CMD_DISPON, NULL, 0);

    nucleo_f031k6_delay(120); // Wait for 120ms -- according to spec.
}

void st7735s_ramwr()
{
    st7735s_cmd(ST7735S_CMD_RAMWR, NULL, 0);
}

void st7735s_caset(uint16_t x1, uint16_t x2)
{
    uint8_t data[] = 
    {
        x1 >> 0x8,
        x1 & 0xff,

        x2 >> 0x8,
        x2 & 0xff
    };

    st7735s_cmd(ST7735S_CMD_CASET, data, sizeof(data));
}

void st7735s_raset(uint16_t y1, uint16_t y2)
{
    uint8_t data[] = 
    {
        y1 >> 0x8,
        y1 & 0xff,

        y2 >> 0x8,
        y2 & 0xff
    };

    st7735s_cmd(ST7735S_CMD_RASET, data, sizeof(data));
}

static const uint8_t default_rtn = 0b00000101;
static const uint8_t default_fp = 0x3c;
static const uint8_t default_bp = 0x3c;

void init_st7735s()
{
    DBG_INFO("Initializing ST7735S, PA3 = RST, PA4 = CS, PA6 = D/C...");

    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    set_nucleo_f031k6_pin_mode(GPIOA, RST_PINOUT, PINMODE_OUTPUT); // PA3 = Reset
    set_nucleo_f031k6_pin_mode(GPIOA, CS_PINOUT, PINMODE_OUTPUT);  // PA4 = Chip Select
    set_nucleo_f031k6_pin_mode(GPIOA, DC_PINOUT, PINMODE_OUTPUT);  // PA6 = Data/Command

    init_spi();

    //=========== BOOT ===========

    DBG_INFO("Doing ST7735S bootup sequence...");

    st7735s_rstcycl(); // Initial reset
    st7735s_swreset(); // SW reset
    st7735s_slpout();  // Exit sleep

    //======= PARAMS =========

    DBG_INFO("Setting default ST7735S parameters...");

    st7735s_frmctr1(default_rtn, default_fp, default_bp); // Normal mode
    st7735s_frmctr2(default_rtn, default_fp, default_bp); // Idle mode
    st7735s_frmctr3(default_rtn, default_fp, default_bp, default_rtn, default_fp, default_bp); // Partial + full color
    
    st7735s_invctr(ST7735S_COLINV_NM | ST7735S_COLINV_IM); // Column inversion on all modes
    st7735s_madctl(ST7735S_MADCTL_BGR); // BGR color mode
    st7735s_colmod(ST7735S_COLMOD_16BIT); // 16 bit colour mode

    DBG_INFO("Turning ST7735S display on...");

    st7735s_dispon(); // Turn display on
}
