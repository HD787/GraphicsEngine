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


    renderContext* rc = createRenderContext(1000, 700);
    

    /*START OF SDL BOILERPLATE*/

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window;    
    window = SDL_CreateWindow("Display Image",
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                rc->width, rc->height, 0);
   
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);
    int format;
    format = SDL_PIXELFORMAT_RGB24;
    
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                             format, SDL_TEXTUREACCESS_STREAMING,
                                             rc->width, rc->height);
    if(!texture){printf("%s", SDL_GetError()); return 0;}

    SDL_UpdateTexture(texture, NULL, rc->frameBuffer, rc->width * 3);

    /*END OF SDL BOILERPLATE*/

    vertexBuffer* vb = malloc(sizeof(vertexBuffer));

    vb->length = 108;
    vb->vertices = correctedCube;
    // mesh* mesh = malloc(sizeof(mesh));
    // mesh->vb = vb;
    // mesh->centroid = malloc(sizeof(vec3));
    // calculateCentroid(mesh);
    normalBuffer* nb = generateNormals(vb);
    colorBuffer* cb = malloc(sizeof(colorBuffer));
    cb->colors = malloc(sizeof(byte) * vb->length);
    cb->length = vb->length;
    for(int i = 0; i < cb->length; i++) cb->colors[i] = 127;
    vec3 light; light.x = 0; light.y = 0; light.z = 1;
    


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
    float mz = -5;
    float angleX = 0.0;
    float angleY = 0.0;

    //update matrices
    matrix4x4 rotationMatrixX, rotationMatrixY, translationMatrix, scalingMatrix, perspectiveProjectionMatrix, screenSpaceMatrix;
    createPerspectiveProjectionMatrix(45.0, 1.0, 10.0, 1000.0/700.0, perspectiveProjectionMatrix);
    createScalingMatrix(0.5f, scalingMatrix);
    createTranslationMatrix(mx, my, mz, translationMatrix);
    createRotationMatrixY(angleY, rotationMatrixY);
    createRotationMatrixX(angleX, rotationMatrixX);
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

        if(keystate[SDL_SCANCODE_E]){angleX += 0.5;}
        if(keystate[SDL_SCANCODE_F]){angleX -= 0.5;}
        if(keystate[SDL_SCANCODE_R]){angleY += 0.5;}
        if(keystate[SDL_SCANCODE_G]){angleY -= 0.5;}

        
        color clr; clr.r = 200; clr.g = 200; clr.b = 200;
        //linear operations
        for(int i = 0; i < framevb->length; i += 3){
            vec4 temp;
            temp.x = vb->vertices[i];
            temp.y = vb->vertices[i + 1];
            temp.z = vb->vertices[i + 2];
            temp.w = 1.0f;

            //not ideal that this has to be a vec4
            vec4 normTemp;
            normTemp.x = nb->normals[i];
            normTemp.y = nb->normals[i + 1];
            normTemp.z = nb->normals[i + 2];
            normTemp.w = 1.0;
            createTranslationMatrix(mx, my, mz, translationMatrix);
            quaternion quatX = createRotationQuaternion(angleX, 1, 0, 0);
            quaternion quatY = createRotationQuaternion(angleY, 0, 1, 0);
            quaternion compositeQuaternion = multiplyQuaternion(quatX, quatY);
            vec3 tempdh = dehomogenizeVector(temp);
            vec3 normTempdh = dehomogenizeVector(normTemp);
            printf("\n%f\n", tempdh.x);
            rotateVectorViaQuaternion(&tempdh, compositeQuaternion);
            rotateVectorViaQuaternion(&normTempdh, compositeQuaternion);
            printf("%f\n", tempdh.x);
            temp = homogenizeVector(tempdh);
            normTemp = homogenizeVector(normTempdh); 
            //createRotationMatrixX(angleX, rotationMatrixX);
            //createRotationMatrixY(angleY, rotationMatrixY);
            // vecByMatrix4x4(&temp, rotationMatrixX);
            // vecByMatrix4x4(&normTemp, rotationMatrixX);
            // vecByMatrix4x4(&temp, rotationMatrixY);
            // vecByMatrix4x4(&normTemp, rotationMatrixY);

            vecByMatrix4x4(&temp, translationMatrix);
            
            //vecByMatrix4x4(&temp, scalingMatrix);
            //createRotationMatrix(angle, translationMatrix);

            //this is annoying
            //printf("%f, %f, %f\n", normTemp.x, normTemp.y, normTemp.z);
            vec3 normTempH; normTempH.x = normTemp.x; normTempH.y = normTemp.y; normTempH.z = normTemp.z;
            normalizeVector(&normTempH);
            float lightScalar = dotProduct(normTempH, light);
            lightScalar += 1;
            //lightScalar = fabsf(lightScalar);
            framecb->colors[i] = RGBClamp(cb->colors[i] * lightScalar);
            framecb->colors[i + 1] = RGBClamp(cb->colors[i + 1] * lightScalar);
            framecb->colors[i + 2] = RGBClamp(cb->colors[i + 2] * lightScalar);
            
            vecByMatrix4x4(&temp, perspectiveProjectionMatrix);

            //perspective divide
            if(temp.w < 0.01 && temp.w > 0) temp.w = .009f;
            if(temp.w > -0.01 && temp.w < 0) temp.w = -.009f;
            if(temp.w < -.01) temp.w = fabsf(temp.w);
            temp.x /= temp.w;
            temp.y /= temp.w;
            temp.z /= temp.w;
            temp.w = 1.0f;
            vecByMatrix4x4(&temp, screenSpaceMatrix);
            
            //create the temporary VBO
            framevb->vertices[i] = temp.x;
            framevb->vertices[i + 1] = temp.y;
            framevb->vertices[i + 2] = temp.z;
        }

        //build framebuffer
       
        rasterize(rc, framevb, framecb);
        SDL_UpdateTexture(texture, NULL, rc->frameBuffer, rc->width * 3);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        //SDL_Delay(30);
    }
    deleteRenderContext(rc);
    //dont forget to free the vertex buffers
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}