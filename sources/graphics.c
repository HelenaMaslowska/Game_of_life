#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "SDL_ttf.h"
#include "common.h"
#include "graphics.h"
#include "game.h"

TTF_Font* font;
SDL_Color fColor;
SDL_Rect fontRect;
SDL_Surface* fontSurface;
static SDL_Window * window;


static void fontInit()
{
    TTF_Init();
    font = TTF_OpenFont("fonts/arial.ttf", 64);
    //printf("font: %p\n", font);
    fColor.r = 0;
    fColor.g = 0;
    fColor.b = 0;
}

SDL_Renderer * graphics_init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {printf("Error: %s\n", SDL_GetError()); return NULL;}
    window = SDL_CreateWindow("The game of life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    fontInit();
    return renderer;
}

void graphics_destroy(SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_string(SDL_Renderer * renderer, char *c, int x, int y, int w, int h)
{
    fontSurface = TTF_RenderText_Solid(font, c, fColor);
    fontRect.x = x;
    fontRect.y = y;
    fontRect.w = w;
    fontRect.h = h;
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, fontSurface);
    SDL_RenderCopy(renderer, Message, NULL, &fontRect);
    SDL_DestroyTexture(Message);
}

void draw_int(SDL_Renderer * renderer, int number, int x, int y, int w, int h)
{
    char s[50];
    snprintf(s, sizeof(s), "%d", number);
    draw_string(renderer, s, x,y,w,h);
}

static void draw_button(SDL_Renderer * renderer, start_scene_t *scene, Button *button)
{
    char *s;
    SDL_Rect rect;
    rect = (SDL_Rect){.x = button->x, .y = button->y, .w = button->width, .h = button->height};
    SDL_SetRenderDrawColor(renderer, button->r, button->g, button->b, 0);
    if(button->is_focused)
    {
        SDL_RenderCopy(renderer, scene->bg_button_texture, NULL, &rect);
        SDL_RenderCopy(renderer, scene->bg2_button_texture, NULL, &rect);
    }
    else
    {
        SDL_RenderCopy(renderer, scene->bg_button_texture, NULL, &rect);
    }

    if(button->name_pressed)
    {
        if(button->is_pressed)  s = button->name_pressed;
        else                    s = button->name;
    }
    else
    {
        s = button->name;
    }
    draw_string(renderer, s, button->x, button->y, button->width, button->height);
}

void graphics_draw_buttons(SDL_Renderer * renderer, start_scene_t *scene)
{
    Button * every_button  = scene->buttons;
    for(int i=0; i<scene->number_of_buttons; i++, every_button++)
    {
        draw_button(renderer, scene, every_button);
    }
}

void draw_welcomebackground(SDL_Renderer * renderer, start_scene_t *scene)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, scene->image_background_texture, NULL, NULL);
}

void draw_savebackground(SDL_Renderer * renderer, start_scene_t *scene)
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, scene->image_background_texture, NULL, NULL);
}

void draw_gamebackground(SDL_Renderer * renderer)
{
    //SDL_RenderClear(renderer);
    SDL_Rect rect;
    int pos_start_x = WIDTH_START_RECT, pos_start_y = HEIGHT_START_RECT;
    int end_width = WIDTH - 2*pos_start_x - not_center_x;
    int end_height = HEIGHT - 2*pos_start_y + not_center_y;

    rect = (SDL_Rect){.x = 0, .y = 0, .w = WIDTH, .h = HEIGHT};
    SDL_SetRenderDrawColor(renderer, 128, 200, 128, 0);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);

    //background color a little bit darker
    rect = (SDL_Rect){.x = 0, .y = HEIGHT_START_RECT-SQUARE-SPACE, .w = WIDTH, .h = HEIGHT - 2*HEIGHT_START_RECT+SQUARE+SPACE*2};
    SDL_SetRenderDrawColor(renderer, 100, 170, 100, 0);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);

    //background board
    rect = (SDL_Rect){.x = pos_start_x, .y = pos_start_y, .w = end_width-not_center_x, .h = end_height+not_center_y};
    SDL_SetRenderDrawColor(renderer, 192, 228, 192, 0);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);
}

