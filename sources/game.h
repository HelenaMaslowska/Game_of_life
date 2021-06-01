#ifndef __GAME_H__
#define __GAME_H__

#define SQUARES_WIDTH           25
#define SQUARES_HEIGHT          18
#define WIDTH_START_RECT        (WIDTH - (SQUARE+SPACE)*SQUARES_WIDTH)/2 - not_center_x

void game_init();
int game_add_delete_element_on_keyboard(int pos_x, int pos_y);
void game_destroy();
int game_get_color(int y, int x);
void clean_board();
void game_event(SDL_Renderer *renderer, SDL_Event *event);
void play_game();
bool game_read_from_file(char * name_file);
void game_save_to_file(char * name_file);
void show_board(int **board);
/*
*/
/*
void draw_board(SDL_Renderer * renderer, int r, int g, int b, int a)
{
    for(int i=0;i<SQUARES_WIDTH;i++)
        {
            for(int j=0;j<SQUARES_HEIGHT;j++)
            {
                int pos_square_x = WIDTH_START_RECT + SQUARE*i + 2*i + 1;
                int pos_square_y = HEIGHT_START_RECT+SQUARE*j + 2*j + 1;
                if(!board[j][i])
                    draw_rect(renderer, pos_square_x, pos_square_y, 128, 200, 128, 0);
                if(board[j][i])
                    draw_rect(renderer, pos_square_x, pos_square_y, 0, 0, 0, 0);
            }
        }
}
*/

/*
void draw_scene(SDL_Renderer * renderer, int SCENE, int x, int y)
{
    if(SCENE == GAME_SCENE)
    {
        draw_background(renderer);
        draw_board(renderer, 128, 200, 128, 0);
        for(int i=0;i< ARRAY_SIZE(Buttons); i++)
        {
            draw_button(renderer, &Buttons[i]);
        }
    }
}*/

#endif
