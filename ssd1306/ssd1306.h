/**
 * @file ssd1306.h
 * @brief Library to drive a SSD1306 128x64 dot matrix OLED display using I2C
 * @author Iván Santiago
 * @date 2023-07-31 20:43
 */
#ifndef SSD1306_H_
#define SSD1306_H_

// #include <stdint.h>
#include "hardware/i2c.h"

/**
 * I2C instance
 */
#define SSD1306_I2C i2c0
/**
 * SSD1306 I2C address
 */
#define SSD1306_ADDRESS 0x3C
/**
 * Set constrast control command: Double byte command to select 1 out of 256 contrast steps
 * @note Reset 0x7F
 */
#define SSD1306_SET_CONTRAST_CONTROL 0x81
/**
 * Entire display on command: Resume to RAM content display
 * @note Reset 0xA4
 */
#define SSD1306_RESUME_TO_RAM_CONTENT 0xA4
/**
 * Entire display on command: Entire display ON
 * @note Reset 0xA4
 */
#define SSD1306_ENTIRE_DISPLAY_ON 0xA5
/**
 * Set normal/inverse display command:
 * Normal display: 0 in RAM -> off in display panel, 1 in RAM -> on in display panel
 * @note Reset 0xA6
 */
#define SSD1306_SET_NORMAL_DISPLAY 0xA6
/**
 * Set normal/inverse display command:
 * Inverse display: 0 in RAM -> on in display panel, 1 in RAM -> off in display panel
 * @note Reset 0xA6
 */
#define SSD1306_SET_INVERSE_DISPLAY 0xA7
/**
 * Set display on/off command: Display off (sleep mode)
 * @note Reset 0xAE
 */
#define SSD1306_SET_DISPLAY_OFF 0xAE
/**
 * Set display on/off command: Display on in normal mode
 * @note Reset 0xAE
 */
#define SSD1306_SET_DISPLAY_ON 0xAF
/**
 * Dummy byte 0x00
 */
#define SSD1306_DUMMY_BYTE_00 0x00
/**
 * Dummy byte 0xFF
 */
#define SSD1306_DUMMY_BYTE_FF 0xFF
/**
 * Continuous horizontal scroll setup command: Right horizontal scroll
 * 0x26/0x27
 * A[7:0] dummy byte 0x00
 * B[2:0] define start page address (0 - 7)
 * C[2:0] set time interval between each scroll step in terms of frame frequency
 * 0 - 5 frames
 * 1 - 64 frames
 * 2 - 128 frames
 * 3 - 256 frames
 * 4 - 3 frames
 * 5 - 4 frames
 * 6 - 25 frames
 * 7 - 2 frames
 * D[2:0] define end page address (0 - 7)
 * E[7:0] dummy byte 0x00
 * F[7:0] dymmy byte 0xFF
 */
#define SSD1306_CONTINUOUS_HORIZONTAL_SCROLL_RIGHT 0x26
/**
 * Continuous horizontal scroll setup command: Left horizontal scroll
 * 0x26/0x27
 * A[7:0] dummy byte 0x00
 * B[2:0] define start page address (0 - 7)
 * C[2:0] set time interval between each scroll step in terms of frame frequency
 * 0 - 5 frames
 * 1 - 64 frames
 * 2 - 128 frames
 * 3 - 256 frames
 * 4 - 3 frames
 * 5 - 4 frames
 * 6 - 25 frames
 * 7 - 2 frames
 * D[2:0] define end page address (0 - 7)
 * E[7:0] dummy byte 0x00
 * F[7:0] dymmy byte 0xFF
 */
#define SSD1306_CONTINUOUS_HORIZONTAL_SCROLL_LEFT 0x27
/**
 * Continuous vertical and horizontal scroll setup command: Vertical and right horizontal scroll
 * A[7:0] dummy byte 0x00
 * B[2:0] define start page address (0 - 7)
 * C[2:0] set time interval between each scroll step in terms of frame frequency
 * 0 - 5 frames
 * 1 - 64 frames
 * 2 - 128 frames
 * 3 - 256 frames
 * 4 - 3 frames
 * 5 - 4 frames
 * 6 - 25 frames
 * 7 - 2 frames
 * D[2:0] define end page address (0 - 7)
 * E[5:0] vertical scrolling offset
 */
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
/**
 * Continuous vertical and horizontal scroll setup command: Vertical and left horizontal scroll
 * A[7:0] dummy byte 0x00
 * B[2:0] define start page address (0 - 7)
 * C[2:0] set time interval between each scroll step in terms of frame frequency
 * 0 - 5 frames
 * 1 - 64 frames
 * 2 - 128 frames
 * 3 - 256 frames
 * 4 - 3 frames
 * 5 - 4 frames
 * 6 - 25 frames
 * 7 - 2 frames
 * D[2:0] define end page address (0 - 7)
 * E[5:0] vertical scrolling offset
 */
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
/**
 * Deactivate scroll command: Stop scrolling that is configured by command
 * 0x26/0x27/0x29/0x2A
 */
