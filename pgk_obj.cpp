#include "pgk_obj.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<Mesh> ObjLoader::loadObj(const std::string &filename) {
    std::vector<Mesh> meshes;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return meshes;
    }

    size_t lastSlash = filename.find_last_of("/\\");
    std::string basePath = (lastSlash != std::string::npos) ? filename.substr(0, lastSlash + 1) : "";

    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> texCoords;
    std::map<std::string, Material> mtlMaterials;

    std::string currentObject;
    std::string currentMtlName;
    bool currentSmooth = false;
    Mesh currentMesh;

    auto finalizeMesh = [&]() {
        if (!currentMesh.vertices.empty()) {
            meshes.push_back(currentMesh);
            currentMesh = Mesh();
        }
    };

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "o") {
            std::string objectName;
            iss >> objectName;
            if (objectName != currentObject) {
                finalizeMesh();
                currentObject = objectName;
                currentMesh.name = currentObject;
            }
        } else if (type == "mtllib") {
            std::string mtlFile;
            iss >> mtlFile;
            parseMtlFile(basePath + mtlFile, mtlMaterials, basePath);
        } else if (type == "usemtl") {
            std::string mtlName;
            iss >> mtlName;
            if (mtlName != currentMtlName) {
                finalizeMesh();
                currentMtlName = mtlName;
                if (mtlMaterials.count(currentMtlName)) {
                    currentMesh.material = mtlMaterials[currentMtlName];
                }
            }
        } else if (type == "s") {
            std::string sVal;
            iss >> sVal;
            bool newSmooth = (sVal != "off");
            if (newSmooth != currentSmooth) {
                finalizeMesh();
                currentSmooth = newSmooth;
            }
            currentMesh.material.smoothShading = currentSmooth;
        } else if (type == "v") {
            Vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        } else if (type == "vn") {
            Vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        } else if (type == "vt") {
            Vec2 tex;
            iss >> tex.x >> tex.y;
            tex.y = 1.0f - tex.y; // Flip V coordinate
            texCoords.push_back(tex);
        } else if (type == "f") {
            std::vector<Vertex> faceVertices;
            std::string vert;
            while (iss >> vert) {
                std::istringstream vss(vert);
                std::string v, vt, vn;
                std::getline(vss, v, '/');
                std::getline(vss, vt, '/');
                std::getline(vss, vn, '/');

                Vertex vertex;
                if (!v.empty()) vertex.position = positions[std::stoi(v) - 1];
                if (!vt.empty()) vertex.texCoord = texCoords[std::stoi(vt) - 1];
                if (!vn.empty()) vertex.normal = normals[std::stoi(vn) - 1];

                currentMesh.vertices.push_back(vertex);
                currentMesh.indices.push_back(currentMesh.vertices.size() - 1);
            }
        }
    }

    finalizeMesh();
    return meshes;
}

void ObjLoader::parseMtlFile(const std::string& filename, std::map<std::string, Material>& materials, const std::string& basePath) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open MTL file " << filename << std::endl;
        return;
    }

    Material currentMtl;
    std::string currentMtlName;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "newmtl") {
            if (!currentMtlName.empty()) {
                materials[currentMtlName] = currentMtl;
            }
            iss >> currentMtlName;
            currentMtl = Material();
        } else if (type == "Ka") {
            iss >> currentMtl.ambient.x >> currentMtl.ambient.y >> currentMtl.ambient.z;
        } else if (type == "Kd") {
            iss >> currentMtl.diffuse.x >> currentMtl.diffuse.y >> currentMtl.diffuse.z;
        } else if (type == "Ks") {
            iss >> currentMtl.specular.x >> currentMtl.specular.y >> currentMtl.specular.z;
        } else if (type == "Ns") {
            iss >> currentMtl.shininess;
        } else if (type == "map_Kd") {
            std::string texFile;
            iss >> texFile;
            currentMtl.texture.load(QString::fromStdString(basePath + texFile));
        }
    }

    if (!currentMtlName.empty()) {
        materials[currentMtlName] = currentMtl;
    }
}
