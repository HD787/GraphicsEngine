//I will do literally anything but use a makefile i guess
//clang SDL_helper.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2
#include <SDL.h>
#include <string.h>
#include "raster/rasterizer.h"
#include "3Dmath/operations.h"
#include "engineTypes.h"
#include "graphicsEngineFunctions.h"
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


    //cube1
    vertexBuffer* vb0 = createVertexBuffer(108);
    memcpy(vb0->inputVertices, correctedCube, sizeof(float) * 108);
    normalBuffer* nb0 = generateNormals(vb0);
    colorBuffer* cb0 = createColorBuffer(108);
    for(int i = 0; i < cb0->length; i++) cb0->inputColors[i] = 127;

    //cube2
    vertexBuffer* vb1 = createVertexBuffer(108);
    memcpy(vb1->inputVertices, correctedCube, sizeof(float) * 108);
    matrix4x4 translation1;
    createTranslationMatrix(-1.5, 0, 0, translation1);
    vertexBufferByMatrix(vb1, translation1);
    normalBuffer* nb1 = generateNormals(vb1);
    colorBuffer* cb1 = createColorBuffer(108);
    for(int i = 0; i < cb1->length; i++) cb1->inputColors[i] = 127;

    //cube3
    vertexBuffer* vb2 = createVertexBuffer(108);
    memcpy(vb2->inputVertices, correctedCube, sizeof(float) * 108);
    createTranslationMatrix(1.5, 0, 0, translation1);
    vertexBufferByMatrix(vb2, translation1);
    normalBuffer* nb2 = generateNormals(vb2);
    colorBuffer* cb2 = createColorBuffer(108);
    for(int i = 0; i < cb2->length; i++) cb2->inputColors[i] = 127;

    //lighting vector
    vec3 light; light.x = 0; light.y = 0; light.z = 1;
    
    scene* sc = createScene(3);
    sc->vertexBuffers[0] = vb0;
    sc->colorBuffers[0] =  cb0;
    sc->normalBuffers[0] = nb0;

    sc->vertexBuffers[1] = vb1;
    sc->colorBuffers[1] =  cb1;
    sc->normalBuffers[1] = nb1;

    sc->vertexBuffers[2] = vb2;
    sc->colorBuffers[2] =  cb2;
    sc->normalBuffers[2] = nb2;
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
        if(keystate[SDL_SCANCODE_W]){my += 0.2f;}
        if(keystate[SDL_SCANCODE_A]){mx -= 0.2f;}
        if(keystate[SDL_SCANCODE_S]){my -= 0.2f;}
        if(keystate[SDL_SCANCODE_D]){mx += 0.2f;}
        if(keystate[SDL_SCANCODE_LSHIFT]){ mz -= 0.2f;}
        if(keystate[SDL_SCANCODE_SPACE]){ mz += 0.2f;}

        if(keystate[SDL_SCANCODE_E]){angleX += 1.5;}
        if(keystate[SDL_SCANCODE_F]){angleX -= 1.5;}
        if(keystate[SDL_SCANCODE_R]){angleY += 1.5;}
        if(keystate[SDL_SCANCODE_G]){angleY -= 1.5;}

        
        //linear operations
        for(int j = 0; j < sc->maxLength; j++){
            vertexBuffer* vb = sc->vertexBuffers[j];
            colorBuffer* cb = sc->colorBuffers[j];
            normalBuffer* nb = sc->normalBuffers[j];
            for(int i = 0; i < vb->length; i += 3){
                vec4 temp;
                temp.x = vb->inputVertices[i];
                temp.y = vb->inputVertices[i + 1];
                temp.z = vb->inputVertices[i + 2];
                temp.w = 1.0f;

                //not ideal that this has to be a vec4
                vec3 normTemp;
                normTemp.x = nb->normals[i];
                normTemp.y = nb->normals[i + 1];
                normTemp.z = nb->normals[i + 2];
                //printf("%f, %f, %f\n", normTemp.x, normTemp.y, normTemp.z);
                createTranslationMatrix(mx, my, mz, translationMatrix);
                quaternion quatX = createRotationQuaternion(angleX, 1, 0, 0);
                quaternion quatY = createRotationQuaternion(angleY, 0, 1, 0);
                quaternion compositeQuaternion = multiplyQuaternion(quatX, quatY);
                vec3 tempdh = dehomogenizeVector(temp);
                rotateVectorViaQuaternion(&tempdh, compositeQuaternion);
                rotateVectorViaQuaternion(&normTemp, compositeQuaternion);
                temp = homogenizeVector(tempdh);

                vecByMatrix4x4(&temp, translationMatrix);

                normalizeVector(&normTemp);
                float lightScalar = dotProduct(normTemp, light);
                lightScalar += 1;
                //printf("%d", RGBClamp(cb->inputColors[i] * lightScalar));
                cb->colors[i] = RGBClamp(cb->inputColors[i] * lightScalar);
                cb->colors[i + 1] = RGBClamp(cb->inputColors[i + 1] * lightScalar);
                cb->colors[i + 2] = RGBClamp(cb->inputColors[i + 2] * lightScalar);
                
                vecByMatrix4x4(&temp, perspectiveProjectionMatrix);

                //perspective divide
                
                clampW(&temp);
                temp.x /= temp.w;
                temp.y /= temp.w;
                temp.z /= temp.w;
                temp.w = 1.0f;
                expandDepthRange(&temp, -1.0f, 2.0f);
                vecByMatrix4x4(&temp, screenSpaceMatrix);
                
                //create the temporary VBO
                vb->vertices[i] = temp.x;
                vb->vertices[i + 1] = temp.y;
                vb->vertices[i + 2] = temp.z;
                //printf("%f\n", temp.z);
                //printf("%i, %d, %d\n", cb->inputColors[i], cb->inputColors[i + 1], cb->inputColors[i + 2]);
            }
            rasterize(rc, vb, cb);
        }       
        
        SDL_UpdateTexture(texture, NULL, rc->frameBuffer, rc->width * 3);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        cleanRenderContext(rc);
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