#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "ssd1306.h"

#define SDA_PIN 8 /** pico pin 11 */
#define SCL_PIN 9 /** pico pin 12 */

void draw_branch(SSD1306_Display *d, int16_t sx, int16_t sy, float len, float angle, float angle_increment)
{
    if (len >= 1.0f)
    {
        int16_t rx = sx + (int16_t)(roundf(len * sinf(angle)));
        int16_t ry = sy - (int16_t)(roundf(len * cosf(angle)));
        ssd1306_draw_line(d, sx, sy, rx, ry);
        draw_branch(d, rx, ry, 0.7f * len, angle + angle_increment, angle_increment);
        draw_branch(d, rx, ry, 0.7f * len, angle - angle_increment, angle_increment);
    }
}

int main(void)
{
    i2c_init(SSD1306_I2C, 400000u);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    SSD1306_Display *display = ssd1306_init();

    uint8_t sx = 64;
    uint8_t sy = 42;
    float angle = 0.0f;
    float angle_increment = 0.0;
    float len = 22.0f;
    float inc = 0.02f;

    for (;;)
    {
        ssd1306_clean(display);
        ssd1306_draw_line(display, sx, 63, sx, sy);
        draw_branch(display, sx, sy, 0.7f * len, angle + angle_increment, angle_increment);
        draw_branch(display, sx, sy, 0.7f * len, angle - angle_increment, -angle_increment);
        angle_increment += inc;
        if (angle_increment >= 6.28318530717959f) angle_increment -= 6.28318530717959f;
        ssd1306_update_graphics(display);
        sleep_ms(10);
    }
    ssd1306_destroy_display(display);
    return 0;
}