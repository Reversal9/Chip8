#include "../include/chip8.h"

// Chip8 fontset that includes characters 0 to F, 5 bytes long each (8 x 5
// pixels)
const uint8_t chip8_fontset[FONTSET_LENGTH] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

/*
 * This function initializes a Chip8 system specified by the first argument. The
 * font is stored as sprites from (0x50 to 0x9F).
 */

void init_chip8(Chip8 *chip8) {
  // Clear out the garbage values and set them to all-bits-zero
  memset(chip8, 0, sizeof(Chip8));

  // Preload the fontset into memory from 0x50 to 0x9F
  for (int i = 0; i < FONTSET_LENGTH; i++) {
    chip8->memory[i + 0x50] = chip8_fontset[i];
  }

  // Initialize stack pointer to 0 (empty stack)
  chip8->sp = 0;

  // graphics
}

void load_rom(Chip8 *chip8, char *rom_path) {
  // read file into memory
  // starting from location 512Kb
  // or 0x200 in hex
}

/* void execute_opcode() { */
/*     // fetch opcode */
/*     opcode = (memory[pc] << 8) + memory[pc + 1]; */
/*     // decode opcode */
/*     switch (opcode & 0xF000) { */
/*         // execute opcode */
/*         case 0x0: */
/*             if (opcode == 0x00E0) */
/*                 clear_display(); */
/*             elseif (opcode == 0x00EE) */
/*                 // return from subroutine */
/*         case 0x1: */
/*         /1* . *1/ */
/*         /1* . *1/ */
/*         /1* . *1/ */
/*         case 0xF: */
/*     } */

/*     // timers */
/*     if (delay_timer > 0) */
/*         --delay_timer; */
/*     if (sound_timer > 0) { */
/*         if (sound_timer == 1) */
/*             play_sound(); */
/*         --sound_timer; */
/*     } */
/* } */
