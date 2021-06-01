#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__


typedef void (*button_function)(void *scene, void *button, SDL_Event *event);

typedef struct
{
    int id;
    char *name;
    char *name_pressed;
    int x;
    int y;
    int width;
    int height;
    int r;
    int g;
    int b;
    bool is_focused;
    bool is_pressed;
    button_function bf_function;
}Button;

typedef struct
{
    int id;
    char *background_bmp;
    char *button_bg_bmp;
    char *button_bg2_bmp;
    SDL_Surface * image_background_surface;
    SDL_Surface * bg_button_surface;
    SDL_Surface * bg2_button_surface;

    SDL_Texture * image_background_texture;
    SDL_Texture * bg_button_texture;
    SDL_Texture * bg2_button_texture;

    Button * buttons;
    int number_of_buttons;
} start_scene_t;

SDL_Renderer *graphics_init();
void create_scene(SDL_Renderer * renderer, start_scene_t *scene);
void graphics_event(SDL_Renderer *renderer, start_scene_t *scene, SDL_Event *event);

void graphics_draw_buttons(SDL_Renderer * renderer, start_scene_t *scene);
void draw_welcomebackground(SDL_Renderer * renderer, start_scene_t *scene);
void draw_gamebackground(SDL_Renderer * m_window_renderer);
void draw_savebackground(SDL_Renderer * renderer, start_scene_t *scene);

void draw_int(SDL_Renderer * renderer, int number, int x, int y, int w, int h);
void draw_string(SDL_Renderer * renderer, char *c, int x, int y, int w, int h);
void graphics_show_board(SDL_Renderer * renderer);
void graphics_load_image_from_file(SDL_Renderer * renderer, char *image_name, int x, int y, int w, int h);

void destroy_scene(start_scene_t *scene);
void graphics_destroy(SDL_Renderer *renderer);


#endif // DECL_FUNC_C
