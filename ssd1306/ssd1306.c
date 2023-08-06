#include "ssd1306.h"
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
    display->cursor_position = 1;
    display->line_limit = 128;

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
    ssd1306_write(init_commands, 27);

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
    d->cursor_position = 1;
    d->line_limit = 128;
    uint32_t i = d->frame_length;
    while (--i)
        *(d->frame + i) = 0x00;
}

void ssd1306_update_graphics(SSD1306_Display *d)
{
    ssd1306_write(d->frame, d->frame_length);
}

void ssd1306_draw_line(SSD1306_Display *d, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
    if ((y1 > 0 || y2 > 0) && (x1 > 0 || x2 > 0))
    {
        if (y1 < 0)
            y1 = 0;
        else if (y1 > d->max_y)
            y1 = d->max_y;
        if (y2 < 0)
            y2 = 0;
        else if (y2 > d->max_y)
            y2 = d->max_y;
        if (x1 < 0)
            x1 = 0;
        else if (x1 > d->max_x)
            x1 = d->max_x;
        if (x2 < 0)
            x2 = 0;
        else if (x2 > d->max_x)
            x2 = d->max_x;

        int16_t dx = x2 - x1;
        int16_t dy = y2 - y1;
        int16_t sx = 1;
        int16_t sy = 1;
        if (dx < 0)
        {
            dx = -dx;
            sx = -1;
        }
        if (dy < 0)
        {
            dy = -dy;
            sy = -1;
        }
        dy = -dy;
        int16_t e = dx + dy;
        int16_t de;
        for (;;)
        {
            ssd1306_put_pixel(d, (uint8_t)x1, (uint8_t)y1);
            de = 2 * e;
            if (de >= dy)
            {
                if (x1 == x2)
                    break;
                e += dy;
                x1 += sx;
            }
            if (de <= dx)
            {
                if (y1 == y2)
                    break;
                e += dx;
                y1 += sy;
            }
        }
    }
}

void ssd1306_draw_ellipse(SSD1306_Display *d, int8_t cx, int8_t cy, int8_t a, int8_t b)
{
    int32_t x = -a;
    int32_t y = 0;
    int32_t de = b;
    int32_t dx = (1 + 2 * x) * de * de;
    int32_t dy = x * x;
    int32_t e = dx + dy;
    do
    {
        ssd1306_put_pixel(d, cx - x, cy + y);
        ssd1306_put_pixel(d, cx + x, cy + y);
        ssd1306_put_pixel(d, cx + x, cy - y);
        ssd1306_put_pixel(d, cx - x, cy - y);
        de = 2 * e;
        if (de >= dx)
        {
            x++;
            e += dx += 2 * b * b;
        }
        if (de <= dy)
        {
            y++;
            e += dy += 2 * a * a;
        }
    } while (x <= 0);
    while (y++ < b)
    {
        ssd1306_put_pixel(d, cx, cy + y);
        ssd1306_put_pixel(d, cx, cy - y);
    }
}

void ssd1306_draw_circle(SSD1306_Display *d, int8_t cx, int8_t cy, int8_t r)
{
    int16_t x = -r;
    int16_t y = 0;
    int16_t e = 2 - 2 * r;
    do
    {
        ssd1306_put_pixel(d, cx - x, cy + y);
        ssd1306_put_pixel(d, cx - y, cy - x);
        ssd1306_put_pixel(d, cx + x, cy - y);
        ssd1306_put_pixel(d, cx + y, cy + x);
        r = e;
        if (r <= y)
            e += ++y * 2 + 1;
        if (r > x || e > y)
            e += ++x * 2 + 1;
    } while (x < 0);
}

void ssd1306_set_font(SSD1306_Display *d, SSD1306_Font *f)
{
    d->font = f;
    ssd1306_set_cursor(d, 0, 0);
}

void ssd1306_set_cursor(SSD1306_Display *d, uint8_t c, uint8_t r)
{
    if (r > (d->pages - d->font->character_height))
        r = d->pages - d->font->character_height;
    if (c > d->max_x)
        c = 0;
    d->cursor_position = 1 + c + r * d->width;
    d->line_limit = d->width * (r + 1);
}

