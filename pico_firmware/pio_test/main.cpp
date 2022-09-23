// --------------------------------------------------------------------
//	PIO test
// ====================================================================
//	2022/9/15	t.hara
// --------------------------------------------------------------------

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pio_test.pio.h"

int main() {
	PIO pio = pio0;
//	uint32_t d = 0;

//    stdio_init_all();

	uint offset = pio_add_program( pio, &pio_test_program );
	uint sm = pio_claim_unused_sm( pio, true );
	pio_test_program_init( pio, sm, offset );

	while (true) {
//		pio_sm_put_blocking( pio, sm, 0x01000302 );
//		d = pio_sm_get_blocking( pio, sm );
//		printf( "%08X ", d );
		sleep_ms(10);
//		pio_sm_put_blocking( pio, sm, 0x01000302 );
//		d = pio_sm_get_blocking( pio, sm );
//		printf( "%08X ", d );
		sleep_ms(10);
//		pio_sm_put_blocking( pio, sm, 0x01000302 );
//		d = pio_sm_get_blocking( pio, sm );
//		printf( "%08X ", d );
		sleep_ms(10);
//		pio_sm_put_blocking( pio, sm, 0x01000302 );
//		d = pio_sm_get_blocking( pio, sm );
//		printf( "%08X ", d );
		sleep_ms(10);
	}
}
