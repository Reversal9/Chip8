#include "../include/chip8.h"

void init_chip8(void) {
    // initialize system
    // memory
    // registers
    // stack
    // graphics
}

void load_rom(char *filename) {
    // read file into memory
    // starting from location 512Kb
    // or 0x200 in hex
}

void execute_opcode() {
    // fetch opcode
    opcode = (memory[pc] << 8) + memory[pc + 1];
    // decode opcode
    switch (opcode & 0xF000) {
        // execute opcode
        case 0x0:
            if (opcode == 0x00E0)
                clear_display();
            elseif (opcode == 0x00EE)
                // return from subroutine
        case 0x1:
        /* . */
        /* . */
        /* . */
        case 0xF:
    }

    // timers
    if (delay_timer > 0)
        --delay_timer;
    if (sound_timer > 0) {
        if (sound_timer == 1)
            play_sound();
        --sound_timer;
    }
}

