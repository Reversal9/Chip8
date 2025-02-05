#ifndef CHIP8_H
#define CHIP8_H

// Dependencies
#include <stdint.h>
#include <string.h>

// Constants
#define MEMORY_SIZE (4096)
#define STACK_SIZE (48)
#define FONTSET_LENGTH (80)

// Chip8 struct
typedef struct {
  uint16_t memory[MEMORY_SIZE]; // 4KB memory from 0x000 to 0xFFF
  uint8_t V[16];                // 16 general purpose 8-bit registers V0 to VF
  uint16_t I;                   // index register, used in memory operations
  uint16_t pc;                  // program counter
  uint16_t opcode;              // opcode placeholder
  uint16_t stack[STACK_SIZE];   // 48 byte stack, handles 24 nested subroutine
                                // calls in theory
  uint8_t sp;                   // stack pointer, manages the call stack and points to the topmost empty index [0, STACK_SIZE - 1]
  uint8_t delay_timer;          // set and read, counts to 0 at 60 Hz
  uint8_t sound_timer;          // read, beeps when non-zero at 60 Hz
} Chip8;

// Chip8 fontset (0 to F) to be preloaded
extern const uint8_t chip8_fontset[FONTSET_LENGTH];

#endif // CHIP8_H
