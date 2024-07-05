//I will do literally anything but use a makefile i guess
//clang SDL_externalTransforms.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2
//gcc SDL_externalTransforms.c -o main -I/opt/homebrew/cellar/sdl2/2.28.4/include/sdl2 -L/opt/homebrew/cellar/sdl2/2.28.4/lib -lSDL2

#include <SDL.h>
#include <string.h>
#include "raster/rasterizer.c"
#include "3Dmath/operations.c"
#include "engineTypes.h"
#include "graphicsEngineFunctions.c"
#include "graphicsEngineTransforms.c"
//#include "NEON_graphicsEngineTransforms.c"
#include "commonCoords.c"
#include "OBJParser/parser.c"

#define TRANSLATION_SPEED 0.5f
#define ANGLE_INCREMENT 1.0f
int main(){


    renderContext* rc = createRenderContext(900, 700);
    

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
    transformSpec* transformations = malloc(sizeof(transformSpec));
    transformations->translateX = 0;
    transformations->translateY = 0;
    transformations->translateZ = 20;
    transformations->rotateX = 180.0;
    transformations->rotateY = 0.0;
    transformations->rotateZ = 0.0;

    //update matrices
    matrix4x4 rotationMatrixX, rotationMatrixY, rotationMatrixZ, translationMatrix, scalingMatrix, perspectiveProjectionMatrix, screenSpaceMatrix;
    matrix4x4 rodMatrix;
    createRotationMatrix(0.0, 0.0, 0.0, 0.0, rodMatrix);
    

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

        if(keystate[SDL_SCANCODE_S]){transformations->translateY += TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_A]){transformations->translateX -= TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_W]){transformations->translateY -= TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_D]){transformations->translateX += TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_LSHIFT]){transformations->translateZ -= TRANSLATION_SPEED;}
        if(keystate[SDL_SCANCODE_SPACE]){transformations->translateZ += TRANSLATION_SPEED;}

        if(keystate[SDL_SCANCODE_E]){transformations->rotateX += ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_F]){transformations->rotateX -= ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_R]){transformations->rotateY += ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_G]){transformations->rotateY -= ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_T]){transformations->rotateZ += ANGLE_INCREMENT;}
        if(keystate[SDL_SCANCODE_H]){transformations->rotateZ -= ANGLE_INCREMENT;}
        
        for(int j = 0; j < sc->length; j++){
            vertexBuffer* vb = sc->meshes[j]->vb;
            colorBuffer* cb = sc->meshes[j]->cb;
            normalBuffer* nb = sc->meshes[j]->nb;
            transform(transformations, sc, vb, cb, nb);
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