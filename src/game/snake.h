#ifndef GAME_SNAKE_H
#define GAME_SNAKE_H
#include <Arduino.h>
#include <stdint.h>
#include "button.h"
#include "display.h"

void game_snake_init();
void game_snake_run();
void game_snake_draw();
void game_snake_tick();
void game_snake_gameover();

#endif
