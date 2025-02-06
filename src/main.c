#include "../include/main.h"
#include "../include/chip8.h"
#include "../include/input.h"
#include "../include/display.h"

int main(int argc, char *argv[]) {
  Chip8 chip8 = {0};
  int system_is_running = 1;

  init_chip8(&chip8);
  /* init_input(); */
  /* init_graphics(); */

  load_rom(&chip8, ROM_PATH);

  while (system_is_running) {
    // Execute an instruction
    /* execute_opcode(); */

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

  return OK;
}
