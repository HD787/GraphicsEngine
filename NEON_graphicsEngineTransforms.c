#include <arm_neon.h>
//this may be best as designated for user input
//there is a reason they use programmable vertex shaders
//this could be quite restrictive
void transform(transformSpec* ts, scene* sc, vertexBuffer* vb, colorBuffer* cb, normalBuffer* nb){
    matrix4x4 rotationMatrixX, rotationMatrixY, rotationMatrixZ, translationMatrix, scalingMatrix, perspectiveProjectionMatrix, screenSpaceMatrix;
    createRotationMatrixX_COLUMNMAJOR(ts->rotateX, rotationMatrixX);
    createRotationMatrixY_COLUMNMAJOR(ts->rotateY, rotationMatrixY);
    createRotationMatrixZ_COLUMNMAJOR(ts->rotateZ, rotationMatrixZ);
    createTranslationMatrix_COLUMNMAJOR(ts->translateX, ts->translateY, ts->translateZ, translationMatrix);
    createPerspectiveProjectionMatrix_COLUMNMAJOR(45.0, 1.0, 10.0, 1000.0/700.0, perspectiveProjectionMatrix);
    //load matrices into v registers
    float32x4_t rotationMatrixXRegisters[4];
    float32x4_t rotationMatrixYRegisters[4];
    float32x4_t rotationMatrixZRegisters[4];
    float32x4_t translationMatrixRegisters[4];
    float32x4_t perspectiveMatrixRegisters[4];
    for(int i = 0; i < 4; i++) rotationMatrixXRegisters[i] = vld1q_f32(&rotationMatrixX[i * 4]);
    for(int i = 0; i < 4; i++) rotationMatrixYRegisters[i] = vld1q_f32(&rotationMatrixY[i * 4]);
    for(int i = 0; i < 4; i++) rotationMatrixZRegisters[i] = vld1q_f32(&rotationMatrixZ[i * 4]);
    for(int i = 0; i < 4; i++) translationMatrixRegisters[i] = vld1q_f32(&translationMatrix[i * 4]);
    for(int i = 0; i < 4; i++) perspectiveMatrixRegisters[i] = vld1q_f32(&perspectiveProjectionMatrix[i * 4]);
    float32x4_t cameraVector = {sc->cameraVector->x, sc->cameraVector->y, sc->cameraVector->z, 0.0f};
    float32x4_t lightVector = {0.0f, -1.0f, 0.0f, 0.0f};
    for(int i = 0; i < vb->length; i += 3){
        //these vectors would be multiplied against the COLUMNS of a matrix
        //e.g. the first column is multiplied against all the x values, the second against the y values and so on
        //load the coordinate vectors into the SIMD registers
        float32x4_t xVecReg = vdupq_n_f32(vb->inputVertices[i]);
        float32x4_t yVecReg = vdupq_n_f32(vb->inputVertices[i + 1]);
        float32x4_t zVecReg = vdupq_n_f32(vb->inputVertices[i + 2]);
        float32x4_t wVecReg = vdupq_n_f32(1.0f);
        float32x4_t vecResult = vdupq_n_f32(0.0f);
        //load the normal vectors
        float32x4_t xNormReg = vdupq_n_f32(nb->normals[i]);
        float32x4_t yNormReg = vdupq_n_f32(nb->normals[i + 1]);
        float32x4_t zNormReg = vdupq_n_f32(nb->normals[i + 2]);
        float32x4_t wNormReg = vdupq_n_f32(1.0f);
        float32x4_t normResult = vdupq_n_f32(0.0f);
        //multiply the vectors and accumulate values in the vecResult vector
        //x rotation matrix
        vecResult = vmlaq_f32(vecResult, xVecReg, rotationMatrixXRegisters[0]);
        vecResult = vmlaq_f32(vecResult, yVecReg, rotationMatrixXRegisters[1]);
        vecResult = vmlaq_f32(vecResult, zVecReg, rotationMatrixXRegisters[2]);
        vecResult = vmlaq_f32(vecResult, wVecReg, rotationMatrixXRegisters[3]);
        //normals
        normResult = vmlaq_f32(normResult, xNormReg, rotationMatrixXRegisters[0]);
        normResult = vmlaq_f32(normResult, yNormReg, rotationMatrixXRegisters[1]);
        normResult = vmlaq_f32(normResult, zNormReg, rotationMatrixXRegisters[2]);
        normResult = vmlaq_f32(normResult, wNormReg, rotationMatrixXRegisters[3]);
        //reset our registers to contain latest transformed values
        xVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 0));
        yVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 1));
        zVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 2));
        wVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 3));
        vecResult = vdupq_n_f32(0.0f);
        //normals
        xNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 0));
        yNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 1));
        zNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 2));
        wNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 3));
        normResult = vdupq_n_f32(0.0f);

        //y rotation matrix
        vecResult = vmlaq_f32(vecResult, xVecReg, rotationMatrixYRegisters[0]);
        vecResult = vmlaq_f32(vecResult, yVecReg, rotationMatrixYRegisters[1]);
        vecResult = vmlaq_f32(vecResult, zVecReg, rotationMatrixYRegisters[2]);
        vecResult = vmlaq_f32(vecResult, wVecReg, rotationMatrixYRegisters[3]);
        //normals
        normResult = vmlaq_f32(normResult, xNormReg, rotationMatrixYRegisters[0]);
        normResult = vmlaq_f32(normResult, yNormReg, rotationMatrixYRegisters[1]);
        normResult = vmlaq_f32(normResult, zNormReg, rotationMatrixYRegisters[2]);
        normResult = vmlaq_f32(normResult, wNormReg, rotationMatrixYRegisters[3]);
        //reset our registers to contain latest transformed values
        xVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 0));
        yVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 1));
        zVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 2));
        wVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 3));
        vecResult = vdupq_n_f32(0.0f);
        //normals
        xNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 0));
        yNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 1));
        zNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 2));
        wNormReg = vdupq_n_f32(vgetq_lane_f32(normResult, 3));
        normResult = vdupq_n_f32(0.0f);

        //z rotation matrix
        vecResult = vmlaq_f32(vecResult, xVecReg, rotationMatrixZRegisters[0]);
        vecResult = vmlaq_f32(vecResult, yVecReg, rotationMatrixZRegisters[1]);
        vecResult = vmlaq_f32(vecResult, zVecReg, rotationMatrixZRegisters[2]);
        vecResult = vmlaq_f32(vecResult, wVecReg, rotationMatrixZRegisters[3]);
        //normals final transform
        normResult = vmlaq_f32(normResult, xNormReg, rotationMatrixZRegisters[0]);
        normResult = vmlaq_f32(normResult, yNormReg, rotationMatrixZRegisters[1]);
        normResult = vmlaq_f32(normResult, zNormReg, rotationMatrixZRegisters[2]);
        normResult = vmlaq_f32(normResult, wNormReg, rotationMatrixZRegisters[3]);
        //reset our registers to contain latest transformed values
        xVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 0));
        yVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 1));
        zVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 2));
        wVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 3));
        vecResult = vdupq_n_f32(0.0f);
        
        //translation matrix
        vecResult = vmlaq_f32(vecResult, xVecReg, translationMatrixRegisters[0]);
        vecResult = vmlaq_f32(vecResult, yVecReg, translationMatrixRegisters[1]);
        vecResult = vmlaq_f32(vecResult, zVecReg, translationMatrixRegisters[2]);
        vecResult = vmlaq_f32(vecResult, wVecReg, translationMatrixRegisters[3]);
        //reset our registers to contain latest transformed values
        xVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 0));
        yVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 1));
        zVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 2));
        wVecReg = vdupq_n_f32(vgetq_lane_f32(vecResult, 3));
        //printf("%f, %f, %f, %f\n", vgetq_lane_f32(vecResult, 0), vgetq_lane_f32(vecResult, 1),vgetq_lane_f32(vecResult, 2), vgetq_lane_f32(vecResult, 3));
        

        //perspective projection matrix
        vec4 v;
        if(vgetq_lane_f32(vecResult, 2) <= 1.0f){
            v.x = vgetq_lane_f32(vecResult, 0);
            v.y = vgetq_lane_f32(vecResult, 1);
            v.z = vgetq_lane_f32(vecResult, 2);
            v.w = vgetq_lane_f32(vecResult, 2);
        }
        else{
            vecResult = vdupq_n_f32(0.0f);
            vecResult = vmlaq_f32(vecResult, xVecReg, perspectiveMatrixRegisters[0]);
            vecResult = vmlaq_f32(vecResult, yVecReg, perspectiveMatrixRegisters[1]);
            vecResult = vmlaq_f32(vecResult, zVecReg, perspectiveMatrixRegisters[2]);
            vecResult = vmlaq_f32(vecResult, wVecReg, perspectiveMatrixRegisters[3]);
            //printf("%f, %f, %f, %f\n", vgetq_lane_f32(vecResult, 0), vgetq_lane_f32(vecResult, 1),vgetq_lane_f32(vecResult, 2), vgetq_lane_f32(vecResult, 3));
            //printf("first: %f\n", vgetq_lane_f32(vecResult, 0));
            if(vgetq_lane_f32(vecResult, 3) > 1.0f){ 
                float32x4_t wVector = vdupq_n_f32(vgetq_lane_f32(vecResult, 3));
                vecResult = vdivq_f32(vecResult, wVector);
                //printf("second: %f\n", vgetq_lane_f32(vecResult, 0));
            }
            v.x = vgetq_lane_f32(vecResult, 0);
            v.y = vgetq_lane_f32(vecResult, 1);
            v.z = vgetq_lane_f32(vecResult, 2);
            v.w = vgetq_lane_f32(vecResult, 3);
            //printf("%f, %f, %f, %f\n", v.x, v.y, v.z, v.w);
        }
        
        //printf("%f, %f, %f, %f\n", vgetq_lane_f32(vecResult, 0), vgetq_lane_f32(vecResult, 1),vgetq_lane_f32(vecResult, 2), vgetq_lane_f32(vecResult, 3));
        //perspective divide
        
        NDCToScreenSpace(&v, 1.0, 100.0, 700, 1000);
        //printf("%f, %f, %f, %f\n", v.x, v.y, v.z, v.w);
        vb->vertices[i] = v.x;
        vb->vertices[i + 1] = v.y;
        //we are now w-buffering, maybe a more thorough implementation would be good
        vb->vertices[i + 2] = v.w * 25;


        //rotate the normal vectors
        //normalize normals
        float32x4_t normal = {vgetq_lane_f32(normResult, 0), vgetq_lane_f32(normResult, 1), vgetq_lane_f32(normResult, 2), 0.0f};
        //printf("%f, %f, %f, %f\n", vgetq_lane_f32(normResult, 0), vgetq_lane_f32(normResult, 1),vgetq_lane_f32(normResult, 2), vgetq_lane_f32(normResult, 3));
            
        float32x4_t normalizer = vmulq_f32(normal, normal);
        normalizer = vdupq_n_f32(sqrt(vaddvq_f32(normalizer)));
        float32x4_t normalizedNormal = vdivq_f32(normal, normalizer);
        
        //printf("%f, %f, %f, %f\n", vgetq_lane_f32(normalizedNormal, 0), vgetq_lane_f32(normalizedNormal, 1),vgetq_lane_f32(normalizedNormal, 2), vgetq_lane_f32(normalizedNormal, 3));

        //light vector and normal vector dot product
        float lightScalar = vaddvq_f32(vmulq_f32(lightVector, normalizedNormal));
        //printf("%f\n", lightScalar);
        lightScalar += 1.0f;
        cb->colors[i] = RGBClamp(cb->inputColors[i] * lightScalar);
        cb->colors[i + 1] = RGBClamp(cb->inputColors[i + 1] * lightScalar);
        cb->colors[i + 2] = RGBClamp(cb->inputColors[i + 2] * lightScalar);
        
        //camera vector and normal vector dot product
        float backFace = vaddvq_f32(vmulq_f32(normalizedNormal, cameraVector));
        if(backFace < -0.5){
            vb->indexBuffer[i/3] = 0;
        }else vb->indexBuffer[i/3] = 1;
    }
}