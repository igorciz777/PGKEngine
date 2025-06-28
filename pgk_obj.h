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
    Vec3 ambient = {1.0f, 1.0f, 1.0f};
    Vec3 diffuse = {1.0f, 1.0f, 1.0f};
    Vec3 specular = {1.0f, 1.0f, 1.0f};
    float specularExponent = 1.0f;
    bool smoothShading = false;
    bool hasNormalMap = false;
    bool hasSpecularMap = false;
    bool hasSpecularHighlightMap = false;
    bool hasAlphaMap = false;
    bool hasDisplacementMap = false;
    bool hasTexture = false;
    float normalMapStrength = 1.0f;
    std::shared_ptr<QImage> texture;
    std::shared_ptr<QImage> normalMap;
    std::shared_ptr<QImage> specularMap;
    std::shared_ptr<QImage> specularHighlightMap;
    std::shared_ptr<QImage> alphaMap;
    std::shared_ptr<QImage> displacementMap;
};

struct Mesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material material;
    std::string name;
};

struct Triangle{
    const bool receiveShadows;
    const bool castShadows;
    const Vec3 worldPosition;
    const Vec4 v0,v1,v2;
    const Vec3 ndc0,ndc1,ndc2;
    const Vec3 s0,s1,s2;
    const Vec3 n0,n1,n2;
    const Vec2 uv0,uv1,uv2;
    const Vec3 tangent, bitangent;
    std::shared_ptr<Material> material;
};

inline bool operator==(const Triangle &t1, const Triangle &t2){
    return t1.worldPosition == t2.worldPosition;
}

namespace ObjLoader
{
    std::vector<Mesh> loadObj(const std::string& filename);
    void parseMtlFile(const std::string& filename, std::map<std::string, Material>& materials, const std::string& basePath);
};

#endif // PGK_OBJ_H
