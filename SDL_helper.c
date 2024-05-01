//I will do literally anything but use a makefile i guess
//clang SDL_helper.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2
#include <SDL.h>
#include <string.h>
#include "raster/rasterizer.h"
#include "3Dmath/operations.h"
#include "engineTypes.h"
#include "graphicsEnginePrivateFunctions.h"
#include "commonCoords.h"


int main(){


    frameBuffer* fb = createFrameBuffer(1000, 700);
    


    /*START OF SDL BOILERPLATE*/

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;    
    if (fb->height > 2000){
    window = SDL_CreateWindow("Display Image",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                1000, 700, 0);
    }else
    window = SDL_CreateWindow("Display Image",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                fb->width, fb->height, 0);
   
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    int format;
    format = SDL_PIXELFORMAT_RGB24;
    
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             format, SDL_TEXTUREACCESS_STREAMING,
                                             fb->width, fb->height);
    if(!texture){printf("%s", SDL_GetError()); return 0;}

    SDL_UpdateTexture(texture, NULL, fb->pixels, fb->width * 3);

    /*END OF SDL BOILERPLATE*/

    vertexBuffer* vb = malloc(sizeof(vertexBuffer));

    vb->length = 108;
    vb->vertices = normalizedCubeVertices;
    mesh* mesh = malloc(sizeof(mesh));
    mesh->vb = vb;
    mesh->centroid = malloc(sizeof(vec3));
    calculateCentroid(mesh);
    normalBuffer* nb = generateNormals(vb);
    colorBuffer* cb = malloc(sizeof(colorBuffer));
    cb->colors = malloc(sizeof(byte) * vb->length);
    cb->length = vb->length;
    for(int i = 0; i < cb->length; i++) cb->colors[i] = 200;
    vec3 light; light.x = -1; light.y = -1; light.z = 1;
    


    //this doesn't feel right
    //alternative idea: some large vector that contains all of the vertices to be rendered
    //looking back im not sure what i meant by this^
    //the vertex buffer that is actually altered.
    //normal can probably just be kept as a vector in the "vertex shader"
    vertexBuffer* framevb = malloc(sizeof(vertexBuffer));
    colorBuffer* framecb = malloc(sizeof(colorBuffer));
    framecb->colors = malloc(sizeof(byte) * vb->length);
    framevb->length = vb->length;
    framevb->vertices = malloc(sizeof(int) * vb->length);

    //movement variables
    float mx = 0;
    float my = 0;
    float mz = 0;
    float angle = 0.0;

    //update matrices
    matrix4x4 rotationMatrix, translationMatrix, scalingMatrix, perspectiveProjectionMatrix, screenSpaceMatrix;
    createPerspectiveProjectionMatrix(45.0, 1.0, 10.0, 1000.0/700.0, perspectiveProjectionMatrix);
    createScalingMatrix(0.5f, scalingMatrix);
    createTranslationMatrix(mx, my, mz, translationMatrix);
    createRotationMatrixX(angle, rotationMatrix);
    createNDCToScreenSpaceMatrix(1000, 700, screenSpaceMatrix);

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
        if(keystate[SDL_SCANCODE_W]){my += 0.01f;}
        if(keystate[SDL_SCANCODE_A]){mx -= 0.01f;}
        if(keystate[SDL_SCANCODE_S]){my -= 0.01f;}
        if(keystate[SDL_SCANCODE_D]){mx += 0.01f;}
        if(keystate[SDL_SCANCODE_LSHIFT]){ mz -= 0.01f;}
        if(keystate[SDL_SCANCODE_SPACE]){ mz += 0.01f;}

        if(keystate[SDL_SCANCODE_E]){angle += 0.1;}
        if(keystate[SDL_SCANCODE_F]){angle -= 0.1;}
        
        color clr; clr.r = 200; clr.g = 200; clr.b = 200;
        //linear operations
        for(int i = 0; i < framevb->length; i += 3){
            vec4 temp;
            temp.x = vb->vertices[i];
            temp.y = vb->vertices[i + 1];
            temp.z = vb->vertices[i + 2];
            temp.w = 1.0f;

            vec3 normTemp;
            normTemp.x = nb->normals[i];
            normTemp.y = nb->normals[i + 1];
            normTemp.z = nb->normals[i + 2];
            normalizeVector(&normTemp);
            float lightScalar = dotProduct(normTemp, light);
            framecb->colors[i] = cb->colors[i] * lightScalar;
            framecb->colors[i + 1] = cb->colors[i + 1] * lightScalar;
            framecb->colors[i + 2] = cb->colors[i + 2] * lightScalar;
            //printf("old: %f, %f , %f\n", temp.x, temp.y, temp.z);
            createTranslationMatrix(mx, my, mz, translationMatrix);
            createRotationMatrixX(angle, rotationMatrix);
            vecByMatrix4x4(&temp, rotationMatrix);
            vecByMatrix4x4(&temp, translationMatrix);
            
            //vecByMatrix4x4(&temp, scalingMatrix);
            //createRotationMatrix(angle, translationMatrix);
            
            
            vecByMatrix4x4(&temp, perspectiveProjectionMatrix);

            // printf(" %f ", temp.w); 
            //perspective divide
            //clamp values to avoid dramatic results of dividing by values approaching 0
            if(temp.w < 0.01 && temp.w > 0) temp.w = .009f;
            if(temp.w > -0.01 && temp.w < 0) temp.w = -.009f;
            if(temp.w < -.01) temp.w = fabsf(temp.w);
            temp.x /= temp.w;
            temp.y /= temp.w;
            temp.z /= temp.w;
            temp.w = 1.0f;
            vecByMatrix4x4(&temp, screenSpaceMatrix);
            //printf(" %f \n", temp.z); 
            
            //create the temporary VBO
            framevb->vertices[i] = temp.x;
            framevb->vertices[i + 1] = temp.y;
            framevb->vertices[i + 2] = temp.z;
        }

        //build framebuffer
       
        rasterize(fb, framevb, framecb);
        SDL_UpdateTexture(texture, NULL, fb->pixels, fb->width * 3);
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