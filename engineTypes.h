typedef struct{
    float maxX;
    float minX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;
}boundingBox;

typedef struct{
    vertexBuffer** vertexBuffers;
    colorBuffer** colorBuffers;
    normalBuffer** normalBuffers;
    char* indexBuffer;
    int maxLength;
    //int currentLength;
} scene;

//idk about this one atm
typedef struct{
    vertexBuffer* vb;
    colorBuffer* cb;
    normalBuffer* nb;
    vec3* centroid;
     //this could be useful but also lowkey we can just rotate before translating a lot of the time
    matrix4x4 worldSpaceMatrix;
    boundingBox bb;
}mesh;