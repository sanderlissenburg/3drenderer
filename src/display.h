#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME (1000/FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;

bool init_window(void); 

void render_color_buffer(void);

void clear_color_buffer(u_int32_t color);

void draw_grid(int color, bool grid);

void draw_pixel(int x, int y, int32_t colo);

void draw_rect(int x, int y, int width, int height, int32_t color);

void destroy_window();

#endif