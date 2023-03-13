#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

bool is_running = false;

triangle_t triangles_to_render[N_MESH_FACES];

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

    for (int i = 0; i < N_MESH_FACES; i++) {
        face_t mesh_face = mesh_faces[i];

        vect3_t face_vertices[3];

        face_vertices[0] = mesh_vertices[mesh_face.a - 1];
        face_vertices[1] = mesh_vertices[mesh_face.b - 1];
        face_vertices[2] = mesh_vertices[mesh_face.c - 1];
        
        triangle_t projected_triangle;

        for (int j = 0; j < 3; j++) {
            vect3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vect3_rotate_x(transformed_vertex, cube_rotation.x);
            transformed_vertex = vect3_rotate_y(transformed_vertex, cube_rotation.y);
            transformed_vertex = vect3_rotate_z(transformed_vertex, cube_rotation.z);

            transformed_vertex.z -= camera_position.z;

            vect2_t projected_point = project(transformed_vertex);
            projected_point.x += window_width/2, 
            projected_point.y += window_height/2, 
            
            projected_triangle.points[j] = projected_point;
        }

        triangles_to_render[i] = projected_triangle;
    }
}

void render(void) {
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderClear(renderer);

    draw_grid(0xFFCCCCCC, false);
    //draw_rect(750, 550, 200, 200, 0xFFFF0000);

    //draw_line(40, 40, 300, 100);

    for (int i = 0; i < N_MESH_FACES; i++) {

        triangle_t triangle_to_render = triangles_to_render[i];

        for (int j = 0; j < 3; j++) {
            vect2_t projected_point = triangle_to_render.points[j];
            draw_rect(
                projected_point.x,
                projected_point.y, 
                4,
                4, 
                0xFFFF0000
            );
            if (j < 2) {
                draw_line(
                    projected_point.x, 
                    projected_point.y, 
                    triangle_to_render.points[j+1].x,
                    triangle_to_render.points[j+1].y
                ); 
            } else {
                draw_line(
                    projected_point.x, 
                    projected_point.y, 
                    triangle_to_render.points[0].x,
                    triangle_to_render.points[0].y
                ); 
            }
            
        }
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