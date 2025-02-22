#ifndef PGK_LIGHT_H
#define PGK_LIGHT_H

#include "pgk_gameobject.h"

class PGK_Light : public PGK_GameObject
{
public:
    PGK_Light();
    float diffusePower = 1.0f;
    float specularPower = 1.0f;
    float shininess = 1.0f;
    std::shared_ptr<cVec3> ambientColor = nullptr;
    cVec3 diffuseColor = cVec3(200, 200, 200);
    cVec3 specularColor = cVec3(255, 255, 255);
    bool castShadows = false;
};

#endif // PGK_LIGHT_H
