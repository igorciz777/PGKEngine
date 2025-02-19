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
    QColor ambientColor = QColor(80, 80, 80);
    QColor diffuseColor = QColor(200, 200, 200);
    QColor specularColor = QColor(255, 255, 255);
    bool castShadows = false;
};

#endif // PGK_LIGHT_H
