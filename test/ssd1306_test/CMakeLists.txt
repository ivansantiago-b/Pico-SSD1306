add_executable(ssd1306_test ssd1306_test.c)
target_link_libraries(ssd1306_test
    pico_stdlib
    hardware_i2c
    hardware_gpio
    ssd1306
)

pico_add_extra_outputs(ssd1306_test)