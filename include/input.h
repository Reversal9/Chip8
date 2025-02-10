#ifndef INPUT_H
#define INPUT_H

// Dependencies
#include "chip8.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

// Function prototypes
void init_input(Chip8 *chip8);
int map_sdl_key(SDL_Keycode key);

// Constants
#define INVALID_KEY (-1)

#endif // INPUT_H
