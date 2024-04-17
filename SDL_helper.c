//I will do literally anything but use a makefile i guess
//clang SDL_helper.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2
#include <SDL.h>
#include <string.h>
#include "raster/rasterizer.h"
#include "linearAlgebra/operations.h"


int main(){
    SDL_Init(SDL_INIT_VIDEO);
    //char* path = argv[1];
    //hardcoding option
    //rgbArray* vals = load();
    vertexBuffer* vb = malloc(sizeof(vertexBuffer));
    static int verticesArray[] = {

    400, 300, 200, // Vertex 1
    600, 300, 100, // Vertex 2
    500, 500, 300, // Vertex 3 (apex)

    600, 300, 200, // Vertex 1 (shared with Triangle 1)
    800, 300, 100, // Vertex 2
    700, 500, 300  // Vertex 3 (apex)
    };

    vb->length = 18;
    vb->vertices = verticesArray;
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
    createRotationMatrixY(60.0, rotationMatrix);
    createTranslationMatrix(0.0, 0.0, 0.0, translationMatrix);
    createPerspectiveProjectionMatrix(60.0, 1.0, 100.0, 1000.0/700.0, perspectiveProjectionMatrix);
    
    //the vertex buffer that is actually altered.
    vertexBuffer* framevb = malloc(sizeof(vertexBuffer));
    framevb->length = vb->length;
    framevb->vertices = malloc(sizeof(int) * vb->length);

    //movement variables
    float mx = 0;
    float my = 0;
    float mz = 0;
    float angle = 0.0;

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
        if(keystate[SDL_SCANCODE_W]){mz += 1.0f;}
        if(keystate[SDL_SCANCODE_A]){mx -= 1.0f;}
        if(keystate[SDL_SCANCODE_S]){mz -= 1.0f;}
        if(keystate[SDL_SCANCODE_D]){mx += 1.0f;}
        if(keystate[SDL_SCANCODE_LSHIFT]){ my -= 1.0f;}
        if(keystate[SDL_SCANCODE_SPACE]){ my += 1.0f;}
        // if(keystate[SDL_SCANCODE_D]){angle += 0.1;}
        // if(keystate[SDL_SCANCODE_A]){angle -= 0.1;}
        //update matrices
        createTranslationMatrix(mx, my, mz, translationMatrix);
        //createRotationMatrixY(angle, rotationMatrix);


        //linear operations
        for(int i = 0; i < framevb->length; i += 3){
            vec4 temp;
            temp.x = (float)vb->vertices[i];
            temp.y = (float)vb->vertices[i + 1];
            temp.z = (float)vb->vertices[i + 2];
            temp.w = 1.0f;
            //printf("old: %f, %f , %f\n", temp.x, temp.y, temp.z);
            vecByMatrix4x4(&temp, translationMatrix);
            //vecByMatrix4x4(&temp, rotationMatrix);
            //printf("new: %f, %f , %f\n", temp.x, temp.y, temp.z);
            //vecByMatrix4x4(&temp, perspectiveProjectionMatrix);
            
            //perspective divide
            // temp.x = temp.x / temp.w;
            // temp.y = temp.y / temp.w;
            // temp.z = temp.z / temp.w;
            
            //create the temporary VBO
            framevb->vertices[i] = (int)(temp.x + 0.5f) ;
            framevb->vertices[i + 1] = (int)(temp.y + 0.5f);
            framevb->vertices[i + 2] = (int)(temp.z + 0.5f);
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