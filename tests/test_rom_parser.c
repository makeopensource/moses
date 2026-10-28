#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "moses/rom_parser.h"
#include "test_utils.h"

/*	The data from a NES 2.0 ROM header should be correctly parsed and loaded into the RomDescriptor struct. As feature support improves, more fields should be correctly parsed and tested, but this covers the basic information (mapper type, console timing, description of memory banks).
*/

//	This is the iNes format .nes header from Donkey Kong (1981).
//	sha1:c4eb14a59b853a58f53538e07c5efac56ddfe5fe
//	It contains no proprietary game data, only the header.
static uint8_t const header_dk[16] = {
	0x4e, 0x45, 0x53, 0x1a,
	0x01, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x61,
	0x73, 0x74, 0x65, 0x72
};

// This is the NES 2.0 format .nes header from Super Mario Bros. 3 (1988).
// sha1:a25d2dcbf6ec3634a84318bfe2cfe28d5331d815
// It contains no proprietary game data, only the header.
static uint8_t const header_smb3[16] = {
	0x4e, 0x45, 0x53, 0x1a,
	0x10, 0x10, 0x40, 0x08,
	0x00, 0x00, 0x07, 0x00,
	0x00, 0x00, 0x00, 0x01
};

// These structs are static global to ensure their zero-initialization,
// for the sake of easy memcmp comparison.

static RomDescriptor const expected_dk = {
	.mapper_id = 0,
	.hardwired_vertical_mirroring = false,
	.timing_mode = RD_TIMING_NTSC,
	.console_type = RD_CONSOLE_NES,
	.prg_rom_size = 16384,
	.chr_rom_size = 8192,
	// Even though the actual cart has no PRG RAM, the header must be interpreted as implying it
	.prg_ram_size = 8192 
};

static RomDescriptor const expected_smb3 = {
	.mapper_id = 4,
	.hardwired_vertical_mirroring = false,
	.timing_mode = RD_TIMING_NTSC,
	.console_type = RD_CONSOLE_NES,
	.prg_rom_size = 0x10 * 16384,
	.chr_rom_size = 0x10 * 8192,
	.prg_ram_size = 8192,
	.default_device = RD_STANDARD_CONTROLLERS
};

static bool test_parsing_header(uint8_t const header[16], RomDescriptor const * expected) {
	bool passing = true;
	RomDescriptor test_descriptor;
	int result = init_rom_descriptor(&test_descriptor, header);

	TEST_ASSERT("\tROM parses successfully", result == 0);
	passing &= (result == 0);


	// We can use memcmp because make_rom_descriptor zeroes the
	// descriptor's memory first, so the padding bytes will be zero.
	bool matched = !memcmp(expected, &test_descriptor, sizeof(RomDescriptor));
	TEST_ASSERT("\tRomDescriptor has expected data", matched);
	passing &= matched;

	return passing;
}

int main() {
	bool passing = true;
	printf("Testing \"Donkey Kong\" (1981) header...\n");
	passing &= test_parsing_header(header_dk, &expected_dk);
	printf("Testing \"Super Mario Bros. 3\" (1988) header...\n");
	passing &= test_parsing_header(header_smb3, &expected_smb3);
	return !passing;
}


