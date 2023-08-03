add_executable(fractal_tree fractal_tree.c)
target_link_libraries(fractal_tree
    pico_stdlib
    hardware_i2c
    hardware_gpio
    ssd1306
)

pico_add_extra_outputs(fractal_tree)