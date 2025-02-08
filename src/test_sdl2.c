#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main() {
  SDL_Window *window = NULL;
  SDL_Surface *screen_surface = NULL; // 2D image

  if (SDL_Init(SDL_INIT_VIDEO) < 0) { // video subsystem
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  } else {
    // Create window
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
      // Get window surface
      screen_surface = SDL_GetWindowSurface(window);

      // Fill the surface white
      SDL_FillRect(screen_surface, NULL,
                   SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

      // Update the surface
      SDL_UpdateWindowSurface(window);

      // Hack to get window to stay up
      SDL_Event e;
      bool quit = false;
      while (quit == false) {
        while (SDL_PollEvent(&e)) {
          if (e.type == SDL_QUIT)
            quit = true;
        }
      }
    }
  }

  // Destroy window
  SDL_DestroyWindow(window);

  // Quit SDL subsystems
  SDL_Quit();

  return 0;
  /* if (SDL_Init(SDL_INIT_VIDEO) < 0) { */
  /*     printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError()); */
  /*     return 1; */
  /* } */

  /* SDL_Window* window = SDL_CreateWindow("SDL2 Window", */
  /*                                       SDL_WINDOWPOS_CENTERED,
   * SDL_WINDOWPOS_CENTERED, */
  /*                                       64, 32, SDL_WINDOW_SHOWN); */
  /* if (!window) { */
  /*     printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
   */
  /*     SDL_Quit(); */
  /*     return 1; */
  /* } */

  /* SDL_Delay(2000); // Keep window open for 2 seconds */

  /* SDL_DestroyWindow(window); */
  /* SDL_Quit(); */
  /* return 0; */
}
