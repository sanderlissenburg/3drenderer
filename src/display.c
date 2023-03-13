#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

bool init_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);

    //window_width = display_mode.w;
    //window_height = display_mode.h;

    window = SDL_CreateWindow(
        NULL, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );

    if (! window) {
        fprintf(stderr, "No SDL window could be created.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        0
    );

    if (! renderer) {
        fprintf(stderr, "No SDL renderer could be created.\n");
        return false;
    }

    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    return true;
}

void render_color_buffer(void) {
    SDL_UpdateTexture(
        color_buffer_texture,
        NULL,
        color_buffer,
        (int) (window_width * sizeof(uint32_t))
    );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(u_int32_t color) {
    for (int i = 0; i < window_height * window_width; i++) {
        color_buffer[i] = color;
    }    
}

void draw_grid(int color, bool grid) {
    for (int y = 0; y < window_height; y++)
    {
        for (int x = 0; x < window_width; x++)
        {
            if (((y % 10 == 0 || x % 10) == 0 && grid) || ((y % 10 == 0 && x % 10 == 0) && !grid)) {
                draw_pixel(x, y, color);
            } 
        }
    }
    
}

void draw_pixel(int x, int y, int32_t color) {
    if (x >= 0 && x < window_width && y >= 0 && y < window_height) {
        color_buffer[(y * window_width) + x] = color;
    }   
}

void draw_line(int x0, int y0, int x1, int y1) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

    float x_increment = delta_x / (float)side_length;
    float y_increment = delta_y / (float)side_length;

    // int side_length = abs(delta_x);
    // float x_increment = delta_x / (float)side_length;
    // float y_increment = (float)(delta_y/delta_x);

    float current_x = x0;
    float current_y = y0;

    for (int i = 0; i < side_length; i++) {
        draw_pixel(round(current_x), round(current_y), 0xFF00FF00);
        current_x += x_increment;
        current_y += y_increment;
    }
}

void draw_rect(int x, int y, int width, int height, int32_t color) {
    for (int posY = y; posY < y + height; posY++) {
        for (int posX = x; posX < x + width; posX++) {
            draw_pixel(posX, posY, color);
        }
    }
}

void destroy_window() {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}