float normalizedTriangle[] = {
        //9 values
        .5f, .75f, .5f, // top right
        .25f, .25f, .5f, // bottom
        .25f, .75f, .5f // top left
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