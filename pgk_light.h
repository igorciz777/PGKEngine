#ifndef PGK_LIGHT_H
#define PGK_LIGHT_H

#include "pgk_gameobject.h"

class PGK_Light : public PGK_GameObject
{
public:
    enum class Type{
        Directional,
        Point,
        Spot
    };
    PGK_Light();
    float diffusePower = 1.0f;
    float specularPower = 1.0f;
    float shininess = 1.0f;
    std::shared_ptr<cVec3> ambientColor = nullptr;
    cVec3 diffuseColor = cVec3(200, 200, 200);
    cVec3 specularColor = cVec3(255, 255, 255);
    bool castShadows = false;
    Type lightType = Type::Directional;

    // Point
    float distance=10;
    float decay=2;

    //Spot
    float angle=0.4f;
    float penumbra=0;
};

#endif // PGK_LIGHT_H
