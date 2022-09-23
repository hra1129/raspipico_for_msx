// --------------------------------------------------------------------
//	PIO test
// ====================================================================
//	2022/9/15	t.hara
// --------------------------------------------------------------------

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pio_bank_address.pio.h"
#include "pio_bank_write.pio.h"
#include "pio_we_oe.pio.h"

// --------------------------------------------------------------------
static const int bank_access[] = {
	//		ビットシフトの量になっている
	//		0  .... バンクレジスタ2
	//		8  .... バンクレジスタ3
	//		16 .... バンクレジスタ0
	//		24 .... バンクレジスタ1
	//		32 .... 非バンクレジスタ
	//            A15 A14 A13 A12
	32,     //     0   0   0   0    非バンクレジスタ (page0)
	32,     //     0   0   0   1    非バンクレジスタ (page0)
	32,     //     0   0   1   0    非バンクレジスタ (page0)
	32,     //     0   0   1   1    非バンクレジスタ (page0)
	32,     //     0   1   0   0    非バンクレジスタ (page1)
	16,     //     0   1   0   1    バンクレジスタ0  (page1)
	32,     //     0   1   1   0    非バンクレジスタ (page1)
	24,     //     0   1   1   1    バンクレジスタ1  (page1)
	32,     //     1   0   0   0    非バンクレジスタ (page2)
	0 ,     //     1   0   0   1    バンクレジスタ2  (page2)
	32,     //     1   0   1   0    非バンクレジスタ (page2)
	16,     //     1   0   1   1    バンクレジスタ3  (page2)
	32,     //     1   1   0   0    非バンクレジスタ (page3)
	32,     //     1   1   0   1    非バンクレジスタ (page3)
	32,     //     1   1   1   0    非バンクレジスタ (page3)
	32,     //     1   1   1   1    非バンクレジスタ (page3)
};

// --------------------------------------------------------------------
static void gpio_settings( PIO pio, uint sm ) {
	int i;

	for( i = 0; i <= 27; i++ ) {
		pio_gpio_init( pio, i );
	}
	// PIOで GPIO0～7ピン (MA13-MA20) の方向を出力に設定する。
	pio_sm_set_consecutive_pindirs( pio, sm, 0, 8, true );
	// PIOで GPIO26～27ピン (/WE,/OE) の方向を出力に設定する。
	pio_sm_set_consecutive_pindirs( pio, sm, 26, 2, true );
	// PIOで GPIO8～22ピンの方向を入力に設定する。
	pio_sm_set_consecutive_pindirs( pio, sm, 8, 15, false );
}

// --------------------------------------------------------------------
int main() {
	uint pio0_offset;
	uint pio0_sm0, pio0_sm1, pio0_sm2;
	uint32_t d, bank_id;
	uint32_t bank_register = 0x01000302;

	pio0_offset = pio_add_program( pio0, &pio_bank_address_program );
	pio0_sm0 = pio_claim_unused_sm( pio0, true );
	gpio_settings( pio0, pio0_sm0 );
	pio_bank_address_program_init( pio0, pio0_sm0, pio0_offset, bank_register );

	pio0_offset = pio_add_program( pio0, &pio_bank_write_program );
	pio0_sm1 = pio_claim_unused_sm( pio0, true );
	gpio_settings( pio0, pio0_sm1 );
	pio_bank_write_program_init( pio0, pio0_sm1, pio0_offset );

	pio0_offset = pio_add_program( pio0, &pio_we_oe_program );
	pio0_sm2 = pio_claim_unused_sm( pio0, true );
	gpio_settings( pio0, pio0_sm2 );
	pio_we_oe_program_init( pio0, pio0_sm2, pio0_offset );

	while( true ) {
		//	pio_bank_write からバンクレジスタ更新らしきタイミングを取得
		d = pio_sm_get_blocking( pio0, pio0_sm1 );
		bank_id = bank_access[ d >> 8 ];
		if( bank_id == 32 ) {
			//	バンクレジスタじゃ無かったので無視
			continue;
		}
		//	バンクレジスタだったので pio_bank_address に新しいバンクレジスタ値を通知する
		bank_register = (bank_register & ~(0x000000FF << bank_id)) | ((d & 0x000000FF) << bank_id);
		pio_sm_put_blocking( pio0, pio0_sm0, bank_register );
	}
}
