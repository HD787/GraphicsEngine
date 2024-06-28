#include <string.h>
#include "raster/rasterizer.c"
#include "3Dmath/operations.c"
#include "engineTypes.h"
#include "graphicsEngineFunctions.c"
#include "graphicsEngineTransforms.c"

#include "commonCoords.c"
#include "OBJParser/parser.c"


webContext* initialize(int height, int width){
    webContext* wc = malloc(sizeof(webContext));
    wc->rc = createRenderContext(height, width);

    object* obj = parseNoMTL("forestPondFixed.obj");
    vertexBuffer* vb0 = createVertexBuffer(obj->faceCount * 9);
    memcpy(vb0->inputVertices, obj->faces, sizeof(float) * obj->faceCount * 9);
    matrix4x4 prescale, center;
    
    createScalingMatrix(2, prescale);
    vertexBufferByMatrix(vb0, prescale);
    createTranslationMatrix(mesh0->centroid->x,mesh0->centroid->y, mesh0->centroid->z, center);
    vertexBufferByMatrix(vb0, center);

    normalBuffer* nb0 = generateNormals(vb0);
    colorBuffer* cb0 = createColorBuffer(obj->faceCount * 9);
    for(int i = 0; i < cb0->length; i++) cb0->inputColors[i] = 127;
    mesh* mesh0 = meshify(vb0, cb0, nb0);
    calculateCentroid(mesh0);
    
    delete(obj);

    wc->sc = createScene(1);
    wc->sc->meshes[0] = mesh0;

    wc->ts = malloc(sizeof(transformSpec));
    transformations->translateX = 0;
    transformations->translateY = 0;
    transformations->translateZ = 20;
    transformations->rotateX = 180.0;
    transformations->rotateY = 0.0;
    transformations->rotateZ = 0.0;
}