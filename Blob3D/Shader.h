#pragma once
#include <string>
#include <GL/glew.h>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath, bool fromFile);
    void use();
    void setMat4(const std::string& name, const float* value) const;
    void setVec3(const std::string& name, const float* value) const;
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};
