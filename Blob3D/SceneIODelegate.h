#pragma once

#include <string>
class Shader;

class SceneIODelegate {
public:
    virtual ~SceneIODelegate() = default;
    virtual void SaveSceneToFile(const std::string& filename) = 0;
    virtual void LoadSceneFromFile(const std::string& filename) = 0;
};