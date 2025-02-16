#ifndef PGK_OBJ_H
#define PGK_OBJ_H

#include "pgk_math.h"

#include <QImage>
#include <map>

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
};

struct Material
{
    Vec3 ambient = {0.2f, 0.2f, 0.2f};
    Vec3 diffuse = {0.8f, 0.8f, 0.8f};
    Vec3 specular = {1.0f, 1.0f, 1.0f};
    float shininess = 32.0f;
    bool smoothShading = false;
    QImage texture;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;
    std::string name;
};

namespace ObjLoader
{
    std::vector<Mesh> loadObj(const std::string& filename);
    void parseMtlFile(const std::string& filename, std::map<std::string, Material>& materials, const std::string& basePath);
};

#endif // PGK_OBJ_H
