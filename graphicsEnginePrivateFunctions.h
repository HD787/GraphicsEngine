void perspectiveDivide(float* vb){
    //this abstration really isn't a big deal but i think it makes sense
}

void generateNormals(vertexBuffer* vb){
    for(int i = 0; i < vb->length; i+=3){
        //take two the the vertices, convert to vec3s
        //take cross product of said vec3s
        //write result somewhere??
        //the question is: "do you want to use interleved arrays or seperate arrays?"
        //idk the answer at this time
    }
}

//might do this differently
//may be better to just have a matrix associated with an obj
void objectSpaceToWolrdSpace(vertexBuffer* vb, int scalar){
    for(int i = 0; i< vb.length; i+= 3){
        vec4 temp;
        matrix4x4 mScalar
        temp.x = vb->pixels[i];
        temp.y = vb->pixels[i + 1];
        temp.z = vb->pixels[i + 2];
        temp.w = 1;
        matrix4x4 = createScalingMatrix(scalar, mScalar);
        vecByMatrix4x4(&temp, &mScalar);
        vb->pixels[i] = temp.x;
        vb->pixels[i + 1] temp.y;
        vb->pixels[i + 2] = temp.z;
    }

}