#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "ssd1306.h"

#define SDA_PIN 8 /** pico pin 11 */
#define SCL_PIN 9 /** pico pin 12 */

int main(void)
{
    i2c_init(SSD1306_I2C, 400000u);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    SSD1306_Display *display = ssd1306_init();
    ssd1306_clean(display);
    // ssd1306_put_pixel(display, 0, 0);
    // ssd1306_put_pixel(display, 127, 0);
    // ssd1306_put_pixel(display, 64, 32);
    // ssd1306_put_pixel(display, 0, 63);
    // ssd1306_put_pixel(display, 127, 63);
    ssd1306_draw_line(display, 0, 0, 127, 63);
    ssd1306_draw_line(display, 127, 0, 0, 63);
    ssd1306_draw_line(display, 64, 0, 64, 16);
    ssd1306_draw_line(display, 0, 32, 16, 32);
    ssd1306_update_graphics(display);
    return 0;
}