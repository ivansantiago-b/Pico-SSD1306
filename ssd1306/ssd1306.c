#include "ssd1306.h"
#include "hardware/i2c.h"
#include <stdlib.h>

SSD1306_Display *ssd1306_init(void)
{
    uint8_t *buffer = malloc(sizeof(uint8_t) * 1025);
    *(buffer) = SSD1306_CONTROL_BYTE_DATA;

    SSD1306_Display *display = malloc(sizeof(SSD1306_Display));
    display->width = 128;
    display->heigth = 64;
    display->pages = 8;
    display->max_x = 127;
    display->max_y = 64;
    display->frame_length = 1025;
    display->frame = buffer;

    const uint8_t init_commands[27] = {
        SSD1306_CONTROL_BYTE_COMMAND,
        SSD1306_SET_MULTIPLEX_RATIO, 0x3F,
        SSD1306_SET_DISPLAY_OFFSET, 0x00,
        SSD1306_SET_DISPLAY_START_LINE(0x00),
        SSD1306_SET_SEGMENT_REMAP_0,
        SSD1306_SET_COM_SCAN_DIRECTION_NORMAL_MODE,
        SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION, 0x12,
        SSD1306_SET_CONTRAST_CONTROL, 0x7F,
        SSD1306_RESUME_TO_RAM_CONTENT,
        SSD1306_SET_NORMAL_DISPLAY,
        SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO, 0x80,
        SSD1306_CHARGE_PUMP_SETTING, 0x14,
        SSD1306_SET_DISPLAY_ON,
        SSD1306_SET_MEMORY_ADDRESSING_MODE,
        SSD1306_HORIZONTAL_ADDRESSING_MODE,
        SSD1306_SET_COLUMN_ADDRESS, 0x00, 0x7F,
        SSD1306_SET_PAGE_ADDRESS, 0x00, 0x07};
    i2c_write_blocking(SSD1306_I2C, SSD1306_ADDRESS, init_commands, 27, false);

    return display;
}

void ssd1306_destroy_display(SSD1306_Display *d)
{
    if (d != NULL)
    {
        if (d->frame != NULL)
            free(d->frame);
        free(d->frame);
    }
}

void ssd1306_clean(SSD1306_Display *d)
{
    uint32_t i = d->frame_length;
    while (--i)
        *(d->frame + i) = 0x00;
}

void ssd1306_update_graphics(SSD1306_Display *d)
{
    i2c_write_blocking(SSD1306_I2C, SSD1306_ADDRESS, d->frame, d->frame_length, false);
}

void ssd1306_put_pixel(SSD1306_Display *d, uint8_t x, uint8_t y)
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