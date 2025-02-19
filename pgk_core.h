#ifndef PGK_CORE_H
#define PGK_CORE_H
#include <QString>
#include <cstdint>

typedef struct PGK_CORE
{
    uint32_t RESOLUTION_WIDTH = 320;
    uint32_t RESOLUTION_HEIGHT = 240;
    bool WINDOWED = true;
    bool SCALABLE = false;
    bool TEX_FILTERING = true;
    bool SMOOTH_SHADING = true;
    bool RAYCAST_SHADOWS = false;
    float ASPECT_RATIO = 4.f / 3.f;
    float REFRESH_RATE = 60;
    float SHADOW_DRAW_DISTANCE = 250.0f;
} PGK_CORE;

extern PGK_CORE g_pgkCore;

#endif // PGK_CORE_H
