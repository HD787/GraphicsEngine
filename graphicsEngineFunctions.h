boundingBox* generateBoundingBox(vertexBuffer* vb){
    boundingBox* temp = malloc(sizeof(boundingBox));
    temp->maxX = -1000000;
    temp->minX = -1000000;
    temp->maxY = -1000000;
    temp->minY = -1000000;
    temp->maxZ = -1000000;
    temp->minZ = -1000000;
    for(int i = 0; i < vb->length; i += 3){
        if(vb->vertices[i] > temp->maxX || temp->maxX == -1000000) temp->maxX = vb->vertices[i];
        if(vb->vertices[i] > temp->minX || temp->minX == -1000000) temp->minX = vb->vertices[i];
        if(vb->vertices[i + 1] > temp->maxY || temp->maxY == -1000000) temp->maxY = vb->vertices[i + 1];
        if(vb->vertices[i + 1] > temp->minY || temp->minY == -1000000) temp->minY = vb->vertices[i + 1];
        if(vb->vertices[i + 2] > temp->maxZ || temp->maxZ == -1000000) temp->maxZ = vb->vertices[i + 2];
        if(vb->vertices[i + 2] > temp->minZ || temp->minZ == -1000000) temp->minZ = vb->vertices[i + 2];
    }
    return temp;
}

void generateBoundingBoxDEPRECATED(mesh* msh){
    vertexBuffer* vb = msh->vb;
    boundingBox* temp = malloc(sizeof(boundingBox));
    temp->maxX = -1000000;
    temp->minX = -1000000;
    temp->maxY = -1000000;
    temp->minY = -1000000;
    temp->maxZ = -1000000;
    temp->minZ = -1000000;
    for(int i = 0; i < vb->length; i += 3){
        if(vb->vertices[i] > temp->maxX || temp->maxX == -1000000) temp->maxX = vb->vertices[i];
        if(vb->vertices[i] > temp->minX || temp->minX == -1000000) temp->minX = vb->vertices[i];
        if(vb->vertices[i + 1] > temp->maxY || temp->maxY == -1000000) temp->maxY = vb->vertices[i + 1];
        if(vb->vertices[i + 1] > temp->minY || temp->minY == -1000000) temp->minY = vb->vertices[i + 1];
        if(vb->vertices[i + 2] > temp->maxZ || temp->maxZ == -1000000) temp->maxZ = vb->vertices[i + 2];
        if(vb->vertices[i + 2] > temp->minZ || temp->minZ == -1000000) temp->minZ = vb->vertices[i + 2];
    }
    msh->bb = temp;
}






normalBuffer* generateNormals(vertexBuffer* vb){
    normalBuffer* nb = malloc(sizeof(normalBuffer));
    nb->length = vb->length;
    nb->normals = malloc(sizeof(float) * nb->length);
    for(int i = 0; i < vb->length; i+=9){
        //and the naming scheme of vecX falls apart
        vec3 v1, v2, v3, edge1, edge2;
        v1.x = vb->inputVertices[i];
        v1.y = vb->inputVertices[i + 1];
        v1.z = vb->inputVertices[i + 2];

        v2.x = vb->inputVertices[i + 3];
        v2.y = vb->inputVertices[i + 4];
        v2.z = vb->inputVertices[i + 5];

        v3.x = vb->inputVertices[i + 6];
        v3.y = vb->inputVertices[i + 7];
        v3.z = vb->inputVertices[i + 8];

        edge1.x = v1.x - v2.x;
        edge1.y = v1.y - v2.y;
        edge1.z = v1.z - v2.z;

        edge2.x = v3.x - v1.x;
        edge2.y = v3.y - v1.y;
        edge2.z = v3.z - v1.z;


        vec3 vecResult = crossProduct(edge2, edge1);
        normalizeVector(&vecResult);

        nb->normals[i] = vecResult.x;
        nb->normals[i + 1] = vecResult.y;
        nb->normals[i + 2] = vecResult.z;

        nb->normals[i + 3] = vecResult.x;
        nb->normals[i + 4] = vecResult.y;
        nb->normals[i + 5] = vecResult.z;

        nb->normals[i + 6] = vecResult.x;
        nb->normals[i + 7] = vecResult.y;
        nb->normals[i + 8] = vecResult.z;
    }
    return nb;
}

