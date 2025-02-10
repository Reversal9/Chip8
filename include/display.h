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
void audio_callback(void *userdata, Uint8 *stream, int len);
int init_audio();

// Global variables
extern SDL_Window *g_window;
extern SDL_Surface *g_screen_surface;
extern bool g_is_running;

extern bool g_draw_flag;

extern SDL_AudioDeviceID g_audio_device;
extern SDL_AudioSpec g_audio_spec;

// Program codes
#define OK (0)
#define SDL_INIT_ERROR (-1)
#define SDL_WINDOW_ERROR (-2)
#define SDL_RENDERER_ERROR (-3)
#define SDL_TEXTURE_ERROR (-4)
#define SDL_AUDIO_ERROR (-5)

// Screen dimensions of Chip-8
#define SCALE_FACTOR (10)

// Pixels
#define UNSET (0)
#define SET (1)

// Audio
#define SOUND_FREQUENCY (440) // in Hz (A4 tone)
#define SAMPLE_RATE (44100) // sample rate (CD quality)
#define AMPLITUDE (28000) // volume

#endif // DISPLAY_H
