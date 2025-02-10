#include "../include/main.h"
#include "../include/chip8.h"
#include "../include/display.h"
#include "../include/input.h"

int main(int argc, char *argv[]) {
  Chip8 chip8 = {0};

  init_chip8(&chip8);
  init_input();
  init_display(&chip8, MAX_WIDTH, MAX_HEIGHT);

  if (load_rom(&chip8, ROM_PATH) == INVALID_ROM_ERR) {
    fprintf(stderr, "Error: Could not load ROM\n");
    return INVALID_ROM_ERR;
  }

  // Hack to get window to stay up
  // Set input and set key states

  SDL_Event e;
  while (g_is_running) {
    // Keep window open until user closes it

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        g_is_running = false;
      } else if ((e.type == SDL_KEYDOWN) || (e.type == SDL_KEYUP)) {
        // Key is either pressed down or released

        int chip8_key = map_sdl_key(e.key.keysym.sym);

        if (chip8_key != INVALID_KEY) {
          // Set the state according to whether the key is pressed down or
          // released

          g_chip8_keys[chip8_key] = (e.type == SDL_KEYDOWN);
        }
      }
    }

    // Execute an instruction

    execute_opcode(&chip8);

    // Refresh display if flag is set
    if (g_draw_flag) {
      update_display(&chip8);
    }

    // Play beep if flag is set
    /* if (play_sound) { */
    /*   play_beep(); */
    /* } */
  }

  // Clean up and exit

  close_display();

  return OK;
}