void calculateCentroid(mesh* m){
    vertexBuffer* vb = m->vb;
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    for(int i = 0; i < vb->length; i += 3){
        x += vb->vertices[i];
        y += vb->vertices[i + 1];
        z += vb->vertices[i + 2];
    }
    m->centroid->x = x / (vb->length / 3);
    m->centroid->y = y / (vb->length / 3);
    m->centroid->z = z / (vb->length / 3);
}

//might do this differently
//may be better to just have a matrix associated with an obj
void objectSpaceToWorldSpace(vertexBuffer* vb, int scalar){
    for(int i = 0; i< vb->length; i+= 3){
        vec4 temp;
        matrix4x4 mScalar;
        temp.x = vb->inputVertices[i];
        temp.y = vb->inputVertices[i + 1];
        temp.z = vb->inputVertices[i + 2];
        temp.w = 1;
        createScalingMatrix(scalar, mScalar);
        vecByMatrix4x4(&temp, mScalar);
        vb->inputVertices[i] = temp.x;
        vb->inputVertices[i + 1] = temp.y;
        vb->inputVertices[i + 2] = temp.z;
    }
}

void perspectiveProjection(vec4* v, matrix4x4 matrix){
    if(v->z <= 1.0f){
        v->w = v->z;
        return;
    }
    vecByMatrix4x4(v, matrix);
}

void perspectiveDivide(vec4* v){
    if(v->w <= 1.0f){
        return; 
    }
    v->x /= v->w;
    v->y /= v->w;
    v->z /= v->w;
}

void NDCToScreenSpace(vec4* v, float nearPlane, float farPlane, int height, int width){
    v->x = ((v->x + 1) * width) / 2;
    v->y = ((v->y + 1) * height) / 2;
    v->z = ((v->z + 1) * (farPlane - nearPlane) / 2) + nearPlane;
}

int RGBClamp(float val){
    if(val < 0) return 0;
    if(val > 255) return 255;
    return (int)val;
}

void vertexBufferByMatrix(vertexBuffer* vb, matrix4x4 matrix){
    for(int i = 0; i < vb->length; i += 3){
        vec4 temp;
        temp.x = vb->inputVertices[i];
        temp.y = vb->inputVertices[i + 1];
        temp.z = vb->inputVertices[i + 2];
        temp.w = 1.0;
        vecByMatrix4x4(&temp, matrix);
        vb->inputVertices[i] = temp.x;
        vb->inputVertices[i + 1] = temp.y;
        vb->inputVertices[i + 2] = temp.z;
    }
}

mesh* meshify(vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb){
    mesh* temp = malloc(sizeof(mesh));
    temp->vb = vb;
    temp->cb = cb;
    temp->nb = nb;
    temp->bb = generateBoundingBox(vb);
    temp->centroid = malloc(sizeof(vec3));
    return temp;
}

void deleteMesh(mesh* msh){
    deleteColorBuffer(msh->cb);
    deleteVertexBuffer(msh->vb);
    free(msh->nb->normals);
    free(msh->nb);
    free(msh);
}


scene* createScene(int size){
    scene* temp = malloc(sizeof(scene));
    temp->length = size;
    temp->meshes = malloc(sizeof(mesh*) * temp->length);
    temp->indexBuffer = calloc(size, sizeof(char));
    vec3* vec = malloc(sizeof(vec3));
    vec->x = 0;
    vec->y = 0;
    vec->z = 1;
    temp->cameraVector = vec;
    return temp;
}
void deleteScene(scene* sc){
    free(sc->cameraVector);
    free(sc->indexBuffer);
    for(int i = 0; i < sc->length; i++){
        deleteMesh(sc->meshes[i]);
    }
    free(sc->meshes);
    free(sc);
}

void cleanScene(scene* sc){
    //thinking i won't do this as its less efficient and the benefits are questionable
    // for(int i = 0; i < sc-> currentLength; i++){
    //     sc->vertexBuffers[i] = NULL;
    //     sc->normalBuffers[i] = NULL;
    //     sc->colorBuffers[i] = NULL;
    // }
    //sc->currentLength = 0;
}

//not sure what this will look like yet
//but its for frustum culling
int frustumCheck(mesh* msh){
    boundingBox* bb = msh->bb;
    if(bb->maxZ < 0 && bb->maxZ < 0) return 1;
    return 0;
}