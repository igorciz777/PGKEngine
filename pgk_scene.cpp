// pgk_scene.cpp
#include "pgk_scene.h"


#include "pgk_input.h"
#include "pgk_light.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

PGK_Scene::PGK_Scene(){
    this->camera = std::make_shared<PGK_Camera>();
    this->camera->setAspectRatio(g_pgkCore.ASPECT_RATIO);
    this->camera->setFov(20);
    this->camera->setMode(PGK_Camera::Mode::Free);
    this->camera->setLocalPosition(Vec3(0,0,-5));
    this->sceneBackgroundColor = Qt::gray;
    createDefaultScene();
}

PGK_Scene::PGK_Scene(QString scenePath) {
    this->camera = std::make_shared<PGK_Camera>();
    this->camera->setAspectRatio(g_pgkCore.ASPECT_RATIO);
    this->camera->setFov(60);
    rootObject = std::make_shared<PGK_GameObject>();

    if(scenePath == "Helicopter demo") scenePath = QDir::currentPath()+"/scenes/heliScene.json";

    QFile sceneFile(scenePath);
    if (sceneFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(sceneFile.readAll());
        QJsonObject sceneObject = doc.object().value("scene").toObject();

        QString sceneName = sceneObject.value("name").toString();
        qDebug() << "Loading scene:" << sceneName;

        QJsonArray background = sceneObject.value("background").toArray();
        this->sceneBackgroundColor =  QColor(background[0].toInt(), background[1].toInt(), background[2].toInt());

        QJsonArray objectsArray = sceneObject.value("objects").toArray();
        for (const QJsonValue& objectValue : objectsArray) {
            QJsonObject object = objectValue.toObject();
            parseGameObject(object);
        }

        QJsonArray lightsArray = sceneObject.value("lights").toArray();
        for (const QJsonValue& lightValue : lightsArray) {
            QJsonObject light = lightValue.toObject();
            parseLight(light);
        }

        QJsonObject cameraObject = sceneObject.value("camera").toObject();
        parseCamera(cameraObject);

        sceneFile.close();
    } else {
        if(scenePath != "Default") qWarning() << "Failed to open scene file:" << scenePath;
        this->camera = std::make_shared<PGK_Camera>();
        this->camera->setAspectRatio(g_pgkCore.ASPECT_RATIO);
        this->camera->setFov(20);
        this->camera->setMode(PGK_Camera::Mode::Free);
        this->camera->setLocalPosition(Vec3(0,0,-5));
        this->sceneBackgroundColor = Qt::gray;
        createDefaultScene();
    }
}

PGK_Scene::~PGK_Scene() {}

void PGK_Scene::update(float &deltaTime) {
    camera->updateCamera(deltaTime);
    rootObject->update(deltaTime);
}

void PGK_Scene::render(PGK_View *view) {
    view->backgroundColor = this->sceneBackgroundColor;
    Mat4 projViewMatrix = this->camera->getProjectionMatrix(view->nearClip, view->farClip) * this->camera->getViewMatrix();
    rootObject->render(view, this->camera->getViewMatrix(),projViewMatrix,camera->getWorldPosition(),lights);
}


//scene parsing

void PGK_Scene::createDefaultScene() {
    rootObject = std::make_shared<PGK_GameObject>();

    lights.push_back(PGK_Light());
    lights[0].setLocalPosition(Vec3(8, 0, 10));
    lights[0].ambientColor = QColor(80, 80, 80);
    lights[0].diffuseColor = QColor(200, 0, 0);

    lights.push_back(PGK_Light());
    lights[1].setLocalPosition(Vec3(-8, 0, 10));
    lights[1].ambientColor = QColor(80, 80, 80);
    lights[1].diffuseColor = QColor(0, 0, 200);

    lights.push_back(PGK_Light());
    lights[2].setLocalPosition(Vec3(0, 8, 10));
    lights[2].ambientColor = QColor(80, 80, 80);
    lights[2].diffuseColor = QColor(0, 200, 0);

    std::vector<Mesh> meshes = ObjLoader::loadObj(QDir::currentPath().toStdString()+"/sphere.obj");
    QImage texture1 = QImage(64,64,QImage::Format_ARGB32);
    QImage texture2 = QImage(64,64,QImage::Format_ARGB32);
    QImage texture3 = QImage(64,64,QImage::Format_ARGB32);
    texture1.fill(Qt::white);
    texture2.fill(Qt::gray);
    texture3.fill(Qt::magenta);
    auto object = std::make_shared<PGK_GameObject>();
    object->setLocalPosition(Vec3(0, -1, -50));
    meshes[0].material.texture = texture1;
    object->setMeshes(meshes);

    auto objectChild_1 = std::make_shared<PGK_GameObject>();
    objectChild_1->setLocalPosition(Vec3(0, 1, -5));
    meshes[0].material.texture = texture2;
    objectChild_1->setMeshes(meshes);

    auto objectChild_2 = std::make_shared<PGK_GameObject>();
    objectChild_2->setLocalPosition(Vec3(0, 1, 5));
    meshes[0].material.texture = texture3;
    objectChild_2->setMeshes(meshes);

    std::weak_ptr<PGK_GameObject> weakobject = object;
    std::weak_ptr<PGK_GameObject> weakobjectChild_1 = objectChild_1;
    std::weak_ptr<PGK_GameObject> weakobjectChild_2 = objectChild_2;


    object->onUpdate = [weakobject](PGK_GameObject*, float deltaTime) {
        if (auto object = weakobject.lock()) {

            Vec3 rotation = object->getLocalEuler();
            rotation.x -= 1.0f * deltaTime;
            rotation.y += 1.0f * deltaTime;
            rotation.z -= 1.0f * deltaTime;

            object->setLocalEuler(rotation, Quat::RotationOrder::YXZ);
        }
    };

    objectChild_1->onUpdate = [weakobjectChild_1](PGK_GameObject*, float deltaTime) {
        if (auto object = weakobjectChild_1.lock()) {
            Vec3 rotation = object->getLocalEuler();
            rotation.x += 1.0f * deltaTime;
            rotation.y -= 1.0f * deltaTime;
            rotation.z += 1.0f * deltaTime;

            object->setLocalEuler(rotation, Quat::RotationOrder::XYZ);
        }
    };

    objectChild_2->onUpdate = [weakobjectChild_2](PGK_GameObject*, float deltaTime) {
        if (auto object = weakobjectChild_2.lock()) {
            Vec3 rotation = object->getLocalEuler();
            rotation.x -= 1.0f * deltaTime;
            rotation.y -= 1.0f * deltaTime;
            rotation.z += 1.0f * deltaTime;

            object->setLocalEuler(rotation, Quat::RotationOrder::ZXY);
        }
    };
    object->addChild(objectChild_1);
    object->addChild(objectChild_2);
    rootObject->addChild(object);
}

