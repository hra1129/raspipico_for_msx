cmake_minimum_required(VERSION 3.13)

add_executable( gpio_test2 )

target_sources( gpio_test2 PUBLIC
	main.cpp
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories( gpio_test2 PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

# pull in common dependencies
target_link_libraries(gpio_test2 PUBLIC pico_stdlib )

# create map/bin/hex file etc.
pico_add_extra_outputs( gpio_test2 )
