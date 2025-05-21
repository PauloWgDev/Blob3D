#pragma once
#include "Object3D.h"
#include <vector>
#include <cmath>

class Sphere : public Object3D
{
public:
    Sphere(Shader* shader_, int sectorCount = 24, int stackCount = 16)
    {
        shader = shader_;
        std::vector<float> vertices;

        const float PI = 3.14159265359f;

        for (int i = 0; i <= stackCount; ++i)
        {
            float stackAngle = PI / 2 - i * (PI / stackCount);  // from pi/2 to -pi/2
            float xy = cosf(stackAngle);   // r * cos(u)
            float z = sinf(stackAngle);    // r * sin(u)

            for (int j = 0; j <= sectorCount; ++j)
            {
                float sectorAngle = j * 2 * PI / sectorCount;  // from 0 to 2pi
                float x = xy * cosf(sectorAngle);              // x = r * cos(u) * cos(v)
                float y = xy * sinf(sectorAngle);              // y = r * cos(u) * sin(v)
                vertices.push_back(x * 0.5f);  // scale to fit -0.5 to +0.5 range
                vertices.push_back(y * 0.5f);
                vertices.push_back(z * 0.5f);
            }
        }

        std::vector<unsigned int> indices;
        for (int i = 0; i < stackCount; ++i)
        {
            int k1 = i * (sectorCount + 1);  // beginning of current stack
            int k2 = k1 + sectorCount + 1;   // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        std::vector<float> finalVertices;
        for (unsigned int idx : indices) {
            finalVertices.push_back(vertices[3 * idx]); // here i get message of arithmetic overload
            finalVertices.push_back(vertices[3 * idx + 1]);
            finalVertices.push_back(vertices[3 * idx + 2]);
        }

        mesh = new Mesh(finalVertices.data(), finalVertices.size() * sizeof(float), finalVertices.size() / 3);

    }

    ~Sphere() {}
};