void PGK_Scene::parseGameObject(const QJsonObject& object) {
    //QString type = object.value("type").toString();
    QString name = object.value("name").toString();
    QString meshPath = object.value("mesh").toString();
    QString texturePath = object.value("texture").toString();

    std::vector<Mesh> meshes = ObjLoader::loadObj(QDir::currentPath().toStdString() + "/" + meshPath.toStdString());
    QImage texture(texturePath);
    for(auto& mesh : meshes){
        mesh.material.texture = texture;
    }

    auto gameObject = std::make_shared<PGK_GameObject>();
    gameObject->setName(name);
    gameObject->setMeshes(meshes);


    if (object.contains("position")) {
        QJsonArray positionArray = object.value("position").toArray();
        Vec3 position(positionArray[0].toDouble(), positionArray[1].toDouble(), positionArray[2].toDouble());
        gameObject->setLocalPosition(position);
    }
    if (object.contains("rotation")) {
        QJsonArray rotationArray = object.value("rotation").toArray();
        Vec3 rotation(rotationArray[0].toDouble(), rotationArray[1].toDouble(), rotationArray[2].toDouble());
        gameObject->setLocalEuler(rotation, Quat::RotationOrder::XYZ);
    }

    if (object.contains("components")) {
        QJsonArray componentsArray = object.value("components").toArray();
        for (const QJsonValue& componentValue : componentsArray) {
            QJsonObject component = componentValue.toObject();
            parseComponent(gameObject, component);
        }
    }

    if (object.contains("attachTo")) {
        QJsonArray attachArray = object.value("attachTo").toArray();
        for (const QJsonValue& attachValue : attachArray) {
            QJsonObject attach = attachValue.toObject();
            QString parentName = attach.value("object").toString();
            QJsonArray positionArray = attach.value("position").toArray();
            Vec3 offset(positionArray[0].toDouble(), positionArray[1].toDouble(), positionArray[2].toDouble());

            auto parent = findObjectByName(parentName);
            if (parent) {
                parent->addChild(gameObject);
                gameObject->setLocalPosition(offset);
            } else {
                qWarning() << "Parent object not found:" << parentName;
            }
        }
    }

    if (!object.contains("attachTo")) {
        rootObject->addChild(gameObject);
    }
}

