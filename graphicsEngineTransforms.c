//this may be best as designated for user input
//there is a reason they use programmable vertex shaders
//this could be quite restrictive
void transform(transformSpec* ts, scene* sc, vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb){
    matrix4x4 rotationMatrixX, rotationMatrixY, rotationMatrixZ, translationMatrix, scalingMatrix, perspectiveProjectionMatrix, screenSpaceMatrix;
    vec3 light; light.x = 0; light.y = -1; light.z = 0;
    createPerspectiveProjectionMatrix(45.0, 1.0, 10.0, 1000.0/700.0, perspectiveProjectionMatrix);
    for(int i = 0; i < vb->length; i += 3){ 
        vec4 temp;
        temp.x = vb->inputVertices[i];
        temp.y = vb->inputVertices[i + 1];
        temp.z = vb->inputVertices[i + 2];
        
        temp.w = 1.0f;

        vec3 normTemp;
        normTemp.x = nb->normals[i];
        normTemp.y = nb->normals[i + 1];
        normTemp.z = nb->normals[i + 2];

        vec4 normTempH = homogenizeVector(normTemp);
        createRotationMatrixX(ts->rotateX, rotationMatrixX);
        createRotationMatrixY(ts->rotateY, rotationMatrixY);
        createRotationMatrixZ(ts->rotateZ, rotationMatrixZ);
        vecByMatrix4x4(&temp, rotationMatrixX);
        vecByMatrix4x4(&temp, rotationMatrixY);
        vecByMatrix4x4(&temp, rotationMatrixZ);
        vecByMatrix4x4(&normTempH, rotationMatrixX);
        vecByMatrix4x4(&normTempH, rotationMatrixY);
        vecByMatrix4x4(&normTempH, rotationMatrixZ);

        createTranslationMatrix(ts->translateX, ts->translateY, ts->translateZ, translationMatrix);
        vecByMatrix4x4(&temp, translationMatrix);
        

        normTemp = dehomogenizeVector(normTempH);
        normalizeVector(&normTemp);
        float lightScalar = dotProduct(normTemp, light);
        lightScalar += 1;
        if(dotProduct(normTemp, *sc->cameraVector) < -0.5){
            vb->indexBuffer[i/3] = 0;
        }else vb->indexBuffer[i/3] = 1;
        cb->colors[i] = RGBClamp(cb->inputColors[i] * lightScalar);
        cb->colors[i + 1] = RGBClamp(cb->inputColors[i + 1] * lightScalar);
        cb->colors[i + 2] = RGBClamp(cb->inputColors[i + 2] * lightScalar);
        
        perspectiveProjection(&temp, perspectiveProjectionMatrix);
        perspectiveDivide(&temp);
        NDCToScreenSpace(&temp, 1.0, 100.0, 700, 1000);
        //create the temporary VBO
        vb->vertices[i] = temp.x;
        vb->vertices[i + 1] = temp.y;
        //we are now w-buffering, maybe a more thorough implementation would be good
        vb->vertices[i + 2] = temp.w * 25;
        // printf("%f\n", temp.w);
    }
}