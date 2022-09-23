cmake_minimum_required( VERSION 3.13 )

add_executable( megacon_k_type_compatible )

pico_generate_pio_header( megacon_k_type_compatible ${CMAKE_CURRENT_LIST_DIR}/pio_bank_address.pio )
pico_generate_pio_header( megacon_k_type_compatible ${CMAKE_CURRENT_LIST_DIR}/pio_bank_write.pio )
pico_generate_pio_header( megacon_k_type_compatible ${CMAKE_CURRENT_LIST_DIR}/pio_we_oe.pio )

target_sources( megacon_k_type_compatible PUBLIC
	main.cpp
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories( megacon_k_type_compatible PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

# pull in common dependencies
target_link_libraries( megacon_k_type_compatible PUBLIC pico_stdlib hardware_pio )

# enable usb output, disable uart output
pico_enable_stdio_usb( megacon_k_type_compatible 1 )
pico_enable_stdio_uart( megacon_k_type_compatible 0 )

# create map/bin/hex file etc.
pico_add_extra_outputs( megacon_k_type_compatible )
