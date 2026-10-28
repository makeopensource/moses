#pragma once

/*
Interface for parsing an iNES or NES 2.0 format rom into a data structure.
*/

#include <stddef.h>
#include <stdint.h>

enum RDTimingMode {
	RD_TIMING_NTSC,
	RD_TIMING_PAL,
	RD_TIMING_MULTIREGION,
	RD_TIMING_DENDY,
};

enum RDConsoleType {
	RD_CONSOLE_NES,
	RD_CONSOLE_VSSYSTEM,
	RD_CONSOLE_PLAYCHOICE,
	RD_CONSOLE_EXTENDED
};

// Indicates the specific devices that the cart expects in the controller slot in order to function.
enum RDExpansionDevice {
	RD_UNSPECIFIED,
	RD_STANDARD_CONTROLLERS
};

typedef struct RomDescriptor {
	/*	Not all features enumerated in the iNES/NES 2.0 header are expected to be
		supported, and some may be deliberately unsupported.
	*/

	uint32_t prg_rom_size;
	uint32_t chr_rom_size;
	uint32_t prg_ram_size;
	uint32_t nvram_size;
	uint32_t chr_ram_size;
	uint32_t chr_nvram_size;

	enum RDTimingMode timing_mode;
	enum RDConsoleType console_type;
	enum RDExpansionDevice default_device;

	uint16_t mapper_id;
	uint8_t submapper_id;


	bool hardwired_vertical_mirroring;
	bool uses_alternative_nametable;
	bool has_persistent_memory;
	bool has_trainer_data;

} RomDescriptor;

// Initialize a RomDescriptor from a buffer holding raw NES rom data.
// Returns 0 on success, non-zero on error.
int init_rom_descriptor(RomDescriptor * romdesc, void const * buffer);


