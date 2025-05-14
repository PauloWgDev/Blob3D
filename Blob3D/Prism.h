#pragma once
#include "Object3D.h"

static float prismVertices[] = {
    // Positions
    -0.5f, -0.75f, -0.25f,
     0.5f, -0.75f, -0.25f,
     0.5f,  0.75f, -0.25f,
     0.5f,  0.75f, -0.25f,
    -0.5f,  0.75f, -0.25f,
    -0.5f, -0.75f, -0.25f,

    -0.5f, -0.75f,  0.25f,
     0.5f, -0.75f,  0.25f,
     0.5f,  0.75f,  0.25f,
     0.5f,  0.75f,  0.25f,
    -0.5f,  0.75f,  0.25f,
    -0.5f, -0.75f,  0.25f,

    -0.5f,  0.75f,  0.25f,
    -0.5f,  0.75f, -0.25f,
    -0.5f, -0.75f, -0.25f,
    -0.5f, -0.75f, -0.25f,
    -0.5f, -0.75f,  0.25f,
    -0.5f,  0.75f,  0.25f,

     0.5f,  0.75f,  0.25f,
     0.5f,  0.75f, -0.25f,
     0.5f, -0.75f, -0.25f,
     0.5f, -0.75f, -0.25f,
     0.5f, -0.75f,  0.25f,
     0.5f,  0.75f,  0.25f,

    -0.5f, -0.75f, -0.25f,
     0.5f, -0.75f, -0.25f,
     0.5f, -0.75f,  0.25f,
     0.5f, -0.75f,  0.25f,
    -0.5f, -0.75f,  0.25f,
    -0.5f, -0.75f, -0.25f,

    -0.5f,  0.75f, -0.25f,
     0.5f,  0.75f, -0.25f,
     0.5f,  0.75f,  0.25f,
     0.5f,  0.75f,  0.25f,
    -0.5f,  0.75f,  0.25f,
    -0.5f,  0.75f, -0.25f
};



class Prism : public Object3D
{
public:
    Prism(Shader* shader_) {
        shader = shader_;
        mesh = new Mesh(prismVertices, sizeof(prismVertices));
    }

    ~Prism(){}
};