#define SSD1306_DEACTIVATE_SCROLL 0x2E
/**
 * Activate scroll command: Start scrolling that is configured by the scrolling setup
 * commands: 0x26/0x27/0x29/0x2A
 */
#define SSD1306_ACTIVATE_SCROLL 0x2F
/**
 * Set vertical scroll area command:
 * A[5:0] Set No. of rows in top fixed area. The No. of
 * rows in top fixed area is referenced to the top of the GDDRAM.
 * B[6:0] Set No. of rows in scroll area. This is the number of rows
 * to be used for vertical scrolling. The scroll area starts in the first
 * row below the top fixed area.
 * @note Reset A[5:0] 0
 * @note Reset B[6:0] 64
 */
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
/**
 * Set lower column start address for page addressing mode command:
 * @param X Set the lower nibble of the column start address
 * register for page addressing mode using X[3:0]
 * as data bits. The initial display line register is
 * reset to 0000b after RESET
 * @note This command is only for page addressing mode
 */
#define SSD1306_SET_LOWER_COLUMN_START_ADDRESS(X) (0x00 | ((X)&0x0F))
/**
 * Set higher column start address for page addressing mode command:
 * @param X Set the higher nibble of the column start address
 * register for Page Addressing Mode using X[3:0] as data bits.
 * The initial display line register is reset to 0000b after RESET.
 * @note This command is only for page addressing mode
 */
#define SSD1306_SET_HIGHER_COLUMN_START_ADDRESS(X) (0x10 | ((X)&0x0F))
/**
 * Set memory addressing mode command:
 * A[1:0] Selects memory addressing mode
 * @note Reset A[1:0] = 2 page addressing mode
 */
#define SSD1306_SET_MEMORY_ADDRESSING_MODE 0x20
/**
 * Horizontal addressing mode
 */
#define SSD1306_HORIZONTAL_ADDRESSING_MODE 0x00
/**
 * Vertical addressing mode
 */
#define SSD1306_VERTICAL_ADDRESSING_M0DE 0x01
/**
 * Page addressing mode
 */
#define SSD1306_PAGE_ADDRESING_MODE 0x02
/**
 * Set column address command: Setup column start and end address
 * A[6:0] : Column start address, range: 0 - 127
 * B[6:0]: Column end address, range: 0 - 127
 * @note Reset A[6:0] 0
 * @note Reset B[6:0] 127
 * @note This command is only for horizontal or vertical addressing mode
 */
#define SSD1306_SET_COLUMN_ADDRESS 0x21
/**
 * Set page address command: Setup page start and end address
 * A[2:0] Page start address, range: 0 - 7
 * b[2:0] Page end addresss, range: 0 - 7
 * @note Reset A[2:0] 0
 * @note Reset B[2:0] 7
 * @note This command is only for horizontal or vertical addressing mode
 */
#define SSD1306_SET_PAGE_ADDRESS 0x22
/**
 * Set page start address for page addressing mode command:
 * @param X Set GDDRAM page start address for page addressing mode
 * using X[2:0]
 * @note This command is only for page addressing mode
 */
#define SSD1306_SET_PAGE_START_ADDRESS(X) (0xB0 | ((X)&0x07))
/**
 * Set display start line command:
 * @param X Set display RAM display start line register from 0-63
 * @note Reset X[6:0] 0
 */
#define SSD1306_SET_DISPLAY_START_LINE(X) (0x40 | ((X)&0x3F))
/**
 * Set segment re-map command: Column address 0 is mapped to SEG0
 * @note Reset 0xA0
 */
#define SSD1306_SET_SEGMENT_REMAP_0 0xA0
/**
 * Set segment re-map command: Column address 127 is mapped to SEG0
 * @note Reset 0xA0
 */
#define SSD1306_SET_SEGMENT_REMAP_127 0xA1
/**
 * Set multiplex ratio command: Set MUX ratio to N+1 MUX
 * A[5:0] from 16MUX to 64MUX, A[5:0] from 0 to 14 are invalid entry.
 * @note Reset A[5:0] 0x3F (63)
 */
#define SSD1306_SET_MULTIPLEX_RATIO 0xA8
/**
 * Set COM output scan direction command: Normal mode. Scan from COM0 to COM[N –1]
 * @note N is the multiplex ratio
 * @note Reset 0xC0
 */
#define SSD1306_SET_COM_SCAN_DIRECTION_NORMAL_MODE 0xC0
/**
 * Set COM output scan direction command: Remapped mode. Scan from COM[N-1] to COM0
 * @note N is the multiplex ratio
 * @note Reset 0xC0
 */
#define SSD1306_SET_COM_SCAN_DIRECTION_REMAPPED_MODE 0xC8
/**
 * Set display offset command: Set vertical shift by COM from 0 - 63
 * A[5:0]
 * @note Reset A[5:0] 0
 */
#define SSD1306_SET_DISPLAY_OFFSET 0xD3
/**
 * Set COM pins hardware configuration command: A[5:4]
 * A[7:0] = 0x02 Sequential COM pin configuration and disable COM left/right remap
 * A[7:0] = 0x22 Sequential COM pin configuration and enable COM left/right remap
 * A[7:0] = 0x12 Alternative COM pin configuration and disable COM left/right remap
 * A[7:0] = 0x32 Alternative COM pin configuration and enable COM left/right remap
 * @note Reset A[7:0] 0x12
 */
