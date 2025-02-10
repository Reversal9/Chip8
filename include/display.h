#ifndef DISPLAY_H
#define DISPLAY_H

// Dependencies
#include "chip8.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

// Function prototypes
void update_display(Chip8 *chip8);
void clear_display(Chip8 *chip8);
void draw_sprite(Chip8 *chip8, int Vx, int Vy, int N);
int init_display(Chip8 *chip8, int width, int height);
void close_display();

// Global variables
extern SDL_Window *g_window;
extern SDL_Surface *g_screen_surface;
extern bool g_is_running;

// Program codes
#define OK (0)
#define SDL_INIT_ERROR (-1)
#define SDL_WINDOW_ERROR (-2)
#define SDL_RENDERER_ERROR (-3)
#define SDL_TEXTURE_ERROR (-4)

// Screen dimensions of Chip-8
#define SCREEN_WIDTH (64)
#define SCREEN_HEIGHT (32)

// Color codes
#define WHITE_PIXEL (0xFF)
#define BLACK_PIXEL (0x00)

// Pixels
#define UNSET (0)
#define SET (1)

#endif // DISPLAY_H
