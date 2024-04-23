void perspectiveDivide(float* vb){
    //this abstration really isn't a big deal but i think it makes sense
}

normalBuffer* generateNormals(vertexBuffer* vb){
    normalBuffer* nb = malloc(sizeof(normalBuffer));
    nb->length = vb->length;
    nb->normals = malloc(sizeof(float) * nb->length);
    for(int i = 0; i < vb->length; i+=9){
        vec3 vec1;
        vec3 vec2;
        vec1.x = vb->vertices[i];
        vec1.y = vb->vertices[i + 1];
        vec1.z = vb->vertices[i + 2];

        vec2.x = vb->vertices[i + 3];
        vec2.y = vb->vertices[i + 4];
        vec2.z = vb->vertices[i + 5];

        vec3 vecResult = crossProduct(vec1, vec2);

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

//might do this differently
//may be better to just have a matrix associated with an obj
void objectSpaceToWolrdSpace(vertexBuffer* vb, int scalar){
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