#define SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION 0xDA
/**
 * Set display clock divide ratio/oscillator frequency command:
 * A[3:0] Define the divide ratio (D) of the display clocks (DCLK): Divide ratio= A[3:0] + 1
 * A[7:4] Set the Oscillator Frequency, FOSC. Oscillator Frequency increases with the value of A[7:4] and vice versa
 * @note Reset A[3:0] 0 (divide ratio = 1)
 * @note Reset A[7:4] 8
 * @note Reset A[7:0] 0x80
 */
#define SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO 0xD5
/**
 * Set pre-charge period: A[7:0]
 * A[3:0] Phase 1 period of up to 15 DCLK clocks 0 is invalid entry
 * A[7:4] Phase 2 period of up to 15 DCLK clocks 0 is invalid entry
 * @note Reset A[3:0] 2
 * @note Reset A[7:4] 2
 * @note Reset A[7:0] 0x22
 */
#define SSD1306_SET_PRECHARGE_PERIOD 0xD9
/**
 * Set V_{COMH} deselect level: A[6:4]
 * A[7:0] 0x00 ~ 0.65 V_{CC}
 * A[7:0] 0x20 ~ 0.77 V_{CC}
 * A[7:0] 0x30 ~ 0.83 V_{CC}
 * @note Reset A[7:0] 0x20
 */
#define SSD1306_SET_VCOMH_DESELECT_LEVEL 0xDB
/**
 * No operation command: Command for no operation
 */
#define SSD1306_NOP_COMMAND 0xE3
/**
 * Charge pump setting command:
 * A[7:0] 0x10 disable charge pump
 * A[7:0] 0x14 enable charge pump during display on
 * @note Reset A[7:0] 0x10
 */
#define SSD1306_CHARGE_PUMP_SETTING 0x8D
/**
 * Control byte that indicates that the following byte is a command
 */
#define SSD1306_CONTROL_BYTE_COMMAND 0x00
/**
 * Control byte that indicates that the following byte is a data
 */
#define SSD1306_CONTROL_BYTE_DATA 0x40

typedef struct ssd1306_display
{
    uint8_t width;
    uint8_t heigth;
    uint8_t pages;
    uint8_t max_x;
    uint8_t max_y;
    uint32_t frame_length;
    uint8_t *frame;
} SSD1306_Display;

/**
 * Set the display to a resolution of 128x64 dots in normal mode
 * and horizontal addressing mode
 * @return a pointer to SSD1306_Display
 */
SSD1306_Display *ssd1306_init(void);

/**
 * Deallocates the memory used by a SSD1306_Display
 * @param d pointer to SSD1306_Display
 */
void ssd1306_destroy_display(SSD1306_Display *d);

/**
 * Fills the frame buffer of a SSD1306_Display with 0x00
 * @param d pointer to SSD1306_Display
 */
void ssd1306_clean(SSD1306_Display *d);

/**
 * Writes the frame content of SSD1306_Display on the SSD1306 GDDRAM
 * @param d pointer to SSD1306_Display
 */
void ssd1306_update_graphics(SSD1306_Display *d);

/**
 * Sets a pixel in the (x, y) position
 * @param d pointer to SSD1306_Display
 * @param x position on the x-axis
 * @param y position on the y-axis
 */
static inline void ssd1306_put_pixel(SSD1306_Display *d, uint8_t x, uint8_t y)
{
    if (x < d->width && y < d->heigth)
    {
        uint32_t index = 1 + x + (y >> 3) * d->width;
        if (d->frame[index] < 0xFF)
        {
            uint32_t value = 1 << (y % 8);
            d->frame[index] |= value;
        }
    }
}

/**
 * Draws a line from (x1, y1) to (x2, y2) on the SSD1306_Display frame
 * @param d pointer to SSD1306_Display
 * @param x1 first point x-axis position
 * @param y1 first point y-axis position
 * @param x2 second point x-axis position
 * @param y2 second point y-axis position
 */
void ssd1306_draw_line(SSD1306_Display *d, int8_t x1, int8_t y1, int8_t x2, int8_t y2);

/**
 * Draws an ellipse with center (cx, cy) on the SSD1306_Display frame
 * @param d pointer to SSD1306_Display
 * @param cx center point x-axis position
 * @param cy center point y-axis position
 * @param a horizontal length
 * @param b vertical length
*/
void ssd1306_draw_ellipse(SSD1306_Display *d, int8_t cx, int8_t cy, int8_t a, int8_t b);

/**
 * Draws a circle with center (cx, xy) and radio r on the SSD1306_Display frame
 * @param d pointer to SSD1306_Display
 * @param cx center point x-axis position
 * @param cy center point y-axis position
 * @param r radius
*/
void ssd1306_draw_circle(SSD1306_Display *d, int8_t cx, int8_t cy, int8_t r);
#endif