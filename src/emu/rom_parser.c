#include "moses/rom_parser.h"
#include <string.h>
#include <stdint.h>

#define PRG_ROM_BANK_SIZE 16384
#define PRG_RAM_BANK_SIZE 8192
#define CHR_ROM_BANK_SIZE 8192

static int parse_nes2_header(RomDescriptor * romdesc, const uint8_t header[16]);
static int parse_ines_header(RomDescriptor * romdesc, const uint8_t header[16]);

int init_rom_descriptor(RomDescriptor * romdesc, void const * buffer) {
	uint8_t const * header = buffer;

	// Magic string for iNes and NES 2.0 headers
	if (!(header[0]=='N' && header[1]=='E' && header[2]=='S' && header[3]==0x1a))
		return 1;

	// initialize descriptor to zero
	memset(romdesc, 0, sizeof(RomDescriptor));

	if ((header[7] & 0xC) == 0x8)
		return parse_nes2_header(romdesc, header);
	else
		return parse_ines_header(romdesc, header);
		
	return 1;
}

static int parse_ines_header(RomDescriptor * romdesc, const uint8_t header[16]) {
	romdesc->prg_rom_size = PRG_ROM_BANK_SIZE * header[4];
	romdesc->chr_rom_size = CHR_ROM_BANK_SIZE * header[5];
	romdesc->mapper_id = (header[6] >> 4) | (header[7] & 0xF0);
	romdesc->hardwired_vertical_mirroring = header[6] & 0x1;
	romdesc->uses_alternative_nametable = header[6] & 0x8;
	romdesc->has_persistent_memory = header[6] & 0x2;
	romdesc->has_trainer_data = header[6] & 0x4;
	romdesc->console_type = header[7] & 0x3;
	// iNes headers always give carts 8KiB of PRG RAM, even when they don't have any
	// https://www.nesdev.org/wiki/INES#Flags_8
	romdesc->prg_ram_size = header[8] ?
		PRG_RAM_BANK_SIZE * header[8] : PRG_RAM_BANK_SIZE;
	romdesc->timing_mode = header[9] & 0x1;
	
	
	return 0;
}

static int parse_nes2_header(RomDescriptor * romdesc, const uint8_t header[16]) {
	romdesc->mapper_id = (header[6] >> 4) |
	                     (header[7] & 0xF0) |
	                     ((header[8] & 0xF0) << 4);
	romdesc->submapper_id = header[8] >> 4;
	romdesc->hardwired_vertical_mirroring = header[6] & 0x1;
	romdesc->uses_alternative_nametable = header[6] & 0x8;
	romdesc->has_persistent_memory = header[6] & 0x2;
	romdesc->has_trainer_data = header[6] & 0x4;
	romdesc->console_type = header[7] & 0x3;
	romdesc->timing_mode = header[12] & 0x1;

	// Set PRG ROM size based on the nibble in header[9]
	if ((header[9] & 0xF) == 0xF) {
		unsigned exponent = header[4] >> 2;
		unsigned base = header[4] & 3;
		romdesc->prg_rom_size = (2 << exponent) * (base * 2 + 1); 
	}
	else {
		unsigned multiplier = header[4] | ((header[9] & 0xF) << 8);
		romdesc->prg_rom_size = PRG_ROM_BANK_SIZE * multiplier;
	}

	// Set CHR ROM size based on the nibble in header[9]
	if ((header[9] & 0xF0) == 0xF0) {
		unsigned exponent = header[4] >> 2;
		unsigned base = header[5] & 3;
		romdesc->chr_rom_size = (2 << exponent) * (base * 2 + 1); 
	}
	else {
		unsigned multiplier = header[4] | ((header[9] & 0xF0) << 4);
		romdesc->chr_rom_size = CHR_ROM_BANK_SIZE * multiplier;
	}

	unsigned prg_ram_shiftcount = header[10] & 0xF;
	if (prg_ram_shiftcount)
		romdesc->prg_ram_size = 64 << prg_ram_shiftcount;

	unsigned nvram_shiftcount = header[10] >> 4;
	if (nvram_shiftcount)
		romdesc->nvram_size = 64 << nvram_shiftcount;

	unsigned chr_ram_shiftcount = header[11] & 0xF;
	if (chr_ram_shiftcount)
		romdesc->chr_ram_size = 64 << chr_ram_shiftcount;

	unsigned chr_nvram_shiftcount = header[11] >> 4;
	if (chr_nvram_shiftcount)
		romdesc->chr_nvram_size = 64 << chr_nvram_shiftcount;
	
	romdesc->default_device = header[15] & 0x7F;

	return 0;
}
