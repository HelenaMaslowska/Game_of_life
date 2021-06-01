#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <sys/time.h>

#include "common.h"
#include "game.h"
#include "graphics.h"

bool quit = false;
static int current_scene = WELCOME_SCENE;

SDL_Renderer *renderer;
int speed = 400;
float start_time = 0, end_time = 0;

void draw_every_scene();
void draw_buttons();

/*  https://gigi.nullneuron.net/gigilabs/displaying-an-image-in-an-sdl2-window/  */
void main_button_function(void *scene, void *button, SDL_Event *event)
{
    char filename[20];
    start_scene_t *scene_pressed = scene;
    Button *button_pressed=button;
    bool clear_event = false;
    //printf("id sceny: %d id przycisku: %d\n", scene_pressed->id, button_pressed->id);
    switch(scene_pressed->id)
    {
        case WELCOME_SCENE:
            switch(button_pressed->id)
            {
                case START:
                    current_scene = GAME_SCENE;
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    clear_event = true;
                    break;

                case EXAMPLE:
                    if(game_read_from_file("saves/input0.txt"))
                    {
                        current_scene = GAME_SCENE;
                        draw_every_scene();
                        draw_buttons();
                        SDL_RenderPresent(renderer);
                    }
                    clear_event = true;
                    break;

                case LOAD:
                    current_scene = LOADING_SCENE;
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    clear_event = true;
                    break;

                case EXIT:
                    quit = true;
                    break;
            }
            break;
        case GAME_SCENE:
            switch(button_pressed->id)
            {
                case G_SAVE:
                    current_scene = SAVINGS_SCENE;
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    break;

                case G_FAST:
                    if(speed < 9500)
                    {
                        if(speed >= 1000)   speed = speed + 500;
                        else                speed = speed + 100;
                    }
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    break;

                case G_SLOW:
                    if(speed > 100)
                    {
                        if(speed > 1400)    speed = speed - 500;
                        else                speed = speed - 100;
                    }
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    break;

                case G_CLEAN:
                    clean_board();
                    draw_every_scene();
                    draw_buttons();
                    break;

                case G_BACK:
                    current_scene = WELCOME_SCENE;
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    break;
            }
            break;

        case SAVINGS_SCENE:
        case LOADING_SCENE:
            switch(button_pressed->id)
            {
                case S_SLOT1:
                case S_SLOT2:
                case S_SLOT3:
                case S_SLOT4:
                    snprintf(filename, sizeof(filename),"saves/input%d.txt", button_pressed->id-S_SLOT1+1);
                    if(scene_pressed->id == LOADING_SCENE)
                    {
                        if (game_read_from_file(filename))
                        {
                            graphics_load_image_from_file(renderer, "images/check.bmp", button_pressed->x-80, button_pressed->y, 60, 60);
                        }
                        else
                        {
                            graphics_load_image_from_file(renderer, "images/notcheck.bmp", button_pressed->x-80, button_pressed->y, 60, 60);
                        }

                        current_scene = GAME_SCENE;
                    }

                    if(scene_pressed->id == SAVINGS_SCENE)
                    {
                        game_save_to_file(filename);
                        graphics_load_image_from_file(renderer, "images/check.bmp", button_pressed->x-80, button_pressed->y, 60, 60);
                        current_scene = GAME_SCENE;
                    }
                    SDL_RenderPresent(renderer);
                    Sleep(800);
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    clear_event = true;
                    break;

                case S_BACK:
                    current_scene = WELCOME_SCENE;
                    draw_every_scene();
                    draw_buttons();
                    SDL_RenderPresent(renderer);
                    break;
            }
    }
    if(clear_event)
        memset(event, 0, sizeof(SDL_Event)); //wype³nia pamiêc = event zerami
}

