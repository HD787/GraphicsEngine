typedef struct{
    float maxX;
    float minX;
    float maxY;
    float minY;
    float maxZ;
    float minZ;
}boundingBox;

typedef struct{
    vertexBuffer* vb;
    //this could be useful but also lowkey we can just rotate before translating a lot of the time
    vec3* centroid;
    matrix4x4 worldSpaceMatrix;
}mesh;