//I will do literally anything but use a makefile i guess
//clang SDL_helper.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2
#include <SDL.h>
#include <string.h>
#include "raster/rasterizer.c"
#include "3Dmath/operations.c"
#include "engineTypes.h"
#include "graphicsEngineFunctions.c"
#include "commonCoords.c"
#include "OBJParser/parser.c"

#define TRANSLATION_SPEED 0.5f
#define ANGLE_INCREMENT 1.0f
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


    object* obj = parseNoMTL("forestPondFixed.obj");
    vertexBuffer* vb0 = createVertexBuffer(obj->faceCount * 9);
    memcpy(vb0->inputVertices, obj->faces, sizeof(float) * obj->faceCount * 9);
    printf("%d", obj->faceCount);
    matrix4x4 prescale, center;
    

    createScalingMatrix(2, prescale);
    vertexBufferByMatrix(vb0, prescale);
    normalBuffer* nb0 = generateNormals(vb0);
    colorBuffer* cb0 = createColorBuffer(obj->faceCount * 9);
    for(int i = 0; i < cb0->length; i++) cb0->inputColors[i] = 127;
    mesh* mesh0 = meshify(vb0, cb0, nb0);
    calculateCentroid(mesh0);
    createTranslationMatrix(mesh0->centroid->x,mesh0->centroid->y, mesh0->centroid->z, center);
    vertexBufferByMatrix(vb0, center);
    delete(obj);


    //lighting vector
    vec3 light; light.x = 0; light.y = -1; light.z = 0;
    
    //scene
    scene* sc = createScene(1);
    sc->meshes[0] = mesh0;

    //movement variables
    float mx = 0;
    float my = 0;
    float mz = 0;
    float angleX = 180.0;
    float angleY = 0.0;
    float angleZ = 0.0;

    //update matrices
    matrix4x4 rotationMatrixX, rotationMatrixY, rotationMatrixZ, translationMatrix, scalingMatrix, perspectiveProjectionMatrix, screenSpaceMatrix;
    matrix4x4 rodMatrix;
    createRotationMatrix(0.0, 0.0, 0.0, 0.0, rodMatrix);
    createPerspectiveProjectionMatrix(45.0, 1.0, 10.0, 1000.0/700.0, perspectiveProjectionMatrix);
    createScalingMatrix(0.5f, scalingMatrix);
    createTranslationMatrix(mx, my, mz, translationMatrix);
    createRotationMatrixX(angleX, rotationMatrixX);
    createRotationMatrixY(angleY, rotationMatrixY);
    createRotationMatrixZ(angleZ, rotationMatrixZ);
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
        if(keystate[SDL_SCANCODE_S]){my += TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_A]){mx -= TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_W]){my -= TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_D]){mx += TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_LSHIFT]){ mz -= TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_SPACE]){ mz += TRANSLATION_SPEED;}

        if(keystate[SDL_SCANCODE_E]){angleX += ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_F]){angleX -= ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_R]){angleY += ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_G]){angleY -= ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_T]){angleZ += ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_H]){angleZ -= ANGLE_INCREMENT;}
        
        for(int j = 0; j < sc->length; j++){
            vertexBuffer* vb = sc->meshes[j]->vb;
            colorBuffer* cb = sc->meshes[j]->cb;
            normalBuffer* nb = sc->meshes[j]->nb;
            for(int i = 0; i < vb->length; i += 3){
                
                vec4 temp;
                temp.x = vb->inputVertices[i];
                temp.y = vb->inputVertices[i + 1];
                temp.z = vb->inputVertices[i + 2];
                
                temp.w = 1.0f;

                vec3 normTemp;
                normTemp.x = nb->normals[i];
                normTemp.y = nb->normals[i + 1];
                normTemp.z = nb->normals[i + 2];

                vec4 normTempH = homogenizeVector(normTemp);
                createRotationMatrixX(angleX, rotationMatrixX);
                createRotationMatrixY(angleY, rotationMatrixY);
                createRotationMatrixZ(angleZ, rotationMatrixZ);
                vecByMatrix4x4(&temp, rotationMatrixX);
                vecByMatrix4x4(&temp, rotationMatrixY);
                vecByMatrix4x4(&temp, rotationMatrixZ);
                vecByMatrix4x4(&normTempH, rotationMatrixX);
                vecByMatrix4x4(&normTempH, rotationMatrixY);
                vecByMatrix4x4(&normTempH, rotationMatrixZ);

                createTranslationMatrix(mx, my, mz, translationMatrix);
                vecByMatrix4x4(&temp, translationMatrix);
               

                normTemp = dehomogenizeVector(normTempH);
                normalizeVector(&normTemp);
                float lightScalar = dotProduct(normTemp, light);
                lightScalar += 1;
                if(dotProduct(normTemp, *sc->cameraVector) < -0.5){
                    vb->indexBuffer[i/3] = 0;
                }else vb->indexBuffer[i/3] = 1;
                cb->colors[i] = RGBClamp(cb->inputColors[i] * lightScalar);
                cb->colors[i + 1] = RGBClamp(cb->inputColors[i + 1] * lightScalar);
                cb->colors[i + 2] = RGBClamp(cb->inputColors[i + 2] * lightScalar);
                
                perspectiveProjection(&temp, perspectiveProjectionMatrix);
                perspectiveDivide(&temp);
                NDCToScreenSpace(&temp, 1.0, 100.0, 700, 1000);
                //create the temporary VBO
                vb->vertices[i] = temp.x;
                vb->vertices[i + 1] = temp.y;
                //we are now w-buffering, maybe a more thorough implementation would be good
                vb->vertices[i + 2] = temp.w * 25;
                // printf("%f\n", temp.w);
            }
            rasterize(rc, vb, cb);
        }       
        SDL_UpdateTexture(texture, NULL, rc->frameBuffer, rc->width * 3);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        cleanRenderContext(rc);
    }
    deleteRenderContext(rc);
    //dont forget to free the vertex buffers
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}