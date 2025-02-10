#include "../include/input.h"

// Array that stores the state of each key (pressed = true)

bool g_chip8_keys[CHIP8_KEY_COUNT] = {0};

/*
 * This function maps keyboard input to the chip8 keyboard, returning the index
 * of the key pressed.
 */

void init_input() {
  memset(g_chip8_keys, 0, sizeof(g_chip8_keys));
}

int map_sdl_key(SDL_Keycode key) {
  switch (key) {
  case SDLK_1:
    return 0x1;
  case SDLK_2:
    return 0x2;
  case SDLK_3:
    return 0x3;
  case SDLK_4:
    return 0xC;
  case SDLK_q:
    return 0x4;
  case SDLK_w:
    return 0x5;
  case SDLK_e:
    return 0x6;
  case SDLK_r:
    return 0xD;
  case SDLK_a:
    return 0x7;
  case SDLK_s:
    return 0x8;
  case SDLK_d:
    return 0x9;
  case SDLK_f:
    return 0xE;
  case SDLK_z:
    return 0xA;
  case SDLK_x:
    return 0x0;
  case SDLK_c:
    return 0xB;
  case SDLK_v:
    return 0xF;
  default:
    return INVALID_KEY;
  }
}
