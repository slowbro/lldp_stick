#include "game/snake.h"
#include <stdint.h>
#include "button.h"
#include "display.h"

uint16_t snake[2048];
uint16_t head;
uint16_t pip;
uint16_t score;
uint8_t direction;
uint32_t last_tick;
bool running;

void game_snake_init(){
    // clear the snake
    memset(snake, '\0', 2048*sizeof(uint8_t));

    // start the snake in the middle of the screen, heading up
    head = 15*64+32;
    snake[0] = head;
    score = 1;
    direction = 1;

    // place the pip in a random location that is not coincident with the start position
    randomSeed(analogRead(PIN_VDIV));
    do {
        pip = random(0, 2047);
    } while(pip == head);

    // draw the initial field
    game_snake_draw();
}

void game_snake_run(){
    running = true;
    game_snake_init();
    while(running){
        game_snake_tick();
    }
}

void game_snake_tick(){
    // only redraw/recalc every 100ms
    if(millis()-last_tick > 100){
        // move
        // 0 = up = pos-64
        // 1 = right pos+1 kinda
        // 2 = down = pos+64
        // 3 = left = pos-1 kinda
        switch(direction){
            case 0: // up
                if(head < 64){
                    // wrap
                    head = 2048-(64-head);
                } else {
                    head -= 64;
                }
                break;

            case 1: //right
                if(head%64 == 63){
                    //wrap
                    head -= 63;
                }
                ++head;
                break;

            case 2: // down
                if(head > 1983){
                    //wrap
                    head = (64+head)-2048;
                } else {
                    head += 64;
                }
                break;

            case 3: // left
                if(head%64 == 0 && head != 0){
                    //wrap
                    head += 63;
                } else if(head == 0){
                    head = 64;
                }
                --head;
                break;
        }

        // check if head location is at pip, if so increase score
        // then re-place the pip
        if(head == pip){
            score++;
            do {
                pip = random(0, 2047);
                // don't place the pip where the head is
                if(pip == head)
                    continue;

                // don't place the pip inside the snake
                bool ok = true;
                for(uint16_t i=0;i<2048;i++){
                    if(snake[i] == pip){
                        ok = false;
                        break;
                    }
                    // we reached the end of the snake without collision, use this spot
                    if(snake[i] == '\0')
                        break;
                }
                if(ok)
                    break;
            } while(1==1);
        }

        // recalculate the snake (i.e. 'move' it on screen)
        if(score>1){
            for(uint16_t i = score-1;i>0;i--){
                snake[i] = snake[i-1];
            }
        }
        snake[0] = head;

        // look for collisions
        for(uint16_t i = score-1;i>0;i--){
            // head collided with a part of the body, game over
            if(head == snake[i]){
                game_snake_gameover();
            }
        }

        game_snake_draw();
        last_tick = millis();
    }

    // respond to input
    button_read();
    if(button_lbtn_pressed()){
        if(direction == 0){
            direction = 3;
        } else {
            direction--;
        }
    }
    if(button_rbtn_pressed()){
        if(direction == 3){
            direction = 0;
        } else {
            direction++;
        }
    }

    // a little delay so we don't slam the cpu
    delay(10);
}

void game_snake_draw(){
    display.clearDisplay();
    // place pip
    display.fillRect((pip%64)*2, (pip/64)*2, 2, 2, WHITE);

    // place snake
    for(uint16_t i=0;i<2048;i++){
        if(snake[i] == '\0')
            break;
        display.fillRect((snake[i]%64)*2, (snake[i]/64)*2, 2, 2, WHITE);
    }
    display.display();
}

void game_snake_gameover(){
    // draw the field as-is behind the 'popup'
    game_snake_draw();

    // draw a little popup window and allow the player to re-play or exit
    display.fillRect(16, 8, 96, 48, BLACK);
    display.drawRect(16, 8, 96, 48, WHITE);
    display.setTextSize(1);
    display.setCursor(38, 16);
    display.print(F("Game Over"));
    display.setCursor(32, 24);
    display.print(F("Score: "));
    display.print(score);
    display.setCursor(18, 46);
    display.print(F("< Again"));
    display.setCursor(74, 46);
    display.print(F("Exit >"));
    display.display();

    while(1==1){
        // respond to input
        button_read();
        if(button_lbtn_pressed()){
            // reset the game
            game_snake_init();
            break;
        }
        if(button_rbtn_pressed()){
            // exit the game
            running = false;
            break;
        }
        delay(10);
    }
}
