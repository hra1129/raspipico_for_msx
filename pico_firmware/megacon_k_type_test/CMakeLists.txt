cmake_minimum_required(VERSION 3.13)

add_executable( megacon_k_type_test )

target_sources( megacon_k_type_test PUBLIC
	main.cpp
)

# Make sure TinyUSB can find tusb_config.h
target_include_directories( megacon_k_type_test PUBLIC ${CMAKE_CURRENT_LIST_DIR} )

# pull in common dependencies
target_link_libraries(megacon_k_type_test PUBLIC pico_stdlib )

# create map/bin/hex file etc.
pico_add_extra_outputs( megacon_k_type_test )
