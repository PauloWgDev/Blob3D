#include "Shader.h"
#include "Mesh.h"
#include "Application.h"
#include <iostream>

int main()
{
    Application app(800, 600, "Blob3D");
    app.Start();  // Setup
    app.Run();    // Enter main loop
    return 0;
}