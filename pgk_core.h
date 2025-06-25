#ifndef PGK_CORE_H
#define PGK_CORE_H
#include <QString>
#include <cstdint>

typedef struct PGK_CORE
{
    size_t AVAILABLE_THREADS = 1;
    uint32_t RESOLUTION_WIDTH = 320;
    uint32_t RESOLUTION_HEIGHT = 240;
    bool WINDOWED = true;
    bool SCALABLE = false;
    bool TEX_FILTERING = true;
    int SHADING_MODE = 1; // 0: Flat, 1: Blinn-Phong, 2: GGX
    bool RAYCAST_SHADOWS = false;
    bool RENDER_FOG = false;
    float ASPECT_RATIO = 4.f / 3.f;
    float REFRESH_RATE = 60;
    float SHADOW_DRAW_DISTANCE = 50.0f;
} PGK_CORE;

extern PGK_CORE g_pgkCore;

#endif // PGK_CORE_H
