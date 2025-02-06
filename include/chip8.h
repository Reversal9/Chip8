#ifndef CHIP8_H
#define CHIP8_H

// Dependencies
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Error codes
#define OK (0)
#define FILE_READ_ERR (-1)
#define INVALID_ROM_ERR (-2)

// Constants
#define MEMORY_SIZE (4096)
#define NUM_REGISTERS (16)
#define STACK_SIZE (24) // Allows for n level of nested subroutines
#define FONTSET_LENGTH (80)
#define MAX_WIDTH (64)
#define MAX_HEIGHT (32)
#define TIMER_SPEED (60)  // rate of timer decrement measured in Hz
#define CLOCK_SPEED (600) // rate that instructions are executed in Hz
#define KEYPAD_SIZE (16)  // 16 keys for input map

// Bit manipulation macros
#define HIGH_NIBBLE_BITMASK (0xF000) // retrieves the first 4 MSB
#define LOW_NIBBLE_BITMASK (0x000F) // retrieves the last 4 MSB
#define MEMORY_ADDRESS_BITMASK (0x0FFF) // retrieves 12 LSB
#define REGISTER_X_BITMASK (0x0F00) // retrieves second nibble
#define REGISTER_Y_BITMASK (0x00F0) // retrieves third nibble
#define NN_BITMASK (0x00FF) // retrieves last byte

// Chip8 struct
typedef struct {
  uint8_t memory[MEMORY_SIZE]; // 4KB memory from 0x000 to 0xFFF
  uint8_t V[NUM_REGISTERS];    // 16 general purpose 8-bit registers V0 to VF
  uint16_t I;                  // index register, used in memory operations
  uint16_t pc;                 // program counter
  uint16_t opcode;             // opcode placeholder
  uint16_t stack[STACK_SIZE];  // 48 byte stack, handles 24 nested subroutines
                               // calls in theory
  uint8_t sp; // stack pointer, manages the call stack and points to the topmost
              // empty index [0, STACK_SIZE - 1]
  uint8_t delay_timer; // set and read, counts to 0 at 60 Hz
  uint8_t sound_timer; // read, beeps when non-zero at 60 Hz
  uint8_t gfx[MAX_WIDTH *
              MAX_HEIGHT];     // graphics of chip8 are black (1) and white (0),
                               // screen has MAX_WIDTH by MAX_HEIGHT pixels
  uint8_t keypad[KEYPAD_SIZE]; // stores current state of each key (bitmap)
} Chip8;

// Chip8 fontset (0 to F) to be preloaded
extern const uint8_t chip8_fontset[FONTSET_LENGTH];

// Function prototypes
void init_chip8(Chip8 *chip8);
int load_rom(Chip8 *chip8, char *rom_path);
void execute_opcode(Chip8 *chip8);

#endif // CHIP8_H
