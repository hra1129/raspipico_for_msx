// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	GPIO test
//	Copyright (c) 2022 Takayuki Hara
//	
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
// --------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

static volatile uint32_t bank_register[4] = { 0x00 << 2, 0x01 << 2, 0x02 << 2, 0x03 << 2 };

// --------------------------------------------------------------------
int main( void ) {
	int i;

	gpio_init( 0 );
	gpio_init( 1 );
	gpio_set_dir( 0, false );	//	in: A13
	gpio_set_dir( 1, false );	//	in: A14

	for( i = 0; i < 8; i++ ) {
		gpio_init( i + 2 );
		gpio_set_dir( i + 2, true );	//	out
	}

	//	static inline void gpio_put_masked(uint32_t mask, uint32_t value) {
	//	    sio_hw->gpio_togl = (sio_hw->gpio_out ^ value) & mask;
	//	}
	//
	//	static inline uint32_t gpio_get_all(void) {
	//	    return sio_hw->gpio_in;
	//	}
	//
	//	//	ひたすら { A14, A13 } だけを見て、対応する bank_register の値を { MA20, MA19, MA18, MA17, MA16, MA15, MA14, MA13 } に出す。
	//	for(;;) {
	//		gpio_put_masked( 0xFF << 2, bank_register[ gpio_get_all() & address_mask ] );
	//	}

	//	ひたすら { A14, A13 } だけを見て、対応する bank_register の値を { MA20, MA19, MA18, MA17, MA16, MA15, MA14, MA13 } に出す。
	const uint32_t address_mask = 3;
	const uint32_t maddress_mask = 0xFF << 2;
	for(;;) {
		sio_hw->gpio_togl = (sio_hw->gpio_out ^ bank_register[ sio_hw->gpio_in & address_mask ] ) & maddress_mask;
	}
	return 0;
}
