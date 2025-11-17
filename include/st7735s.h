#ifndef ST7735S_H_
#define ST7735S_H_

#include <stddef.h>
#include <stdint.h>

// Implemented via: https://files.waveshare.com/upload/e/e2/ST7735S_V1.1_20111121.pdf

/****** DEFS ******/

// System Commands

#define ST7735S_CMD_NOP        (0x00) /* No Operation */
#define ST7735S_CMD_SWRESET    (0x01) /* Software Reset */
#define ST7735S_CMD_RDDID      (0x04) /* Read Display ID */
#define ST7735S_CMD_RDDST      (0x09) /* Read Display Status */
#define ST7735S_CMD_RDDPM      (0x0A) /* Read Display Power Mode */
#define ST7735S_CMD_RDD_MADCTL (0x0B) /* Read display MADCTL */
#define ST7735S_CMD_RDD_COLMOD (0x0C) /* Read display pixel format */
#define ST7735S_CMD_RDDIM      (0x0D) /* Read display image mode */
#define ST7735S_CMD_RDDSM      (0x0E) /* Read display signal mode */
#define ST7735S_CMD_RDDSDR     (0x0F) /* Read display self-diagnostic result */
#define ST7735S_CMD_SLPIN      (0x10) /* Sleep in & Booster off */
#define ST7735S_CMD_SLPOUT     (0x11) /* Sleep out & Booster on */
#define ST7735S_CMD_PTLON      (0x12) /* Partial mode on */
#define ST7735S_CMD_NORON      (0x13) /* Partial mode off */
#define ST7735S_CMD_INVOFF     (0x20) /* Display inversion off */
#define ST7735S_CMD_INVON      (0x21) /* Display inversion on */
#define ST7735S_CMD_GAMSET     (0x26) /* Gamme curve select */
#define ST7735S_CMD_DISPOFF    (0x28) /* Display off */
#define ST7735S_CMD_DISPON     (0x29) /* Display on */
#define ST7735S_CMD_CASET      (0x2A) /* Column address set */
#define ST7735S_CMD_RASET      (0x2B) /* Row address set */
#define ST7735S_CMD_RAMWR      (0x2C) /* Memory write */
#define ST7735S_CMD_RGBSET     (0x2D) /* LUT for 4k, 65k, 262k color display */
#define ST7735S_CMD_RAMRD      (0x2E) /* Memory read */
#define ST7735S_CMD_PTLAR      (0x30) /* Partial start/end address set */
#define ST7735S_CMD_SCRLAR     (0x33) /* Scroll area set */
#define ST7735S_CMD_TEOFF      (0x34) /* Tearing effect line off */
#define ST7735S_CMD_TEON       (0x35) /* Tearing effect mode set & on */
#define ST7735S_CMD_MADCTL     (0x36) /* Memory data access control */
#define ST7735S_CMD_VSCSAD     (0x37) /* Scroll start address of RAM */
#define ST7735S_CMD_IDMOFF     (0x38) /* Idle mode off */
#define ST7735S_CMD_IDMON      (0x39) /* Idle mode on */
#define ST7735S_CMD_COLMOD     (0x3A) /* Interface pixel format */
#define ST7735S_CMD_RDID1      (0xDA) /* Read ID1 */
#define ST7735S_CMD_RDID2      (0xDB) /* Read ID2 */
#define ST7735S_CMD_RDID3      (0xDC) /* Read ID3 */

// Panel function commands

#define ST7735S_CMD_FRMCTR1    (0xB1) /* Frame Control Setting - In normal mode - full colors */
#define ST7735S_CMD_FRMCTR2    (0xB2) /* Frame Control Setting - In idle mode - 8 colors */
#define ST7735S_CMD_FRMCTR3    (0xB3) /* Frame Control Setting - In partial mode + full colors */
#define ST7735S_CMD_INVCTR     (0xB4) /* Display inversion control */
#define ST7735S_CMD_PWCTR1     (0xC0) /* Power control setting - GVDD voltage */
#define ST7735S_CMD_PWCTR2     (0xC1) /* Power control setting - VGH/VGL voltage */
#define ST7735S_CMD_PWCTR3     (0xC2) /* Power control setting - In normal mode - full colors */
#define ST7735S_CMD_PWCTR4     (0xC3) /* Power control setting - In idle mode - 8 colors */
#define ST7735S_CMD_PWCTR5     (0xC4) /* Power control setting - In partial mode + full colors */
#define ST7735S_CMD_VMCTR1     (0xC5) /* VCOM Control 1 - VCOM Voltage */
#define ST7735S_CMD_VMOFCTR    (0xC7) /* Set VCOM offset control */
#define ST7735S_CMD_WRID2      (0xD1) /* Set LCM version code */
#define ST7735S_CMD_WRID3      (0xD2) /* Set customer project code */
#define ST7735S_CMD_NVCTR1     (0xD9) /* NVM control status */
#define ST7735S_CMD_NVCTR2     (0xDE) /* NVM read command */
#define ST7735S_CMD_NVCTR3     (0xDF) /* NVM write command */
#define ST7735S_CMD_GMCTRP1    (0xE0) /* Gamma adjustment (+pol) */
#define ST7735S_CMD_GMCTRN1    (0xE1) /* Gamma adjustment (-pol) */
#define ST7735S_CMD_GCV        (0xFC) /* Gate clock variable */

