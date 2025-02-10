#include "../include/chip8.h"
#include "../include/display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  while (fscanf(fp, "%c", &byte) == 1) {
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

  /* printf("Executing opcode: %X\n", chip8->opcode); */

  // Decode the opcode and execute the instruction

  switch (HN(chip8->opcode)) {
  case 0x0:
    if (chip8->opcode == 0x00E0) { // 00E0: clears the screen
      clear_display(chip8);
      chip8->pc += 2;
    } else if (chip8->opcode == 0x00EE) { // 00EE: return from subroutine
      chip8->pc = chip8->stack[--(chip8->sp)];
    }
    break;

  case 0x1: // 1NNN: jump to address NNN
    chip8->pc = NNN(chip8->opcode);
    break;

  case 0x2: // 2NNN: calls subroutine at NNN
    chip8->stack[(chip8->sp)++] = chip8->pc;
    chip8->pc = NNN(chip8->opcode);
    break;

  case 0x3: // 3XNN: skips next instruction if Vx == NN
    if (chip8->V[Vx(chip8->opcode)] == NN(chip8->opcode)) {
      chip8->pc += 4;
    } else {
      chip8->pc += 2;
    }
    break;

  case 0x4: // 4XNN: skips next instruction if Vx != NN
    if (chip8->V[Vx(chip8->opcode)] != NN(chip8->opcode)) {
      chip8->pc += 4;
    } else {
      chip8->pc += 2;
    }
    break;

  case 0x5: // 5XY0: skips next instruction if Vx == Vy
    if (chip8->V[Vx(chip8->opcode)] == chip8->V[Vy(chip8->opcode)]) {
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
    chip8->pc += 2;
    break;

  case 0x8:
    if (LN(chip8->opcode) == 0x0) { // 8XY0: sets Vx = Vy
      chip8->V[Vx(chip8->opcode)] = chip8->V[Vy(chip8->opcode)];
    } else if (LN(chip8->opcode) == 0x1) { // 8XY1: sets Vx |= Vy
      chip8->V[Vx(chip8->opcode)] |= chip8->V[Vy(chip8->opcode)];
    } else if (LN(chip8->opcode) == 0x2) { // 8XY2: sets Vx &= Vy
      chip8->V[Vx(chip8->opcode)] &= chip8->V[Vy(chip8->opcode)];
    } else if (LN(chip8->opcode) == 0x3) { // 8XY3: sets Vx ^= Vy
      chip8->V[Vx(chip8->opcode)] ^= chip8->V[Vy(chip8->opcode)];
    } else if (LN(chip8->opcode) == 0x4) { // 8XY4: sets Vx += Vy, VF = 1 if OF
      chip8->V[Vx(chip8->opcode)] += chip8->V[Vy(chip8->opcode)];
      chip8->V[0xF] =
          (chip8->V[Vy(chip8->opcode)] > chip8->V[Vx(chip8->opcode)]) ? 1 : 0;
    } else if (LN(chip8->opcode) == 0x5) { // 8XY5: sets Vx -= Vy, VF = 0 if UF
      chip8->V[Vx(chip8->opcode)] -= chip8->V[Vy(chip8->opcode)];
      chip8->V[0xF] =
          (chip8->V[Vx(chip8->opcode)] >= chip8->V[Vy(chip8->opcode)]) ? 1 : 0;
    } else if (LN(chip8->opcode) ==
               0x6) { // 8XY6: VF = LSB, right shifts Vx by 1
      chip8->V[0xF] = LN(chip8->V[Vx(chip8->opcode)]);
      chip8->V[Vx(chip8->opcode)] >>= 1;
    } else if (LN(chip8->opcode) == 0x7) { // 8XY7: Vx = Vy - Vx, VF = 0 if UF
      chip8->V[0xF] =
          (chip8->V[Vy(chip8->opcode)] >= chip8->V[Vx(chip8->opcode)]) ? 1 : 0;
      chip8->V[Vx(chip8->opcode)] =
          chip8->V[Vy(chip8->opcode)] - chip8->V[Vx(chip8->opcode)];
    } else if (LN(chip8->opcode) ==
               0xE) { // 8XYE: VF = MSB, left shifts Vx by 1
      chip8->V[0xF] = Vy(chip8->V[Vx(chip8->opcode)]);
      chip8->V[Vx(chip8->opcode)] <<= 1;
    }
    chip8->pc += 2;
    break;

  case 0x9: // 9XY0: skips next instruction if Vx != Vy
    if (chip8->V[Vx(chip8->opcode)] != chip8->V[Vy(chip8->opcode)]) {
      chip8->pc += 4;
    } else {
      chip8->pc += 2;
    }
    break;

  case 0xA: // ANNN: sets I to NNN
    chip8->I = NNN(chip8->opcode);
    chip8->pc += 2;
    break;

  case 0xB: // BNNN: jumps to address NNN + V0
    chip8->pc = NNN(chip8->opcode) + chip8->V[0x0];
    break;

  case 0xC: // CXNN: sets Vx = rand(0, 255) & NN
    chip8->V[Vx(chip8->opcode)] = (rand() % 255 + 1) & NN(chip8->opcode);
    chip8->pc += 2;
    break;

  case 0xD: // DXYN: draw sprite at (Vx, Vy), w = 8, h = n
    draw_sprite(chip8, chip8->V[Vx(chip8->opcode)], chip8->V[Vy(chip8->opcode)],
                LN(chip8->opcode));
    chip8->pc += 2;
    break;

  case 0xE:
    if (NN(chip8->opcode) == 0x9E) { // EX9E: skips next instruction if key[Vx]
      if (chip8->keypad[chip8->V[Vx(chip8->opcode)]]) {
        chip8->pc += 4;
      } else {
        chip8->pc += 2;
      }
    } else if (NN(chip8->opcode) ==
               0xA1) { // EXA1: skips next instruction if !key[Vx]
      if (!chip8->keypad[chip8->V[Vx(chip8->opcode)]]) {
        chip8->pc += 4;
      } else {
        chip8->pc += 2;
      }
    }
    break;

  case 0xF:
    if (NN(chip8->opcode) == 0x07) { // FX07: sets Vx = delay_timer
      chip8->V[Vx(chip8->opcode)] = chip8->delay_timer;
    } else if (NN(chip8->opcode) ==
               0x0A) { // FX0A: waits for key press and stores in Vx
      bool key_pressed = false;

      // Go through each key state

      for (int i = 0; i < KEYPAD_SIZE; i++) {
        if (chip8->keypad[i]) {
          chip8->V[Vx(chip8->opcode)] = i;
          key_pressed = true;
        }
      }

      // Do not increment program counter (blocking operation)

      if (!key_pressed) {
        return;
      }

    } else if (NN(chip8->opcode) == 0x15) { // FX15: sets delay_timer = Vx
      chip8->delay_timer = chip8->V[Vx(chip8->opcode)];
    } else if (NN(chip8->opcode) == 0x18) { // FX18: sets sound_timer = Vx
      chip8->sound_timer = chip8->V[Vx(chip8->opcode)];
    } else if (NN(chip8->opcode) ==
               0x1E) { // FX1E: sets I = I + Vx, VF = 1 if overflow (dependent)
      chip8->I += chip8->V[Vx(chip8->opcode)];
      /* chip8->V[0xF] = (chip8->I + chip8->V[Vx(chip8->opcode)] > 0xFFF) ? 1 :
       * 0; */
    } else if (NN(chip8->opcode) ==
               0x29) { // FX29: sets I to location of sprite for digit Vx
      chip8->I = chip8->V[Vx(chip8->opcode) + 0x50];
    } else if (NN(chip8->opcode) ==
               0x33) { // FX33: stores BCD representation of Vx in memory
      chip8->memory[chip8->I] = chip8->V[Vx(chip8->opcode)] / 100;
      chip8->memory[chip8->I + 1] = (chip8->V[Vx(chip8->opcode)] / 10) % 10;
      chip8->memory[chip8->I + 2] = (chip8->V[Vx(chip8->opcode)] / 100) % 10;
    } else if (NN(chip8->opcode) == 0x55) { // FX55: stores V0 to Vx in memory
      for (int i = 0x0; i <= Vx(chip8->opcode); i++) {
        chip8->memory[chip8->I + i] = chip8->V[i];
      }
    } else if (NN(chip8->opcode) == 0x65) { // FX66: fills v0 to Vx from memory
      for (int i = 0x0; i <= Vx(chip8->opcode); i++) {
        chip8->V[i] = chip8->memory[chip8->I + i];
      }
    }
    chip8->pc += 2;
    break;

  default:
    fprintf(stderr, "Error: Unknown opcode %X\n", chip8->opcode);
    chip8->pc += 2;
    break;

    // timers
    if (chip8->delay_timer > 0) {
      --chip8->delay_timer;
    }
    if (chip8->sound_timer > 0) {
      if (chip8->sound_timer == 1) {
        SDL_PauseAudioDevice(g_audio_device, 0);
        --chip8->sound_timer;
      } else {
        SDL_PauseAudioDevice(g_audio_device, 1);
      }
    }
  }
}
