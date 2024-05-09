typedef struct{
    float maxX;
    float minX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;
}boundingBox;


//idk about this one atm
typedef struct{
    vertexBuffer* vb;
    colorBuffer* cb;
    normalBuffer* nb;
    vec3* centroid;
    boundingBox* bb;
    //this could be useful but also lowkey we can just rotate before translating a lot of the time
    matrix4x4 worldSpaceMatrix;
}mesh;

typedef struct{
    mesh** meshes;
    char* indexBuffer;
    vec3* cameraVector;
    int length;
    //int currentLength;
}scene;