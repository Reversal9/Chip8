#include "../include/display.h"
#include "../include/chip8.h"

#include <SDL2/SDL.h>
#include <stdio.h>

SDL_Window *g_window = NULL;
SDL_Surface *g_screen_surface = NULL;
bool g_is_running = true;

void update_display() {
  if (g_window == NULL) {
    return;
  }

  SDL_UpdateWindowSurface(g_window);
}

void clear_display() {
  if (g_window == NULL) {
    return;
  }

  SDL_FillRect(g_screen_surface, NULL,
               SDL_MapRGB(g_screen_surface->format, 0xFF, 0xFF, 0xFF));
}

void draw_sprite(Chip8 *chip8, int Vx, int Vy, int N) {
  if (g_window == NULL) {
    return;
  }

  // TODO
}

int init_display(int width, int height) {

  // Initialize SDL video subsystem

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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

  // Get the window surface

  g_screen_surface = SDL_GetWindowSurface(g_window);

  // Fill the surface white

  clear_display();

  // Update the surface

  update_display();

  // Hack to get window to stay up

  SDL_Event e;
  while (g_is_running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        g_is_running = false;
      }
    }
  }

  return OK;
}

void close_display() {
  SDL_DestroyWindow(g_window);
  SDL_Quit();
}
