#include "../include/main.h"
#include "../include/chip8.h"
#include "../include/display.h"
#include "../include/input.h"

int main(int argc, char *argv[]) {
  Chip8 chip8 = {0};

  init_chip8(&chip8);
  /* init_input(); */
  init_display(&chip8, MAX_WIDTH, MAX_HEIGHT);

  load_rom(&chip8, ROM_PATH);

  // Hack to get window to stay up

  SDL_Event e;
  while (g_is_running) {
    // Keep window open until user closes it

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        g_is_running = false;
      }
    }

    // Execute an instruction

    execute_opcode(&chip8);

    // Refresh display if flag is set
    /* if (draw_flag) { */
    /*   draw_graphics(); */
    /* } */

    // Play beep if flag is set
    /* if (play_sound) { */
    /*   play_beep(); */
    /* } */

    // Set input and set key states
    /* set_input(); */
  }

  // Clean up and exit

  close_display();

  return OK;
}
