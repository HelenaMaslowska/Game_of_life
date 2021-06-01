#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include "common.h"
#include "game.h"
#include "graphics.h"

static int **board;
static int **new_board;

void change_number_on_board(int **board, int y, int x)
{
    if(board[x][y])
        board[x][y] = 0;
    else
        board[x][y] = 1;
}

int game_add_delete_element_on_keyboard(int pos_x, int pos_y)
{
    int pos_square_x, pos_square_y;
    for(int i=0;i<SQUARES_WIDTH;i++)
    {
        for(int j=0;j<SQUARES_HEIGHT;j++)
        {
            pos_square_x = WIDTH_START_RECT + SQUARE*i + 2*i + 1;
            pos_square_y = HEIGHT_START_RECT+SQUARE*j + 2*j + 1;
            if( pos_square_x<pos_x && pos_square_x+SQUARE > pos_x && pos_square_y < pos_y && pos_square_y+SQUARE > pos_y )
            {
                //printf("x = %d, y = %d\n", pos_x, pos_y);
                change_number_on_board(board, i, j);
                return 1;
            }
        }
    }
    return 0;
}

static int **create_board()
{
    int **board = (int**)malloc(sizeof(int*)*(SQUARES_HEIGHT));
    if(board == NULL)   { fputs("Error: \n", stderr); exit(2); }
    for(int i=0; i<SQUARES_HEIGHT; i++)
    {
        board[i] = (int*)malloc(sizeof(int)*(SQUARES_WIDTH));
        for(int j=0; j<SQUARES_WIDTH; j++)
        {
            board[i][j] = 0;
        }
    }
    return board;
}

bool game_read_from_file(char * name_file)
{
    FILE *file = fopen(name_file, "r");
    if(!file) return false;
    for(int i=0; i<SQUARES_HEIGHT; i++)
    {
        for (int j=0; j<SQUARES_WIDTH; j++)
        {
            fscanf(file, "%d ", &board[i][j]);
        }
    }
    fclose(file);
    return true;
}

void game_save_to_file(char * name_file)
{
    FILE *file = fopen(name_file, "w");
    if(!file) return;
    for(int i=0; i<SQUARES_HEIGHT; i++)
    {
        for (int j=0; j<SQUARES_WIDTH; j++)
        {
            fprintf(file, "%d ", board[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void clean_board()
{
    for(int i=0; i<SQUARES_HEIGHT; i++)
    {
        for(int j=0; j<SQUARES_WIDTH;j++)
            board[i][j] = 0;
    }
}

void game_event(SDL_Renderer *renderer, SDL_Event *event)
{
    int pos_x, pos_y;
    if(event->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&pos_x, &pos_y);
        if(game_add_delete_element_on_keyboard(pos_x, pos_y))
        {
            graphics_show_board(renderer);
        }
        SDL_RenderPresent(renderer);
        //draw_every_scene();
    }
}

void game_init()
{
    board = create_board();
    new_board = create_board();
}

int game_get_color(int y, int x)
{
    if(!board)
        printf("blad\n");
    return board[x][y];
}

void game_destroy()
{
    for(int i=0;i<SQUARES_HEIGHT; i++)
    {
        free(board[i]);
    }
    free(board);
}

//--------------------------------------------------------------------------------------------------------------
void copy_board(int **new_board, int **board)
{
	for(int i=0;i<SQUARES_HEIGHT;i++)
	{
		for(int j=0;j<SQUARES_WIDTH;j++)
		{
			board[i][j]=new_board[i][j];
		}
	}
}
void show_board(int **board)
{
    for(int i=0; i<SQUARES_HEIGHT; i++)
    {
        for(int j=0; j<SQUARES_WIDTH; j++)
        {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int how_many_neighbours(int **board, int height, int width)
{
	int neighbours = 0;
	if(height+1 < SQUARES_HEIGHT    && board[height+1][width] > 0) neighbours++;
	if(height > 0                   && board[height-1][width] > 0) neighbours++;
	if(width+1 < SQUARES_WIDTH      && board[height][width+1] > 0) neighbours++;
	if(width > 0                    && board[height][width-1] > 0) neighbours++;

	if(height > 0                   && width+1 < SQUARES_WIDTH  && board[height-1][width+1] > 0) neighbours++;
	if(height+1 < SQUARES_HEIGHT    && width+1 < SQUARES_WIDTH  && board[height+1][width+1] > 0) neighbours++;
	if(height > 0                   && width > 0                && board[height-1][width-1] > 0) neighbours++;
	if(height+1 < SQUARES_HEIGHT    && width > 0                && board[height+1][width-1] > 0) neighbours++;
	return neighbours;
}

void add_neighbour(int **new_board, int **board)
{
	int neighbours;
	for(int width=0;width<SQUARES_WIDTH;width++)
	{
		for(int height=0;height<SQUARES_HEIGHT;height++)
		{
			neighbours = how_many_neighbours(board, height, width);
			if(neighbours==3)
			{
			    if(board[height][width] < 4)
                    new_board[height][width]++;
			}
		}
	}
}

void delete_neighbour(int **new_board, int **board)
{
    int neighbours;
	for(int width=0; width<SQUARES_WIDTH; width++)
	{
		for(int height=0; height<SQUARES_HEIGHT; height++)
		{
			neighbours = how_many_neighbours(board, height, width);
			if(neighbours>3 || neighbours<2)
			{
				new_board[height][width] = 0;
			}
		}
	}
}

void play_game()
{
    copy_board(board, new_board);
    add_neighbour(new_board, board);
    delete_neighbour(new_board, board);
    //show_board(board);
    copy_board(new_board, board);
}
