#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

bool is_running = false;

const int N_POINTS = 9*9*9;
vect3_t cube_points[N_POINTS];
vect2_t projected_points[N_POINTS];
vect3_t cube_rotation = {0, 0, 0};

float fov_factor = 512;
vect3_t camera_position = {0, 0, -5};

int previous_frame_time = 0;

void setup(void) {
    color_buffer = (uint32_t*) malloc(window_width * window_height * sizeof(u_int32_t));

    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );

    int i = 0;

    float step = 0.25;

    for (float x = -1.0; x <= 1.0; x += step ) {
        for (float y = -1.0; y <= 1.0; y += step ) {
            for (float z = -1.0; z <= 1.0; z += step ) {
                vect3_t point = { x, y, z};
                cube_points[i] = point;
                i++;
            }
        }
    }
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
            }
            break;
    }
}

vect2_t project(vect3_t point) {
    vect2_t projected_point = {
        (point.x * fov_factor) / point.z,
        (point.y * fov_factor) / point.z
    };

    return projected_point;
}

void update(void) {

    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
    }

    previous_frame_time = SDL_GetTicks();

    cube_rotation.x += 0.01;
    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for (int i = 0; i < N_POINTS; i++) {
        vect3_t point = cube_points[i];

        vect3_t transformed_point = vect3_rotate_x(point, cube_rotation.x);
        transformed_point = vect3_rotate_y(transformed_point, cube_rotation.y);
        transformed_point = vect3_rotate_z(transformed_point, cube_rotation.z);

        transformed_point.z -= camera_position.z;
        
        vect2_t projected_point = project(transformed_point);
        projected_points[i] = projected_point;
    }
}

void render(void) {
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderClear(renderer);

    draw_grid(0xFFCCCCCC, false);
    //draw_rect(750, 550, 200, 200, 0xFFFF0000);

    for (int i = 0; i < N_POINTS; i++) {
        vect2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + window_width/2, 
            projected_point.y + window_height/2, 
            4, 
            4, 
            0xFFFF0000
        );
    }

    render_color_buffer();

    clear_color_buffer(0xFF000000);


    SDL_RenderPresent(renderer);    
}

void destroy(void) {
    destroy_window();
}

int main(void) {
    is_running = init_window();

    if (! is_running) {
        fprintf(stderr, "No window could be initialized.\n");
        return 1;
    }

    setup();

    while (is_running) {
        process_input();
        update();
        render();
    }

    destroy();
    
    return 0;
}