Button Buttons_game[]=
{
    [G_START]=
    {
        .id = G_START,
        .name = " Start ",
        .name_pressed = " Stop ",
        .x = WIDTH_START_OPTIONS,
        .y = HEIGHT_START_RECT,
        .width = GAME_BUTTON_WIDTH,
        .height = GAME_BUTTON_HEIGHT,
        .bf_function = main_button_function,
    },
    [G_CLEAN]=
    {
        .id = G_CLEAN,
        .name="Clean",
        .x=WIDTH_START_OPTIONS,
        .y=HEIGHT_START_RECT + (SQUARE+SPACE)*4,
        .width=GAME_BUTTON_WIDTH,
        .height=GAME_BUTTON_HEIGHT,
        .bf_function = main_button_function,
    },
    [G_FAST]=
    {
        .id = G_FAST,
        .name = " >> ",
        .x = WIDTH_START_OPTIONS + SQUARE +(GAME_BUTTON_WIDTH-SQUARE)/2,
        .y = HEIGHT_START_RECT+(SQUARE+SPACE)*10,
        .width = (GAME_BUTTON_WIDTH-SQUARE)/2,
        .height = GAME_BUTTON_HEIGHT - SQUARE-SPACE,
        .bf_function = main_button_function,
    },
    [G_SLOW]=
    {
        .id = G_SLOW,
        .name = " << ",
        .x = WIDTH_START_OPTIONS,
        .y = HEIGHT_START_RECT+(SQUARE+SPACE)*10,
        .width = (GAME_BUTTON_WIDTH-SQUARE)/2,
        .height = GAME_BUTTON_HEIGHT - SQUARE-SPACE,
        .bf_function = main_button_function,
    },
    [G_BACK]=
    {
        .id = G_BACK,
        .name = "   Back  ",
        .x = WIDTH_START_OPTIONS,
        .y = HEIGHT_START_RECT+(SQUARE+SPACE)*16,
        .width = GAME_BUTTON_WIDTH,
        .height = GAME_BUTTON_HEIGHT - SQUARE-SPACE,
        .bf_function = main_button_function,
    },
    [G_SAVE]=
    {
        .id = G_SAVE,
        .name="   Save   ",
        .x=WIDTH_START_OPTIONS,
        .y=HEIGHT_START_RECT+(SQUARE+SPACE)*13,
        .width=GAME_BUTTON_WIDTH,
        .height=GAME_BUTTON_HEIGHT - SQUARE-SPACE,
        .bf_function = main_button_function,
    },

};

