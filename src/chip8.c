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

  // Set program counter to 0x200 (start of ROM)

  chip8->pc = 0x200;

  // Set delay_timer and sound_timer to 0

  chip8->delay_timer = 0;
  chip8->sound_timer = 0;
}

int load_rom(Chip8 *chip8, char *rom_path) {
  // Read binary rom file

  FILE *fp = fopen(rom_path, "rb");

  // Check fp for NULL when unable to read file

  if (fp == NULL) {
    fprintf(stderr, "Error: Unable to read file %s\n", rom_path);
  }

  // Get each byte inside the rom file and store it in memory starting from
  // 0x200

  uint8_t byte = 0;
  int i = 0x200;

  // Read one byte each time (char) and place it in memory

  while ((byte = fscanf(fp, "%c", &byte)) == 1) {
    // Check for memory overflow, when ROM is formatted incorrectly (too large)

    if (i >= MEMORY_SIZE) {
      fprintf(stderr, "Error: ROM too large to fit in memory\n");
      fclose(fp);
      fp = NULL;

      return INVALID_ROM_ERR;
    }

    chip8->memory[i++] = byte;
  }

  // Close file stream

  fclose(fp);
  fp = NULL;

  return OK;
}

void execute_opcode(Chip8 *chip8) {
  // Get the opcode from memory by combining the current (right padding by 8
  // bits) and next byte relative to the PC

  chip8->opcode =
      (chip8->memory[chip8->pc] << 8) + chip8->memory[chip8->pc + 1];

  // Decode the opcode and execute the instruction

  switch (HN(chip8->opcode)) {
  case 0x0:
    if (chip8->opcode == 0x00E0) { // 00E0: clears the screen
      /* clear_display(); */
      chip8->pc += 2;
    } else if (chip8->opcode == 0x00EE) { // 00EE: return from subroutine
      /* chip8->pc = chip8->pop(chip8->stack); */
    }
    break;

  case 0x1: // 1NNN: jump to address NNN
    chip8->pc = NNN(chip8->opcode);
    break;

  case 0x2: // 2NNN: calls subroutine at NNN
    /* push(&(chip8->stack), chip8->pc); */
    chip8->pc = NNN(chip8->opcode);
    break;

  case 0x3: // 3XNN: skips next instruction if Vx == NN
    if (chip8->V[Vx(chip8->opcode)] ==
        NN(chip8->opcode)) {
      chip8->pc += 4;
    } else {
      chip8->pc += 2;
    }
    break;

  case 0x4: // 4XNN: skips next instruction if Vx != NN
    if (chip8->V[Vx(chip8->opcode)] !=
        NN(chip8->opcode)) {
      chip8->pc += 4;
    } else {
      chip8->pc += 2;
    }
    break;

  case 0x5: // 5XY0: skips next instruction if Vx == Vy
    if (chip8->V[Vx(chip8->opcode)] ==
        chip8->V[Vy(chip8->opcode)]) {
      chip8->pc += 4;
    } else {
      chip8->pc += 2;
    }
    break;

  case 0x6: // 6XNN: sets Vx to NN
    chip8->V[Vx(chip8->opcode)] = NN(chip8->opcode);
    chip8->pc += 2;
    break;

  case 0x7: // 7XNN: adds NN to Vx
    chip8->V[Vx(chip8->opcode)] += NN(chip8->opcode);
    break;

  case 0x8:
    if (LN(chip8->opcode) == 0) { // 8XY0: sets Vx = Vy
      chip8->V[Vx(chip8->opcode)] =
          chip8->V[Vy(chip8->opcode)];
    } else if (LN(chip8->opcode) == 1) { // 8XY1: sets Vx |= Vy

    } else if (LN(chip8->opcode) == 2) { // 8XY2: sets Vx &= Vy
    } else if (LN(chip8->opcode) == 3) { // 8XY3: sets Vx ^= Vy
    } else if (LN(chip8->opcode) == 4) { // 8XY4: sets Vx += Vy
    } else if (LN(chip8->opcode) == 5) {
    }
    break;

    // timers
    /* if (delay_timer > 0) */
    /*   --delay_timer; */
    /* if (sound_timer > 0) { */
    /*   if (sound_timer == 1) */
    /*     play_sound(); */
    /*   --sound_timer; */
    /* } */
  }
}