void ssd1306_print(SSD1306_Display *d, const char *text)
{
    uint32_t i = 0;
    while (*(text + i))
    {
        uint32_t character = text[i] - d->font->first_character;
        if (character > (d->font->last_character - d->font->first_character))
        {
            character = d->font->overflow_character - d->font->first_character;
        }
        uint8_t char_width = d->font->character_width[character];
        if ((d->cursor_position + char_width) > d->line_limit)
        {
            uint8_t p = d->line_limit / d->width;
            if (p == d->pages - 1)
                break;
            else
                ssd1306_set_cursor(d, 0, (p + (d->font)->character_height) - 1);
        }
        for (int j = 0; j < char_width; j++)
        {
            uint32_t offset = (d->font)->character_offset[character] + j;
            d->frame[d->cursor_position + j] |= (d->font)->font_array[offset];
            uint32_t vertical_index = d->cursor_position + d->width + j;
            for (int k = 0; k < ((d->font)->character_height - 1); k++)
            {
                d->frame[vertical_index] |= (d->font)->font_array[(d->font)->vertical_offsets[k] + offset];
                vertical_index += d->width;
            }
        }
        d->cursor_position += char_width + d->font->character_spacing;
        i++;
    }
}

void ssd1306_println(SSD1306_Display *d, const char *text)
{
    uint32_t i = 0;
    while (*(text + i))
    {
        uint32_t character = text[i] - d->font->first_character;
        if (character > (d->font->last_character - d->font->first_character))
        {
            character = d->font->overflow_character - d->font->first_character;
        }
        uint8_t char_width = d->font->character_width[character];
        if ((d->cursor_position + char_width) > d->line_limit)
        {
            break;
        }
        for (int j = 0; j < char_width; j++)
        {
            uint32_t offset = (d->font)->character_offset[character] + j;
            d->frame[d->cursor_position + j] |= (d->font)->font_array[offset];
            uint32_t vertical_index = d->cursor_position + d->width + j;
            for (int k = 0; k < ((d->font)->character_height - 1); k++)
            {
                d->frame[vertical_index] |= (d->font)->font_array[(d->font)->vertical_offsets[k] + offset];
                vertical_index += d->width;
            }
        }
        d->cursor_position += char_width + d->font->character_spacing;
        i++;
    }
    uint8_t p = d->line_limit / d->width;
    if (p != d->pages - 1)
    {
        ssd1306_set_cursor(d, 0, (p + (d->font)->character_height) - 1);
    }
}

void ssd1306_print_aligned(SSD1306_Display *d, const char *text, uint8_t a)
{
    uint32_t i = 0;
    uint32_t text_width = 0;
    while (*(text + i))
    {
        uint32_t character = text[i] - d->font->first_character;
        if (character > (d->font->last_character - d->font->first_character))
        {
            character = d->font->overflow_character - d->font->first_character;
        }
        text_width += d->font->character_width[character];
        text_width += d->font->character_spacing;
        i++;
    }
    text_width -= d->font->character_spacing;
    uint8_t c = 0;
    uint8_t r = d->line_limit / d->width;
    if (text_width < d->width)
    {
        switch (a)
        {
        case SSD1306_TEXT_CENTER:
            c = (d->max_x - text_width) >> 1;
            break;
        case SSD1306_TEXT_RIGHT:
            c = d->max_x - text_width;
        default:
            break;
        }
    }
    ssd1306_set_cursor(d, c, r);
    ssd1306_println(d, text);
}

void ssd1306_activate_horizontal_scroll(uint8_t rl, uint8_t start_page, uint8_t end_page, uint8_t frame_rate)
{
    const uint8_t scroll_commands[9] = {
        SSD1306_CONTROL_BYTE_COMMAND,
        rl ? SSD1306_CONTINUOUS_HORIZONTAL_SCROLL_RIGHT : SSD1306_CONTINUOUS_HORIZONTAL_SCROLL_LEFT,
        SSD1306_DUMMY_BYTE_00,
        start_page,
        frame_rate,
        end_page,
        SSD1306_DUMMY_BYTE_00,
        SSD1306_DUMMY_BYTE_FF,
        SSD1306_ACTIVATE_SCROLL};
    ssd1306_write(scroll_commands, 9);
}
void ssd1306_activate_vertical_and_horizontal_scroll(uint8_t rl, uint8_t start_page, uint8_t end_page, uint8_t start_row, uint8_t end_row, uint8_t vertical_scrolling_offset, uint8_t frame_rate)
{
    const uint8_t scroll_commands[11] = {
        SSD1306_CONTROL_BYTE_COMMAND,
        SSD1306_SET_VERTICAL_SCROLL_AREA,
        start_row,
        end_row,
        rl ? SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL : SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL,
        SSD1306_DUMMY_BYTE_00,
        start_page,
        frame_rate,
        end_page,
        vertical_scrolling_offset,
        SSD1306_ACTIVATE_SCROLL
    };
    ssd1306_write(scroll_commands, 11);
}

void ssd1306_deactivate_scroll(void)
{
    const uint8_t deactivation_command[2] = 
    {
        SSD1306_CONTROL_BYTE_COMMAND,
        SSD1306_DEACTIVATE_SCROLL
    };
    ssd1306_write(deactivation_command, 2);
}