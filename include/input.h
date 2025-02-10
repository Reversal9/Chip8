#ifndef INPUT_H
#define INPUT_H

// Dependencies
#include <SDL2/SDL.h>

#include <stdbool.h>

// Function prototypes
void init_input();
int map_sdl_key(SDL_Keycode key);

// Constants
#define CHIP8_KEY_COUNT (16)
#define INVALID_KEY (-1)

// Array storing state of each key
extern bool g_chip8_keys[CHIP8_KEY_COUNT];

#endif // INPUT_H
