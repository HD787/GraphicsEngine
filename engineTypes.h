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
    vec3* lightVector;
    int length;
    //int currentLength;
}scene;

typedef struct{
    float translateX;
    float translateY;
    float translateZ;
    float rotateX;
    float rotateY;
    float rotateZ;
    float explodeScalar;
    float* rotationX;
    float* rotationY;
    float* rotationZ;
    float* translation;
    float* perspectiveProjection;
    //might keep this for misc matrices
    float* matrixArray;
    int matrixArrayLength;
}transformSpec;

//im unsure about the idea of having structs that are

//this contains all data in one struct
//ideally this should simplify the javaScript part
typedef struct{
    scene* sc;
    renderContext* rc;
    transformSpec* ts;
}webContext;

//this is exclusive to OpenCL implementations

// typedef struct{
//     void* context;
//     void* deviceId;
//     void*program;
//     void* commandQueue;
//     void* kernel;
//     void* vertexOutput;
//     void* colorOutput;

//     // cl_context context;
//     // cl_device_id deviceId;
//     // cl_program program;
//     // cl_command_queue commandQueue;
//     // cl_kernel kernel;
//     // cl_mem vertexOutput;
//     // cl_mem colorOutput;
// }openClResources;