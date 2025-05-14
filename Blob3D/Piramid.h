#pragma once
#include "Object3D.h"

static float pyramidVertices[] = {
    // Base (square)
    -0.5f, -0.75f, -0.5f,
     0.5f, -0.75f, -0.5f,
     0.5f, -0.75f,  0.5f,
     0.5f, -0.75f,  0.5f,
    -0.5f, -0.75f,  0.5f,
    -0.5f, -0.75f, -0.5f,

    // Side 1
    -0.5f, -0.75f, -0.5f,
     0.5f, -0.75f, -0.5f,
     0.0f,  0.75f,  0.0f,

     // Side 2
      0.5f, -0.75f, -0.5f,
      0.5f, -0.75f,  0.5f,
      0.0f,  0.75f,  0.0f,

      // Side 3
       0.5f, -0.75f,  0.5f,
      -0.5f, -0.75f,  0.5f,
       0.0f,  0.75f,  0.0f,

       // Side 4
       -0.5f, -0.75f,  0.5f,
       -0.5f, -0.75f, -0.5f,
        0.0f,  0.75f,  0.0f
};

class Piramid : public Object3D
{
public:
    Piramid(Shader* shader_) {
        shader = shader_;
        mesh = new Mesh(pyramidVertices, sizeof(pyramidVertices));
    }

    ~Piramid() {}
};
