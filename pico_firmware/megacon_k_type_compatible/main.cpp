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
	//		�r�b�g�V�t�g�̗ʂɂȂ��Ă���
	//		0  .... �o���N���W�X�^2
	//		8  .... �o���N���W�X�^3
	//		16 .... �o���N���W�X�^0
	//		24 .... �o���N���W�X�^1
	//		32 .... ��o���N���W�X�^
	//            A15 A14 A13 A12
	32,     //     0   0   0   0    ��o���N���W�X�^ (page0)
	32,     //     0   0   0   1    ��o���N���W�X�^ (page0)
	32,     //     0   0   1   0    ��o���N���W�X�^ (page0)
	32,     //     0   0   1   1    ��o���N���W�X�^ (page0)
	32,     //     0   1   0   0    ��o���N���W�X�^ (page1)
	16,     //     0   1   0   1    �o���N���W�X�^0  (page1)
	32,     //     0   1   1   0    ��o���N���W�X�^ (page1)
	24,     //     0   1   1   1    �o���N���W�X�^1  (page1)
	32,     //     1   0   0   0    ��o���N���W�X�^ (page2)
	0 ,     //     1   0   0   1    �o���N���W�X�^2  (page2)
	32,     //     1   0   1   0    ��o���N���W�X�^ (page2)
	16,     //     1   0   1   1    �o���N���W�X�^3  (page2)
	32,     //     1   1   0   0    ��o���N���W�X�^ (page3)
	32,     //     1   1   0   1    ��o���N���W�X�^ (page3)
	32,     //     1   1   1   0    ��o���N���W�X�^ (page3)
	32,     //     1   1   1   1    ��o���N���W�X�^ (page3)
};

// --------------------------------------------------------------------
static void gpio_settings( PIO pio, uint sm ) {
	int i;

	for( i = 0; i <= 27; i++ ) {
		pio_gpio_init( pio, i );
	}
	// PIO�� GPIO0�`7�s�� (MA13-MA20) �̕������o�͂ɐݒ肷��B
	pio_sm_set_consecutive_pindirs( pio, sm, 0, 8, true );
	// PIO�� GPIO26�`27�s�� (/WE,/OE) �̕������o�͂ɐݒ肷��B
	pio_sm_set_consecutive_pindirs( pio, sm, 26, 2, true );
	// PIO�� GPIO8�`22�s���̕�������͂ɐݒ肷��B
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
		//	pio_bank_write ����o���N���W�X�^�X�V�炵���^�C�~���O���擾
		d = pio_sm_get_blocking( pio0, pio0_sm1 );
		bank_id = bank_access[ d >> 8 ];
		if( bank_id == 32 ) {
			//	�o���N���W�X�^���ᖳ�������̂Ŗ���
			continue;
		}
		//	�o���N���W�X�^�������̂� pio_bank_address �ɐV�����o���N���W�X�^�l��ʒm����
		bank_register = (bank_register & ~(0x000000FF << bank_id)) | ((d & 0x000000FF) << bank_id);
		pio_sm_put_blocking( pio0, pio0_sm0, bank_register );
	}
}
