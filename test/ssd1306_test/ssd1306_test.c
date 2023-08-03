#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "ssd1306.h"
#include "ssd1306_font7seg.h"
#include "ssd1306_font5x7.h"
#include "ssd1306_font7x9.h"
#include "ssd1306_font7x11.h"

#define SDA_PIN 8 /** pico pin 11 */
#define SCL_PIN 9 /** pico pin 12 */
#define DELAY 1000

int main(void)
{
    i2c_init(SSD1306_I2C, 400000u);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    SSD1306_Display *display = ssd1306_init();

    char str[97];
    for (int i = 0; i < 96; i++)
        str[i] = i + 32;
    str[96] = 0x00;
    for (;;)
    {
        ssd1306_clean(display);
        ssd1306_put_pixel(display, 0, 0);
        ssd1306_put_pixel(display, 127, 0);
        ssd1306_put_pixel(display, 64, 32);
        ssd1306_put_pixel(display, 0, 63);
        ssd1306_put_pixel(display, 127, 63);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_draw_line(display, 0, 0, 127, 63);
        ssd1306_draw_line(display, 127, 0, 0, 63);
        ssd1306_draw_line(display, 64, 0, 64, 16);
        ssd1306_draw_line(display, 0, 32, 16, 32);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_draw_ellipse(display, 64, 32, 60, 30);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_draw_circle(display, 64, 32, 20);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_set_font(display, &ssd1306_font7x11);
        ssd1306_print(display, str);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_set_font(display, &ssd1306_font7x11);
        ssd1306_println(display, "Hello");
        ssd1306_println(display, "world!");
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_set_font(display, &ssd1306_font7x11);
        ssd1306_print_aligned(display, "left", SSD1306_TEXT_LEFT);
        ssd1306_print_aligned(display, "center", SSD1306_TEXT_CENTER);
        ssd1306_print_aligned(display, "right", SSD1306_TEXT_RIGHT);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_set_font(display, &ssd1306_font5x7);
        ssd1306_print(display, str);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_set_font(display, &ssd1306_font7x9);
        ssd1306_print(display, str);
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);

        ssd1306_clean(display);
        ssd1306_set_font(display, &ssd1306_font7segment);
        ssd1306_print(display, "0:123456789.");
        ssd1306_update_graphics(display);
        sleep_ms(DELAY);
    }
    ssd1306_destroy_display(display);
    return 0;
}