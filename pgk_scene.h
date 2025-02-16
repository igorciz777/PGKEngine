// pgk_scene.h
#ifndef PGK_SCENE_H
#define PGK_SCENE_H

#include "pgk_camera.h"
#include "pgk_gameobject.h"
#include "pgk_view.h"
#include <pgk_core.h>
#include <memory>

class PGK_Scene {
public:
    PGK_Scene();
    PGK_Scene(QString scenePath);
    ~PGK_Scene();

    std::shared_ptr<PGK_Camera> getCamera() { return camera; }

    void update(float &deltaTime);
    void render(PGK_View *view);

private:
    std::shared_ptr<PGK_GameObject> rootObject;
    std::vector<PGK_Light> lights;
    std::shared_ptr<PGK_Camera> camera;
    QColor sceneBackgroundColor;
    void createDefaultScene();

    //Json scene parser
    void parseGameObject(const QJsonObject& object);
    void parseComponent(std::shared_ptr<PGK_GameObject> gameObject, const QJsonObject& component);
    void parseLight(const QJsonObject& light);
    void parseCamera(const QJsonObject& camera);
    std::shared_ptr<PGK_GameObject> findObjectByName(const QString& name);
};

#endif // PGK_SCENE_H
