//emcc -o WASM_forestDemo.js WASM_forestDemo.c -s EXPORTED_FUNCTIONS='["_initialize", "_renderPass", "_getFrameBuffer", "_deleteWebContext", "_initializeFromObj"]' -s --preload-file forestpondFIXED.obj
#include <string.h>
#include <emscripten.h>
#include "raster/rasterizer.c"
#include "3Dmath/operations.c"
#include "engineTypes.h"
#include "graphicsEngineFunctions.c"
#include "graphicsEngineTransforms.c"

#include "commonCoords.c"
#include "OBJParser/parser.c"


EMSCRIPTEN_KEEPALIVE
webContext* initialize(int height, int width){
    webContext* wc = malloc(sizeof(webContext));
    wc->rc = createRenderContext_RGBA(height, width);

    vertexBuffer* vb0 = createVertexBuffer(108);
    memcpy(vb0->inputVertices, correctedCube, sizeof(float) * 108);
    normalBuffer* nb0 = generateNormals(vb0);
    colorBuffer* cb0 = createColorBuffer(108);
    for(int i = 0; i < cb0->length; i++) cb0->inputColors[i] = 127;
    mesh* mesh0 = meshify(vb0, cb0, nb0);

    wc->sc = createScene(1);
    wc->sc->meshes[0] = mesh0;

    wc->ts = malloc(sizeof(transformSpec));
    wc->ts->translateX = 0;
    wc->ts->translateY = 0;
    wc->ts->translateZ = 5;
    wc->ts->rotateX = 0.0;
    wc->ts->rotateY = 0.0;
    wc->ts->rotateZ = 0.0;
    return wc;
}

EMSCRIPTEN_KEEPALIVE
webContext* initializeFromObj(int height, int width){
    webContext* wc = malloc(sizeof(webContext));
    object* obj = parseNoMTL("forestPondFixed.obj");
    vertexBuffer* vb0 = createVertexBuffer(obj->faceCount * 9);
    memcpy(vb0->inputVertices, obj->faces, sizeof(float) * obj->faceCount * 9);

    normalBuffer* nb0 = generateNormals(vb0);
    colorBuffer* cb0 = createColorBuffer(obj->faceCount * 9);
    for(int i = 0; i < cb0->length; i++) cb0->inputColors[i] = 127;
    mesh* mesh0 = meshify(vb0, cb0, nb0);

    scene* sc = createScene(1);
    sc->meshes[0] = mesh0;
    //this is currently hardcoded in the transforms, take some time to fix all the demos
    sc->lightVector = malloc(sizeof(vec3)); 
    sc->lightVector->x = 0; sc->lightVector->y = 0; sc->lightVector->z = -1;


    wc->ts = malloc(sizeof(transformSpec));
    wc->ts->translateX = 0;
    wc->ts->translateY = 0;
    wc->ts->translateZ = 5;
    wc->ts->rotateX = 0.0;
    wc->ts->rotateY = 0.0;
    wc->ts->rotateZ = 0.0;
    
    return wc;
}

EMSCRIPTEN_KEEPALIVE
void deleteWebContext(webContext* wc){
    deleteRenderContext(wc->rc);
    deleteScene(wc->sc);
    free(wc->ts);
    free(wc);
}

EMSCRIPTEN_KEEPALIVE
void renderPass(webContext* wc, float translateX, float translateY, float translateZ, float rotateX, float rotateY, float rotateZ){
    cleanRenderContext_RGBA(wc->rc);
    wc->ts->translateX += translateX;
    wc->ts->translateY += translateY;
    wc->ts->translateZ += translateZ;
    wc->ts->rotateX += rotateX;
    wc->ts->rotateY += rotateY;
    wc->ts->rotateZ += rotateZ;
    transform(wc->rc, wc->ts, wc->sc, wc->sc->meshes[0]->vb, wc->sc->meshes[0]->cb, wc->sc->meshes[0]->nb);
    rasterize_RGBA(wc->rc, wc->sc->meshes[0]->vb, wc->sc->meshes[0]->cb);
}


EMSCRIPTEN_KEEPALIVE
uint8_t* getFrameBuffer(webContext* wc){
    return wc->rc->frameBuffer;
}



// int main(){
//     webContext* wc = initialize(100, 100);
//     renderPass(wc, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//     renderPass(wc, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//     // unsigned char* ptr = getFrameBuffer(wc);
//     renderPass(wc, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//     for(int i = 0; i < wc->sc->meshes[0]->vb->length; i++){
//         //if(wc->sc->meshes[0]->vb->inputVertices[i] == -1000000)
//         printf(" %f : %p\n ", wc->sc->meshes[0]->vb->inputVertices[i], wc->sc->meshes[0]->vb->inputVertices + i);
//     }
// } 