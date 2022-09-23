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
//  Raspberry Pi Pico
//                ______|~~~~~|______
//           GP0  |  Åõ |_____| Åõ  | VBUS
//           GP1  |  [ ]  USB       | VSYS
//           GND  |  LED        Å°  | GND
//           GP2  |                 | 3V3_EN
//           GP3  |  ( )     Å° Å°  | 3V3(OUT)
//           GP4  |                 | ADC_VREF
//           GP5  |                 | GP28
//           GND  |  Å°             | GND
//           GP6  |                 | GP27
//           GP7  |  y  Å°Å°Å°      | GP26
//           GP8  |  r  Å°Å°Å°      | RUN
//           GP9  |  r  Å°Å°Å°      | GP22
//           GND  |  e              | GND
//           GP10 |  b              | GP21
//           GP11 |  p              | GP20
//           GP12 |  s              | GP19
// [/SLTSEL] GP13 |  a              | GP18 [A15]
//           GND  |  R              | GND
//     [/RD] GP14 |      DEBUG      | GP17 [A14]
//           GP15 |  Åõ         Åõ  | GP16 [A13]
//                ~~~~~~~~~~~~~~~~~~~
// --------------------------------------------------------------------

// --------------------------------------------------------------------
int main( void ) {
	int address = 0;
	static const int sltsel = 13;
	static const int rd = 14;

	gpio_init( 13 );
	gpio_init( 14 );
	gpio_init( 16 );
	gpio_init( 17 );
	gpio_init( 18 );
	gpio_set_dir( 13, true );	//	out
	gpio_set_dir( 14, true );	//	out
	gpio_set_dir( 16, true );	//	out
	gpio_set_dir( 17, true );	//	out
	gpio_set_dir( 18, true );	//	out
	for(;;) {
		//	A13-A15
		gpio_put_masked( 7 << 16, address << 16 );
		//	/RD = 0
		gpio_put( rd, 0 );
		//	/SLTSEL = 0
		gpio_put( sltsel, 0 );
		address = (address + 1) & 7;
		sleep_us(1);
		//	/RD = 1
		gpio_put( rd, 1 );
		//	/SLTSEL = 1
		gpio_put( sltsel, 1 );
		sleep_us(1);
	}
	return 0;
}
