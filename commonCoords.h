float normalizedTriangle[] = {
        //9 values
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
    };

float normalizedCubeVertices[] = {
        //108 values
        // Bottom face (two triangles)
        -0.5f, -0.5f, -0.5f,  // Triangle 1, Vertex 1
        0.5f, -0.5f, -0.5f,  // Triangle 1, Vertex 2
        0.5f, -0.5f,  0.5f,  // Triangle 1, Vertex 3
        
        0.5f, -0.5f,  0.5f,  // Triangle 2, Vertex 1
        -0.5f, -0.5f,  0.5f,  // Triangle 2, Vertex 2
        -0.5f, -0.5f, -0.5f,  // Triangle 2, Vertex 3

        // Top face (two triangles)
        -0.5f,  0.5f, -0.5f,  // Triangle 1, Vertex 1
        0.5f,  0.5f, -0.5f,  // Triangle 1, Vertex 2
        0.5f,  0.5f,  0.5f,  // Triangle 1, Vertex 3
        
        0.5f,  0.5f,  0.5f,  // Triangle 2, Vertex 1
        -0.5f,  0.5f,  0.5f,  // Triangle 2, Vertex 2
        -0.5f,  0.5f, -0.5f,  // Triangle 2, Vertex 3

        // Front face (two triangles)
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        // Back face (two triangles)
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,

        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // Left face (two triangles)
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // Right face (two triangles)
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f
    };

float correctedCube[] = {
     // Bottom face (two triangles) - now top face
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f,  0.5f,  // Triangle 1, Vertex 1
     0.5f, 0.5f, -0.5f,  // Triangle 1, Vertex 2

     0.5f, 0.5f,  0.5f,
     -0.5f, 0.5f, -0.5f,   // Triangle 2, Vertex 1
    -0.5f, 0.5f,  0.5f,  // Triangle 2, Vertex 2
     // Triangle 2, Vertex 3

    // Top face (two triangles) - now bottom face
    -0.5f, -0.5f, -0.5f, // Triangle 1, Vertex 1
     0.5f, -0.5f, -0.5f, // Triangle 1, Vertex 2
     0.5f, -0.5f,  0.5f, // Triangle 1, Vertex 3


    //this is part of the back face
     0.5f, -0.5f,  0.5f, // Triangle 2, Vertex 1
    -0.5f, -0.5f,  0.5f, // Triangle 2, Vertex 2
    -0.5f, -0.5f, -0.5f, // Triangle 2, Vertex 3

    // back face (two triangles) - remains the same, just swap top/bottom in description
    -0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     

     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
   

    // front face (two triangles) - remains the same, just swap top/bottom in description
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,

     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,

    // Left face (two triangles) - remains the same, just swap top/bottom in description
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    

    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // Right face (two triangles) - remains the same, just swap top/bottom in description
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,

     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f
};

float partialCube[] = {
        // -0.5f, -0.5f, -0.5f,
        // -0.5f, -0.5f,  0.5f,
        // -0.5f,  0.5f,  0.5f,

        // -0.5f,  0.5f,  0.5f,
        // -0.5f,  0.5f, -0.5f,
        // -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,

        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        // -0.5f, -0.5f,  0.5f,
        // 0.5f, -0.5f,  0.5f,
        // 0.5f,  0.5f,  0.5f,

        // 0.5f,  0.5f,  0.5f,
        // -0.5f,  0.5f,  0.5f,
        // -0.5f, -0.5f,  0.5f,

    };