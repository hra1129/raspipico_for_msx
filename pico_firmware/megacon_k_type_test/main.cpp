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

// --------------------------------------------------------------------
//  Raspberry Pi Pico (Top View)
//                 ______|~~~~~|______
//      o[D0] GP0  |  Åõ |_____| Åõ  | VBUS
//      o[D1] GP1  |  [ ]  USB       | VSYS
//            GND  |  LED        Å°  | GND
//      o[D2] GP2  |                 | 3V3_EN
//      o[D3] GP3  |  ( )     Å° Å°  | 3V3(OUT)
//      o[D4] GP4  |                 | ADC_VREF
//      o[D5] GP5  |                 | GP28
//            GND  |  Å°             | GND
//      o[D6] GP6  |                 | GP27
//      o[D7] GP7  |  y  Å°Å°Å°      | GP26
//            GP8  |  r  Å°Å°Å°      | RUN
//            GP9  |  r  Å°Å°Å°      | GP22
//            GND  |  e              | GND
//            GP10 |  b              | GP21
//            GP11 |  p              | GP20
//            GP12 |  s              | GP19 [A15]o
// o[/SLTSEL] GP13 |  a              | GP18 [A14]o
//            GND  |  R              | GND
//     o[/RD] GP14 |      DEBUG      | GP17 [A13]o
//     o[/WR] GP15 |  Åõ         Åõ  | GP16 [A12]o
//                 ~~~~~~~~~~~~~~~~~~~
// --------------------------------------------------------------------

// --------------------------------------------------------------------
int main( void ) {
	int address = 0, data = 0;
	static const int sltsel = 13;
	static const int wr = 15;
	static const int rd = 14;

	gpio_init( 0 );
	gpio_init( 1 );
	gpio_init( 2 );
	gpio_init( 3 );
	gpio_init( 4 );
	gpio_init( 5 );
	gpio_init( 6 );
	gpio_init( 7 );
	gpio_init( 13 );
	gpio_init( 14 );
	gpio_init( 15 );
	gpio_init( 16 );
	gpio_init( 17 );
	gpio_init( 18 );
	gpio_init( 19 );
	gpio_set_dir( 0, true );	//	out
	gpio_set_dir( 1, true );	//	out
	gpio_set_dir( 2, true );	//	out
	gpio_set_dir( 3, true );	//	out
	gpio_set_dir( 4, true );	//	out
	gpio_set_dir( 5, true );	//	out
	gpio_set_dir( 6, true );	//	out
	gpio_set_dir( 7, true );	//	out
	gpio_set_dir( 13, true );	//	out
	gpio_set_dir( 14, true );	//	out
	gpio_set_dir( 15, true );	//	out
	gpio_set_dir( 16, true );	//	out
	gpio_set_dir( 17, true );	//	out
	gpio_set_dir( 18, true );	//	out
	gpio_set_dir( 19, true );	//	out
	for(;;) {
		//	/WR phase ---------------------------------------
		//	A15-A12, D7-D0
		gpio_put_masked( 0x000F00FF, (address << 16) | data );
		//	/WR = 0
		gpio_put( wr, 0 );
		//	/SLTSEL = 0
		gpio_put( sltsel, 0 );
		sleep_us(1);
		//	/WR = 1
		gpio_put( wr, 1 );
		//	/SLTSEL = 1
		gpio_put( sltsel, 1 );
		sleep_us(1);

		//	/RD phase ---------------------------------------
		//	A15-A12, D7-D0
		gpio_put_masked( 0x000F00FF, (address << 16) | 0xFF );
		//	/RD = 0
		gpio_put( rd, 0 );
		//	/SLTSEL = 0
		gpio_put( sltsel, 0 );
		sleep_us(1);
		//	/RD = 1
		gpio_put( rd, 1 );
		//	/SLTSEL = 1
		gpio_put( sltsel, 1 );
		sleep_us(1);
		address = (address + 1) & 15;
	}
	return 0;
}
