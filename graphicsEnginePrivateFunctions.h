void perspectiveDivide(float* vb){
    //this abstration really isn't a big deal but i think it makes sense
}

normalBuffer* generateNormals(vertexBuffer* vb){
    normalBuffer* nb = malloc(sizeof(normalBuffer));
    nb->length = vb->length;
    nb->normals = malloc(sizeof(float) * nb->length);
    for(int i = 0; i < vb->length; i+=9){
        //and the naming scheme of vecX falls apart
        vec3 v1, v2, v3, edge1, edge2;
        v1.x = vb->vertices[i];
        v1.y = vb->vertices[i + 1];
        v1.z = vb->vertices[i + 2];

        v2.x = vb->vertices[i + 3];
        v2.y = vb->vertices[i + 4];
        v2.z = vb->vertices[i + 5];

        v3.x = vb->vertices[i + 6];
        v3.y = vb->vertices[i + 7];
        v3.z = vb->vertices[i + 8];

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
    m->centroid->x = x / (vb->length /3);
    m->centroid->y = y / (vb->length /3);
    m->centroid->z = z / (vb->length /3);
}

//might do this differently
//may be better to just have a matrix associated with an obj
void objectSpaceToWorldSpace(vertexBuffer* vb, int scalar){
    for(int i = 0; i< vb->length; i+= 3){
        vec4 temp;
        matrix4x4 mScalar;
        temp.x = vb->vertices[i];
        temp.y = vb->vertices[i + 1];
        temp.z = vb->vertices[i + 2];
        temp.w = 1;
        createScalingMatrix(scalar, mScalar);
        vecByMatrix4x4(&temp, mScalar);
        vb->vertices[i] = temp.x;
        vb->vertices[i + 1] = temp.y;
        vb->vertices[i + 2] = temp.z;
    }
}

int RGBClamp(float val){
    if(val < 0) return 0;
    if(val > 255) return 255;
    return (int)val;
}