void PGK_Scene::parseComponent(std::shared_ptr<PGK_GameObject> gameObject, const QJsonObject& component) {
    QString type = component.value("type").toString();
    QJsonObject properties = component.value("properties").toObject();

    if (type == "HeliControls") {
        const float speed = properties.value("speed").toDouble();
        const float lift = properties.value("lift").toDouble();
        const float tiltAmount = PGK_Math::degToRad(15.0f);
        const float tiltSpeed = 5.0f;

        gameObject->onUpdate = [speed, lift, tiltAmount, tiltSpeed](PGK_GameObject* self, float deltaTime) {
            auto& input = PGK_Input::instance();

            Vec3 position = self->getLocalPosition();

            Vec3 forward = self->getWorldRotation() * Vec3(0, 0, -1);
            Vec3 right   = self->getWorldRotation() * Vec3(1, 0,  0);

            forward = forward.normalize();
            right = right.normalize();
            forward.y = 0;
            right.y = 0;

            bool movingForward = input.getKey(Qt::Key_W);
            bool movingBackward = input.getKey(Qt::Key_S);
            bool movingLeft = input.getKey(Qt::Key_A);
            bool movingRight = input.getKey(Qt::Key_D);

            if (movingForward) position -= forward * speed * deltaTime;
            if (movingBackward) position += forward * speed * deltaTime;
            if (movingLeft) position += right * speed * deltaTime;
            if (movingRight) position -= right * speed * deltaTime;
            if (input.getKey(Qt::Key_Space)) position.y += lift * deltaTime;
            if (input.getKey(Qt::Key_Control)) position.y -= lift * deltaTime;

            static Quat baseRotation = Quat::IDENTITY;  // Stores yaw-only rotation
            static Quat currentTilt = Quat::IDENTITY;   // Stores temporary pitch/roll

            // Handle yaw rotation (Q/E)
            float yawDelta = 0.0f;
            if (input.getKey(Qt::Key_Q)) yawDelta += 1.0f;
            if (input.getKey(Qt::Key_E)) yawDelta -= 1.0f;
            if (yawDelta != 0.0f) {
                baseRotation = Quat(Vec3(0,1,0), yawDelta * deltaTime) * baseRotation;
            }

            // Calculate target tilt based on movement
            Vec3 targetTilt(0,0,0);
            if (movingForward)  targetTilt.x += tiltAmount;
            if (movingBackward) targetTilt.x -= tiltAmount;
            if (movingLeft)     targetTilt.z -= tiltAmount;
            if (movingRight)    targetTilt.z += tiltAmount;

            // Convert target tilt to quaternion (YXZ order)
            Quat targetTiltQuat = Quat(targetTilt, T_Quat<float>::RotationOrder::XYZ);

            // Smoothly interpolate towards target tilt
            currentTilt = Quat::slerp(currentTilt.normalize(), targetTiltQuat.normalize(), tiltSpeed * deltaTime);

            // Combine rotations: first apply yaw, then apply tilt
            Quat finalRotation = baseRotation * currentTilt;

            self->setLocalRotation(finalRotation);
            self->setLocalPosition(position);
        };
    }

    if(type == "Rotor"){
        float speed = properties.value("speed").toDouble();
        gameObject->onUpdate = [speed](PGK_GameObject* self, float deltaTime) {
            auto& input = PGK_Input::instance();
            bool movingUp = input.getKey(Qt::Key_Space);
            bool movingDown = input.getKey(Qt::Key_Control);

            Vec3 rotation = self->getLocalEuler();

            if(movingUp) rotation.y += (speed*2) * deltaTime;
            else if(movingDown) rotation.y += (speed/2) * deltaTime;
            else rotation.y += speed * deltaTime;

            self->setLocalEuler(rotation, Quat::RotationOrder::XYZ);
        };
    }
}

void PGK_Scene::parseLight(const QJsonObject& light) {
    PGK_Light phongLight;

    QJsonArray positionArray = light.value("position").toArray();
    phongLight.setLocalPosition(Vec3(positionArray[0].toDouble(), positionArray[1].toDouble(), positionArray[2].toDouble()));

    QJsonArray colorArray = light.value("color").toArray();
    phongLight.diffuseColor = QColor(colorArray[0].toInt(), colorArray[1].toInt(), colorArray[2].toInt());

    phongLight.diffusePower = light.value("intensity").toDouble();
    phongLight.ambientColor = this->sceneBackgroundColor;

    lights.push_back(phongLight);
}

void PGK_Scene::parseCamera(const QJsonObject& camera) {
    QString type = camera.value("type").toString();
    QJsonObject properties = camera.value("properties").toObject();

    if (type == "AttachedCamera") {
        QString targetName = properties.value("target").toString();
        QJsonArray offsetArray = properties.value("offset").toArray();
        Vec3 offset(offsetArray[0].toDouble(), offsetArray[1].toDouble(), offsetArray[2].toDouble());

        auto target = findObjectByName(targetName);
        if (target) {
            this->camera->setMode(PGK_Camera::Mode::Attached);
            this->camera->setLocalPosition(offset);
            target->addChild(this->camera);
        } else {
            qWarning() << "Camera target not found:" << targetName;
        }
    } else if (type == "FreeFlyCamera") {
        this->camera->setMode(PGK_Camera::Mode::Free);
    }
}

std::shared_ptr<PGK_GameObject> PGK_Scene::findObjectByName(const QString& name) {
    // rekursja znajdywanie objektu
    std::function<std::shared_ptr<PGK_GameObject>(std::shared_ptr<PGK_GameObject>)> search;
    search = [&search, &name](std::shared_ptr<PGK_GameObject> obj) -> std::shared_ptr<PGK_GameObject> {
        if (obj->getName() == name) {
            return obj;
        }
        for (auto& child : obj->getChildren()) {
            auto result = search(child);
            if (result) {
                return result;
            }
        }
        return nullptr;
    };

    return search(rootObject);
}
