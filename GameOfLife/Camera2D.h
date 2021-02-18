#pragma once
#include "Shader.h"

class Camera2D :
    public OrographicCamera
{
public:
    Camera2D(int& weight, int& height, float scale = 1.0f, float near_plane = -1000.0f, float far_plane = 1000.0f): 
        OrographicCamera(weight, height, scale , near_plane, far_plane)
    {
    }

    Camera2D() = default;


};