Button Buttons_start[]=
{
    {
        .id = START,
        .name = "  Start  ",
        .x = START_SCENE_POS_X_BUTTONS,
        .y = 120,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
    {
        .id = EXAMPLE,
        .name = "Example",
        .x = START_SCENE_POS_X_BUTTONS,
        .y = 220,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
    {
        .id = LOAD,
        .name = " Load ",
        .x = START_SCENE_POS_X_BUTTONS,
        .y = 320,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
    {
        .id = EXIT,
        .name = "   Exit   ",
        .x = START_SCENE_POS_X_BUTTONS,
        .y = 420,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
};

Button Buttons_slots[]=
{
    {
        .id = S_SLOT1,
        .name = " Slot 1 ",
        .x = START_SCENE_POS_X_BUTTONS + 200,
        .y = 120,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
    {
        .id = S_SLOT2,
        .name = " Slot 2 ",
        .x = START_SCENE_POS_X_BUTTONS + 200,
        .y = 220,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
    {
        .id = S_SLOT3,
        .name = " Slot 3 ",
        .x = START_SCENE_POS_X_BUTTONS + 200,
        .y = 320,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
    {
        .id = S_SLOT4,
        .name = " Slot 4 ",
        .x = START_SCENE_POS_X_BUTTONS + 200,
        .y = 420,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT+SQUARE,
        .bf_function = main_button_function,
    },
        {
        .id = S_BACK,
        .name = " Back ",
        .x = 120,
        .y = 420,
        .width = BUTTON_WIDTH - 50,
        .height = BUTTON_HEIGHT,
        .bf_function = main_button_function,
    },
};

start_scene_t scenes[]=
{
    [WELCOME_SCENE]=
    {
        .id = WELCOME_SCENE,
        .background_bmp="images/background.bmp",
        .button_bg_bmp="images/start_button.bmp",
        .button_bg2_bmp="images/picked_button.bmp",
        .buttons=Buttons_start,
        .number_of_buttons=ARRAY_SIZE(Buttons_start),
    },

    [GAME_SCENE]=
    {
        .id = GAME_SCENE,
        .background_bmp="images/background.bmp",
        .button_bg_bmp="images/start2_button.bmp",
        .button_bg2_bmp="images/picked_button.bmp",
        .buttons=Buttons_game,
        .number_of_buttons=ARRAY_SIZE(Buttons_game),
    },

    [SAVINGS_SCENE]=
    {
        .id = SAVINGS_SCENE,
        .background_bmp="images/blankbackground.bmp",
        .button_bg_bmp="images/start_button.bmp",
        .button_bg2_bmp="images/picked_button.bmp",
        .buttons=Buttons_slots,
        .number_of_buttons=ARRAY_SIZE(Buttons_slots),
    },
    [LOADING_SCENE]=
    {
        .id = LOADING_SCENE,
        .background_bmp="images/blankbackground.bmp",
        .button_bg_bmp="images/start_button.bmp",
        .button_bg2_bmp="images/picked_button.bmp",
        .buttons=Buttons_slots,
        .number_of_buttons=ARRAY_SIZE(Buttons_slots),
    },
};


/*static DWORD WINAPI game_thread(void* parameter)
{
    while(!quit)
    {
        //printf("My Thread! Param1:%d, Param2:%d\n", params->param1, params->param2);
        //printf("cos sensowanego\n");
        //fflush(stdout);
        if(current_scene == GAME_SCENE && Buttons_game[G_START].is_pressed)
        {
            play_game();
            draw_every_scene();
            draw_buttons();
            SDL_RenderPresent(renderer);
            Sleep(speed);
        }
        else
            Sleep(100);
    }

    return 0;
}*/

void draw_buttons()
{
    graphics_draw_buttons(renderer, &scenes[current_scene]);
}

void draw_every_scene()
{
    SDL_RenderClear(renderer);
    switch (current_scene)
    {
        case WELCOME_SCENE:
            draw_welcomebackground(renderer, &scenes[WELCOME_SCENE]);
            break;
        case GAME_SCENE:
            draw_gamebackground(renderer);
            graphics_show_board(renderer);
            draw_int(renderer, speed,
                             WIDTH_START_OPTIONS + 1.5*SQUARE, HEIGHT_START_RECT + (SQUARE+SPACE)*7.5,
                             GAME_BUTTON_WIDTH - 3*SQUARE, GAME_BUTTON_HEIGHT - SQUARE-SPACE);
            break;

        case SAVINGS_SCENE:
        case LOADING_SCENE:
            draw_savebackground(renderer, &scenes[SAVINGS_SCENE]);
            break;
    }
}

bool check_exit(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            if (event->key.keysym.sym == SDLK_ESCAPE)
            {
                return true;
            }
            break;
            //printf( "Key press detected: %d\n", (int)event.key.keysym.sym );
    }
    return false;
}

/*void create_game_thread()
{
    DWORD threadDescriptor;

    CreateThread(
        NULL,                   // default security attributes.
        0,                      // use default stack size.
        game_thread,            // thread function name.
        NULL,                   // parameters/argument to thread function.
        0,                      // use default creation flags.
        &threadDescriptor);     // returns the thread identifier.
}
*/

struct timeval start;
struct timeval end;
int calculate_time(struct timeval *start, struct timeval *end)
{
    return ((end->tv_sec - start->tv_sec) + 1e-6*(end->tv_usec - start->tv_usec))*1000;
}

void play()
{
    if(current_scene != GAME_SCENE || !Buttons_game[G_START].is_pressed) return;
    gettimeofday(&end, NULL);
    if (calculate_time(&start, &end) < speed)
    {
        return;
    }
    gettimeofday(&start, NULL);
    play_game();
    draw_every_scene();
    draw_buttons();
    SDL_RenderPresent(renderer);
}

int main(int argc, char ** argv)
{
    SDL_Event event;
    renderer = graphics_init();
    game_init();
    gettimeofday(&start, NULL);
    //create_game_thread();

    for(int i=0;i<ARRAY_SIZE(scenes); i++)
        create_scene(renderer, &scenes[i]);

    draw_every_scene();
    draw_buttons();
    SDL_RenderPresent(renderer);

    //Sleep(10000); printf("tu jestem\n");
    while (!quit)
    {
        // SDL_WaitEvent(&event);   //while(SDL_PollEvent(&event))
        while(SDL_PollEvent(&event))
        {
            quit = check_exit(&event);
            graphics_event(renderer, &scenes[current_scene], &event);
            if(current_scene == GAME_SCENE)
            {
                game_event(renderer, &event);
            }
        }
        play();
        Sleep(1);
    }

    for(int i=0;i<ARRAY_SIZE(scenes); i++)
        destroy_scene(&scenes[i]);
    graphics_destroy(renderer);
    game_destroy();
    return 0;
}
