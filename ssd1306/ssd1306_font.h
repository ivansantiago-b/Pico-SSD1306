/**
 * @file ssd1306_font.h
 * @brief Header file to manage fonts in ssd1306 library
 * @author Iván Santiago
 * @date 2023-08-02 20:27
*/
#ifndef SSD1306_FONT_H_
#define SSD1306_FONT_H_

#include <stdint.h>

typedef struct ssd1306_font
{
    const char first_character;
    const char last_character;
    const char overflow_character;
    const uint8_t *font_array;
    const uint32_t *character_offset;
    const uint8_t *character_width;
    const uint8_t character_height;
    const uint32_t *vertical_offsets;
    const uint8_t character_spacing;
} SSD1306_Font;
#endif