void graphics_show_board(SDL_Renderer * renderer)
{
    SDL_Rect rect;
    int pos_start_x = WIDTH_START_RECT, pos_start_y = HEIGHT_START_RECT;
    //draw small squares
    for (int i=0;i<SQUARES_WIDTH;i++)
    {
        for(int j=0;j<SQUARES_HEIGHT; j++)
        {
            rect = (SDL_Rect)
            {
                .x = SPACE/2 + pos_start_x + (SQUARE+SPACE)*i,
                .y = SPACE/2 + pos_start_y + (SQUARE+SPACE)*j,
                .w = SQUARE,
                .h = SQUARE
            };

            int draw_color_rect = game_get_color(i, j);
            switch(draw_color_rect)
            {
                case 0:
                    SDL_SetRenderDrawColor(renderer, 100, 170, 100, 0);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0);
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 90, 90, 90, 0);
                    break;
            }

            SDL_RenderFillRect(renderer, &rect);
            SDL_RenderDrawRect(renderer, &rect);
        }
    }
    //SDL_RenderPresent(renderer);
}

void graphics_event(SDL_Renderer *renderer, start_scene_t *scene, SDL_Event *event)
{
    int pos_x;
    int pos_y;
    bool redraw;

    Button *every_button=scene->buttons;
    SDL_GetMouseState(&pos_x, &pos_y);
    for(int i=0; i<scene->number_of_buttons; i++, every_button++)
    {
        redraw = false;
        if  (
                (pos_x>=every_button->x) && (pos_x<=(every_button->x+every_button->width))
            &&  (pos_y>=every_button->y) && (pos_y<=(every_button->y+every_button->height))
            )
        {
            if(!every_button->is_focused)
            {
                redraw = true;
            }
            every_button->is_focused = true;
            if (event->type == SDL_MOUSEBUTTONDOWN)
            {
                if(every_button->name_pressed)
                {
                    every_button->is_pressed = !every_button->is_pressed;
                    redraw=true;
                }
                if (every_button->bf_function)
                {
                    every_button->bf_function(scene, every_button, event);
                }
                //printf("x: %d y: %d %s\n", pos_x, pos_y, b->name);
            }
        }
        else
        {
            if(every_button->is_focused)
            {
                redraw = true;
            }
            every_button->is_focused = false;
        }
        if(redraw)
        {
            //printf("focus: %d\n", every_button->is_focused);
            draw_button(renderer, scene, every_button);
            SDL_RenderPresent(renderer);
        }
    }
}

void graphics_load_image_from_file(SDL_Renderer * renderer, char *image_name, int x, int y, int w, int h)
{
    SDL_Surface * image_surface;
    SDL_Texture * image_texture;
    SDL_Rect rect;
    rect = (SDL_Rect){.x = x, .y = y, .w = w, .h = h};
    image_surface = SDL_LoadBMP(image_name);
    image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_RenderCopy(renderer, image_texture, NULL, &rect);
    SDL_DestroyTexture(image_texture);
    SDL_FreeSurface(image_surface);
}

void create_scene(SDL_Renderer * renderer, start_scene_t *scene)
{

    scene->image_background_surface = SDL_LoadBMP(scene->background_bmp);
    scene->bg_button_surface        = SDL_LoadBMP(scene->button_bg_bmp);
    scene->bg2_button_surface       = SDL_LoadBMP(scene->button_bg2_bmp);

    scene->image_background_texture = SDL_CreateTextureFromSurface(renderer, scene->image_background_surface);
    scene->bg_button_texture        = SDL_CreateTextureFromSurface(renderer, scene->bg_button_surface);
    scene->bg2_button_texture       = SDL_CreateTextureFromSurface(renderer, scene->bg2_button_surface);

    SDL_RenderCopy(renderer, scene->image_background_texture, NULL, NULL);
}


void destroy_scene(start_scene_t *scene)
{
    SDL_DestroyTexture(scene->image_background_texture);
    SDL_DestroyTexture(scene->bg_button_texture);
    SDL_DestroyTexture(scene->bg2_button_texture);

    SDL_FreeSurface(scene->bg_button_surface);
    SDL_FreeSurface(scene->bg2_button_surface);
    SDL_FreeSurface(scene->image_background_surface);
}
