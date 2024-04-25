//I will do literally anything but use a makefile i guess
//clang SDL_helper.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2
#include <SDL.h>
#include <string.h>
#include "raster/rasterizer.h"
#include "3Dmath/operations.h"
#include "graphicsEnginePrivateFunctions.h"
#include "commonCoords.h"


int main(){
    SDL_Init(SDL_INIT_VIDEO);
    //char* path = argv[1];
    //hardcoding option
    //rgbArray* vals = load();
    vertexBuffer* vb = malloc(sizeof(vertexBuffer));

    vb->length = 9;
    vb->vertices = normalizedTriangle;
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

    
    
    //the vertex buffer that is actually altered.
    vertexBuffer* framevb = malloc(sizeof(vertexBuffer));
    framevb->length = vb->length;
    framevb->vertices = malloc(sizeof(int) * vb->length);

    //movement variables
    float mx = 0;
    float my = 0;
    float mz = 0;
    float angle = 0.0;

    //update matrices
    matrix4x4 rotationMatrix, translationMatrix, scalingMatrix, perspectiveProjectionMatrix;
    //createPerspectiveProjectionMatrix(45.0, 0.1, 100.0, 1000.0/700.0, perspectiveProjectionMatrix);
    createScalingMatrix(0.5f, scalingMatrix);
    createTranslationMatrix(mx, my, mz, translationMatrix);
    createRotationMatrixX(angle, rotationMatrix);

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
        if(keystate[SDL_SCANCODE_W]){mz += 0.01f;}
        if(keystate[SDL_SCANCODE_A]){mx -= 0.01f;}
        if(keystate[SDL_SCANCODE_S]){mz -= 0.01f;}
        if(keystate[SDL_SCANCODE_D]){mx += 0.01f;}
        if(keystate[SDL_SCANCODE_LSHIFT]){ my -= 0.01f;}
        if(keystate[SDL_SCANCODE_SPACE]){ my += 0.01f;}

        // if(keystate[SDL_SCANCODE_D]){angle += 0.1;}
        // if(keystate[SDL_SCANCODE_A]){angle -= 0.1;}
        


        //linear operations
        for(int i = 0; i < framevb->length; i += 3){
            vec4 temp;
            temp.x = (float)vb->vertices[i];
            temp.y = (float)vb->vertices[i + 1];
            temp.z = (float)vb->vertices[i + 2];
            temp.w = 1.0f;
            //printf("old: %f, %f , %f\n", temp.x, temp.y, temp.z);
            createTranslationMatrix(mx, my, mz, translationMatrix);
            vecByMatrix4x4(&temp, translationMatrix);
            //vecByMatrix4x4(&temp, scalingMatrix);
            //createRotationMatrix(angle, translationMatrix);
            
            //vecByMatrix4x4(&temp, rotationMatrix);
            //vecByMatrix4x4(&temp, perspectiveProjectionMatrix);

            
            //perspective divide
            // if(temp.w != 0){
            //     temp.x /= temp.w;
            //     temp.y /= temp.w;
            //     temp.z /= temp.w;
            // }
            // if(temp.z != 0){
            //     temp.x /= temp.z;
            //     temp.y /= temp.z;
            // }
            
            //printf(": %f, %f, %f :", temp.x, temp.y, temp.z);
            printf(" %f ", temp.z);
            //create the temporary VBO
            framevb->vertices[i] = temp.x;
            framevb->vertices[i + 1] = temp.y;
            framevb->vertices[i + 2] = temp.z;
        }

        //build framebuffer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // RGBA for black
       
        rasterize(fb, framevb);
        SDL_UpdateTexture(texture, NULL, fb->pixels, fb->width * 3);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        //SDL_Delay(30);
    }
    deleteFrameBuffer(fb);
    //dont forget to free the vertex buffers
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}