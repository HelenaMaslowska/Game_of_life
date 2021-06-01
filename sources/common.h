#ifndef COMMON_H
#define COMMON_H

#define ARRAY_SIZE(n) (sizeof(n)/sizeof(n[0]))

enum
{
    WELCOME_SCENE,
    GAME_SCENE,
    SAVINGS_SCENE,
    LOADING_SCENE,
    //LICZBA_SCEN
};

enum
{
    START,
    EXAMPLE,
    LOAD,
    EXIT,
    OPTIONS,
};

enum
{
    G_START,
    G_CLEAN,
    G_SLOW,
    G_FAST,
    G_SAVE,
    G_BACK,
};

enum
{
    S_SLOT1,
    S_SLOT2,
    S_SLOT3,
    S_SLOT4,
    S_BACK,
};

#define WIDTH                   800
#define HEIGHT                  600

#define SQUARE                  20
#define SPACE                   2
#define not_center_x            80
#define not_center_y            20
#define HEIGHT_START_RECT       (HEIGHT - (SQUARE+SPACE)*SQUARES_HEIGHT)/2 + not_center_y
#define WIDTH_START_OPTIONS     (WIDTH_START_RECT+(SQUARES_WIDTH*(SQUARE+SPACE)) + SQUARE)
#define GAME_BUTTON_WIDTH       (WIDTH - WIDTH_START_OPTIONS - SQUARE)
#define GAME_BUTTON_HEIGHT      (SQUARE+SPACE)*3
//(WIDTH/2 + (WIDTH_START_RECT+SQUARES_WIDTH*(SQUARE+SPACE))/2 - BUTTON_WIDTH/2)
#define START_SCENE_POS_X_BUTTONS   (WIDTH/2 - BUTTON_WIDTH/2)


#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

#endif
