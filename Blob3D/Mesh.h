#pragma once
#include <GL/glew.h>

class Mesh
{
public:
    unsigned int VAO;
    Mesh(const float* vertices, unsigned int size);
    void draw() const;
    void cleanup() const;
private:
    unsigned int VBO;
};