#define ST7735S_COLINV_NM      (0x1U << 0x0) /* Normal mode column inversion */
#define ST7735S_COLINV_IM      (0x1U << 0x1) /* Idle mode column inversion */
#define ST7735S_COLINV_PMFC    (0x1U << 0x2) /* Partial mode + full color column inversion */

#define ST7735S_MADCTL_MY      (0x1U << 0x7) /* Row address order */
#define ST7735S_MADCTL_MX      (0x1U << 0x6) /* Column address order */
#define ST7735S_MADCTL_MV      (0x1U << 0x5) /* Row/column exchange */
#define ST7735S_MADCTL_ML      (0x1U << 0x4) /* Vertical refresh order */
#define ST7735S_MADCTL_BGR     (0x1U << 0x3) /* RGB-BGR order */
#define ST7735S_MADCTL_MH      (0x1U << 0x2) /* Horizontal refresh order */

#define ST7735S_COLMOD_12BIT   (0b011) /* 12-bit */
#define ST7735S_COLMOD_16BIT   (0b101) /* 16-bit */
#define ST7735S_COLMOD_18BIT   (0b110) /* 18-bit */

/**
 * Initializes the SPI for the ST7735S v1.1 display driver.
 */
void init_st7735s(void);

/**
 * Buffer out n amount of data to SPI port.
 *
 * @param data The data.
 * @param n The amount of data.
 */
void st7735s_bufw8(uint8_t data, size_t n);

/**
 * Buffer out n amount of data to SPI port.
 *
 * @param data The data.
 * @param n The amount of data.
 */
void st7735s_bufw16(uint16_t data, size_t n);

/**
 * Cleans up after the bufw write.
 */
void st7735s_bufw_end(void);

/**
 * Run a reset pin cycle with ST7735S RST pin.
 */
void st7735s_rstcycl(void);

/**
 * Sends a SWRESET command.
 */
void st7735s_swreset(void);

/**
 * Sends a SLPOUT command.
 */
void st7735s_slpout(void);

/**
 * Sends a FRMCTR1 command
 *
 * @param rtna The RTN period
 * @param fpa The front porch
 * @param bpa The back porch
 */
void st7735s_frmctr1(uint8_t rtna, uint8_t fpa, uint8_t bpa);

/**
 * Sends a FRMCTR2 command
 *
 * @param rtnb The RTN period
 * @param fpb The front porch
 * @param bpb The back porch
 */
void st7735s_frmctr2(uint8_t rtnb, uint8_t fpb, uint8_t bpb);

/**
* Sends a FRMCTR3 command
*
* @param rtnc The RTN period
* @param fpc The front porch
* @param bpc The back porch
* @param rtnd The RTN period D
* @param fpd The front porch
* @param bpd The back porch
*/
void st7735s_frmctr3(uint8_t rtnc, uint8_t fpc, uint8_t bpc, uint8_t rtnd, uint8_t fpd, uint8_t bpd);

/**
 * Sends a INVCTR command
 *
 * @param nl The inversion setting.
 */ 
void st7735s_invctr(uint8_t nl);

/**
 * Sends a MADCTL command
 *
 * @param m The params.
 */
void st7735s_madctl(uint8_t m);

/**
 * Sends a COLMOD command.
 *
 * @param ifpf The colmod mode.
 */
void st7735s_colmod(uint8_t ifpf);

/**
 * Sends a DISPON command.
 */
void st7735s_dispon(void);

/**
 * Sends a RAMWR command.
 * Please note you will have to buffer after this.
 */
void st7735s_ramwr(void);

/**
 * Sends a CASET command.
 *
 * @param x1 The first X param.
 * @param x2 The second X param.
 */
void st7735s_caset(uint16_t x1, uint16_t x2);

/**
 * Sends a RASET command.
 *
 * @param y1 The first Y param.
 * @param y2 The second Y param.
 */
void st7735s_raset(uint16_t y1, uint16_t y2);

#endif // !ST7735S_H_
