cmake_minimum_required(VERSION 3.13)

add_executable( pio_test )

pico_generate_pio_header( pio_test ${CMAKE_CURRENT_LIST_DIR}/pio_test.pio )

target_sources( pio_test PUBLIC
	main.cpp
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories( pio_test PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

# pull in common dependencies
target_link_libraries(pio_test PUBLIC pico_stdlib hardware_pio)

# enable usb output, disable uart output
pico_enable_stdio_usb(pio_test 1)
pico_enable_stdio_uart(pio_test 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(pio_test)
