#include "../include/display.h"
#include "../include/chip8.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>
#include <stdio.h>

// Global variables for the display

SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Texture *g_texture = NULL;
bool g_is_running = true;

// Variable for the draw flag set by CHIP-8

bool g_draw_flag = false;

// Global variables for the audio

SDL_AudioDeviceID g_audio_device = 0;
SDL_AudioSpec g_audio_spec = {0};

void update_display(Chip8 *chip8) {
  if (g_window == NULL) {
    return;
  }

  int texture_pitch = 0;
  void *texture_pixels = NULL;

  if (SDL_LockTexture(g_texture, NULL, &texture_pixels, &texture_pitch) != 0) {
    printf("Unable to lock texture: %s", SDL_GetError());
    return;
  }

  memcpy(texture_pixels, chip8->gfx, texture_pitch * SCREEN_HEIGHT);

  SDL_UnlockTexture(g_texture);
  SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
  SDL_RenderPresent(g_renderer);
}

void clear_display(Chip8 *chip8) {
  if (g_window == NULL) {
    return;
  }

  for (int r = 0; r < SCREEN_HEIGHT; r++) {
    for (int c = 0; c < SCREEN_WIDTH; c++) {
      chip8->gfx[r][c] = WHITE_PIXEL;
    }
  }
}

void draw_sprite(Chip8 *chip8, int Vx, int Vy, int N) {
  if (g_window == NULL) {
    return;
  }

  bool has_flipped = false;

  // TODO
  for (int i = 0; i < N; i++) {
    // Read a byte from memory

    uint8_t byte = chip8->memory[chip8->I + i]; // each byte has 8 bits

    for (int j = 0; j < SPRITE_WIDTH; j++) {
      // Get position of the pixel to draw

      int x = Vx + i;
      int y = Vy + j;

      // Get the pixel value

      int pixel = (byte >> (SPRITE_WIDTH - 1 - i)) & 0x1;
      int current_pixel = chip8->gfx[x][y];

      // XOR the pixel values

      chip8->gfx[x][y] ^= pixel;

      // Check if pixel was flipped from SET to UNSET

      if ((current_pixel == SET) && (chip8->gfx[x][y] == UNSET)) {
        has_flipped = true;
      }
    }

    if (has_flipped) {
      chip8->V[0xF] = 1;
    } else {
      chip8->V[0xF] = 0;
    }
  }
}

int init_display(Chip8 *chip8, int width, int height) {

  // Initialize SDL video subsystem

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return SDL_INIT_ERROR;
  }

  // Create the window to display the CHIP-8 screen

  g_window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

  if (g_window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return SDL_WINDOW_ERROR;
  }

  // Create the renderer for the window

  g_renderer = SDL_CreateRenderer(
      g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (g_renderer == NULL) {
    printf("Unable to create renderer: %s", SDL_GetError());
    return SDL_RENDERER_ERROR;
  }

  // Create the texture for the renderer

  g_texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32,
                                SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                SCREEN_HEIGHT);

  if (g_texture == NULL) {
    printf("Unable to create texture: %s", SDL_GetError());
    return SDL_TEXTURE_ERROR;
  }

  // Fill the surface white

  clear_display(chip8);

  // Update the surface

  update_display(chip8);

  return OK;
}

void close_display() {
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}

void audio_callback(void *userdata, Uint8 *stream, int len) {
  static int phase = 0;
  int16_t *buffer = (int16_t *)stream;
  int samples = len / 2;

  for (int i = 0; i < samples; i++) {
    buffer[i] =
        (phase < SAMPLE_RATE / SOUND_FREQUENCY / 2) ? AMPLITUDE : -AMPLITUDE;
    phase = (phase + 1) % (SAMPLE_RATE / SOUND_FREQUENCY);
  }
}

int init_audio() {
  SDL_AudioSpec desired_spec = {0};
  SDL_memset(&desired_spec, 0, sizeof(desired_spec));

  desired_spec.freq = SAMPLE_RATE;
  desired_spec.format = AUDIO_S16SYS;
  desired_spec.channels = 1;
  desired_spec.samples = 1024;
  desired_spec.callback = audio_callback;

  g_audio_device =
      SDL_OpenAudioDevice(NULL, 0, &desired_spec, &g_audio_spec, 0);
  if (g_audio_device != 0) {
    printf("Failed to open audio device: %s\n", SDL_GetError());
    return SDL_AUDIO_ERROR;
  }

  return OK;
}
