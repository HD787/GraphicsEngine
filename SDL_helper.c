//I will do literally anything but use a makefile i guess
//clang SDL_helper.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2
#include <SDL.h>
#include <string.h>
#include "Raster/rasterizer.h"
#include "linearAlgebra/operations.h"


int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    char* path = argv[1];
    //hardcoding option
    //rgbArray* vals = load();
    vertexBuffer vb;
    static int verticesArray[] = {
    // Triangle 1
    400, 300, 200, 1, // Vertex 1
    600, 300, 100, 1, // Vertex 2
    500, 500, 300, 1, // Vertex 3 (apex)

    // Triangle 2
    600, 300, 200, 1, // Vertex 1 (shared with Triangle 1)
    800, 300, 100, 1, // Vertex 2
    700, 500, 300, 1, // Vertex 3 (apex)
    };

    vb.length = 18;
    vb.vertices = verticesArray;
    framebuffer* fb = createFrameBuffer(1000, 700);

    SDL_Window* window;    
    if (fb->height > 2000){
    window = SDL_CreateWindow("Display Image",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                1000, 700, 0);
    }else
    window = SDL_CreateWindow("Display Image",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                fb->width, fb->height, 0);
   
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int format;
    format = SDL_PIXELFORMAT_RGB24;
    
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             format, SDL_TEXTUREACCESS_STREAMING,
                                             fb->width, fb->height);
    if(!texture){printf("%s", SDL_GetError()); return 0;}

    SDL_UpdateTexture(texture, NULL, fb->pixels, fb->width * 3);

    matrix4x4 rotationMatrix, translationMatrix, perspectiveProjectionMatrix;
    createRotationMatrixX(0.0, rotationMatrix);
    createTranslationMatrix(0.0, 0.0, 0.0, translationMatrix);
    createPerspectiveProjectionMatrix(60.0, 1.0, 100.0, 1000.0/700.0, perspectiveProjectionMatrix);
    

    int quit = 0;
    SDL_Event e;
    while(!quit){
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
        //input gathering
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        if(keystate[SDL_SCANCODE_W]){z += 0.1;}
        if(keystate[SDL_SCANCODE_A]){x -= 0.1;}
        if(keystate[SDL_SCANCODE_S]){z -= 0.1;}
        if(keystate[SDL_SCANCODE_D]){x += 0.1;}
        if(keystate[SDL_SCANCODE_LSHIFT]){ y -= 0.1;}
        if(keystate[SDL_SCANCODE_SPACE]){ y += 0.1;}

        //linear operations


        //build fram
        rasterize(fb, )
        SDL_UpdateTexture(texture, NULL, fb->pixels, fb->width * 3);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    deleteFrameBuffer(fb);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}