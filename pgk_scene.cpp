#include "pgk_scene.h"
#include "pgk_draw.h"
#include "pgk_input.h"
#include "pgk_light.h"
#include "pgk_raycast.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

PGK_Scene::PGK_Scene(){
    createDefaultScene();
}

PGK_Scene::PGK_Scene(QString scenePath) {
    this->camera = std::make_shared<PGK_Camera>();
    this->camera->setAspectRatio(g_pgkCore.ASPECT_RATIO);
    this->camera->setFov(60);
    rootObject = std::make_shared<PGK_GameObject>();

    if(scenePath == "Helicopter demo") scenePath = QDir::currentPath()+"/scenes/heliScene.json";
    if(scenePath == "Normal mapping demo") scenePath = QDir::currentPath()+"/scenes/mtlTest.json";
    if(scenePath == "Lightning demo") scenePath = QDir::currentPath()+"/scenes/stanfordBunny.json";
    if(scenePath == "Stanford Dragon benchmark") scenePath = QDir::currentPath()+"/scenes/stanfordDragon.json";

    QFile sceneFile(scenePath);
    if (sceneFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(sceneFile.readAll());
        QJsonObject sceneObject = doc.object().value("scene").toObject();

        QString sceneName = sceneObject.value("name").toString();
        qDebug() << "Loading scene:" << sceneName;

        QJsonArray background = sceneObject.value("background").toArray();
        this->sceneBackgroundColor =  std::make_shared<cVec3>(background[0].toInt(), background[1].toInt(), background[2].toInt());

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
        createDefaultScene();
    }
    triangleBufferSize = rootObject->calcTriangleBufferSize();
}

PGK_Scene::~PGK_Scene() {}

void PGK_Scene::update(float &deltaTime) {
    camera->updateCamera(deltaTime);
    rootObject->update(deltaTime);
    for(const auto &light : lights){
        light->update(deltaTime);
    }
}

void PGK_Scene::render(PGK_View *view) {
    view->backgroundColor = this->sceneBackgroundColor;

    triangleBuffer.clear();
    triangleBuffer.reserve(triangleBufferSize);

    rootObject->getTriangleBuffer(triangleBuffer, view, this->camera->getViewMatrix(), this->camera->getProjectionMatrix(view->nearClip, view->farClip));
    for(const auto& triangle : triangleBuffer){
        PGK_Draw::drawTriangle(view->canvas, triangle, view->_zbuffer, lights, camera->getWorldPosition(),triangleBuffer);
    }
}


//scene parsing

void PGK_Scene::createDefaultScene() {
    this->camera = std::make_shared<PGK_Camera>();
    this->camera->setAspectRatio(g_pgkCore.ASPECT_RATIO);
    this->camera->setFov(20);
    this->camera->setMode(PGK_Camera::Mode::Free);
    this->camera->setLocalPosition(Vec3(0,0,-5));
    this->sceneBackgroundColor = std::make_shared<cVec3>(PGK_Math::cVec3FromQColor(Qt::gray));

    rootObject = std::make_shared<PGK_GameObject>();

    auto light1 = std::make_shared<PGK_Light>();
    light1->setLocalPosition(Vec3(8, 0, 10));
    light1->ambientColor = this->sceneBackgroundColor;
    light1->diffuseColor = cVec3(200, 0, 0);
    light1->castShadows = true;
    lights.push_back(light1);

    auto light2 = std::make_shared<PGK_Light>();
    light2->setLocalPosition(Vec3(-8, 0, 10));
    light2->ambientColor = this->sceneBackgroundColor;
    light2->diffuseColor = cVec3(0, 0, 200);
    light2->castShadows = true;
    lights.push_back(light2);

    auto light3 = std::make_shared<PGK_Light>();
    light3->setLocalPosition(Vec3(0, 8, 10));
    light3->ambientColor = this->sceneBackgroundColor;
    light3->diffuseColor = cVec3(0, 200, 0);
    light3->castShadows = true;
    lights.push_back(light3);

    std::vector<Mesh> title1 = ObjLoader::loadObj(QDir::currentPath().toStdString()+"/title_1.obj");
    std::vector<Mesh> title2 = ObjLoader::loadObj(QDir::currentPath().toStdString()+"/title_2.obj");
    auto texture1 = std::make_shared<QImage>(QImage(64,64,QImage::Format_ARGB32));
    texture1->fill(Qt::gray);
    auto object = std::make_shared<PGK_GameObject>();
    object->setLocalPosition(Vec3(0, 0, -20));
    title1[0].material.texture = texture1;
    object->setMeshes(title1);
    object->castShadows = true;
    object->receiveShadows = true;

    auto object2 = std::make_shared<PGK_GameObject>();
    object2->setLocalPosition(Vec3(0, -1, -20));
    title2[0].material.texture = texture1;
    object2->setMeshes(title2);
    object2->castShadows = true;
    object2->receiveShadows = true;

    std::weak_ptr<PGK_GameObject> weakobject = object;

    object->onUpdate = [weakobject](PGK_GameObject*, float deltaTime) {
        if (auto object = weakobject.lock()) {

            Vec3 rotation = object->getLocalEuler();
            //rotation.x -= 1.0f * deltaTime;
            //rotation.y += 1.0f * deltaTime;
            rotation.z -= 1.0f * deltaTime;

            object->setLocalEuler(rotation, Quat::RotationOrder::YXZ);
        }
    };
    rootObject->addChild(object);
    rootObject->addChild(object2);
}

void PGK_Scene::parseGameObject(const QJsonObject& object) {
    //QString type = object.value("type").toString();
    QString name = object.value("name").toString();
    QString meshPath = object.value("mesh").toString();
    QString texturePath = object.value("texture").toString();
    bool castShadows = object.value("cast_shadow").toBool();
    bool receiveShadows = object.value("receive_shadow").toBool();

    auto gameObject = std::make_shared<PGK_GameObject>();

    if(object.contains("mesh")){
        std::vector<Mesh> meshes = ObjLoader::loadObj(QDir::currentPath().toStdString() + "/" + meshPath.toStdString());
        if(object.contains("texture")){
            auto texture = std::make_shared<QImage>(QImage(texturePath));
            for(auto& mesh : meshes){
                mesh.material.texture = texture;
            }
        }
        gameObject->setMeshes(meshes);
    }else{
        gameObject->isVisible = false;
    }


    gameObject->setName(name);

    if(castShadows) gameObject->castShadows = true;
    if(receiveShadows) gameObject->receiveShadows = true;

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
    if (object.contains("scale")) {
        QJsonArray scaleArray = object.value("scale").toArray();
        Vec3 scale(scaleArray[0].toDouble(), scaleArray[1].toDouble(), scaleArray[2].toDouble());
        gameObject->setLocalScale(scale);
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

            // forward is inverted - the heli.obj model
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

            static Quat baseRotation = Quat::IDENTITY;
            static Quat currentTilt = Quat::IDENTITY;

            float yawDelta = 0.0f;
            if (input.getKey(Qt::Key_Q)) yawDelta += 1.0f;
            if (input.getKey(Qt::Key_E)) yawDelta -= 1.0f;
            if (yawDelta != 0.0f) {
                baseRotation = Quat(Vec3(0,1,0), yawDelta * deltaTime) * baseRotation;
            }

            Vec3 targetTilt(0,0,0);
            if (movingForward)  targetTilt.x += tiltAmount;
            if (movingBackward) targetTilt.x -= tiltAmount;
            if (movingLeft)     targetTilt.z -= tiltAmount;
            if (movingRight)    targetTilt.z += tiltAmount;

            Quat targetTiltQuat = Quat(targetTilt, T_Quat<float>::RotationOrder::XYZ);

            currentTilt = Quat::slerp(currentTilt.normalize(), targetTiltQuat.normalize(), tiltSpeed * deltaTime);

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

    if (type == "TankControls") {
        const float speed = properties.value("speed").toDouble();

        gameObject->onUpdate = [speed, this](PGK_GameObject* self, float deltaTime) {
            auto& input = PGK_Input::instance();

            Vec3 position = self->getLocalPosition();

            Vec3 forward = self->getWorldRotation() * Vec3(0, 0, 1);
            Vec3 right   = self->getWorldRotation() * Vec3(1, 0,  0);

            forward = forward.normalize();
            right = right.normalize();
            forward.y = 0;
            right.y = 0;

            bool movingForward = input.getKey(Qt::Key_W);
            bool movingBackward = input.getKey(Qt::Key_S);

            if (movingForward) position += forward * speed * deltaTime;
            if (movingBackward) position -= forward * speed * deltaTime;

            static Quat baseRotation = Quat::IDENTITY;
            float yawDelta = 0.0f;
            if (input.getKey(Qt::Key_A)) yawDelta += 1.0f;
            if (input.getKey(Qt::Key_D)) yawDelta -= 1.0f;
            if (yawDelta != 0.0f) {
                baseRotation = Quat(Vec3(0,1,0), yawDelta * deltaTime) * baseRotation;
            }

            Vec3 rayOrigin = position;
            Vec3 rayDirection = Vec3(0, -1, 0);
            float rayLength = 20.0f;

            RaycastHit hit = PGK_Raycast::raycast(rayOrigin, rayDirection, rayLength, this->rootObject->getChildren());
            if (hit.hit) {
                position.y = hit.point.y + 1.1f;

                Vec3 up = hit.normal;
                Vec3 newForward = forward - (forward.dot(up)) * up;
                newForward = newForward.normalize();

                Quat newRotation = baseRotation * self->getWorldRotation();

                self->setLocalRotation(newRotation);
            }else{
                position.y -= 9.81f * deltaTime;
            }

            self->setLocalPosition(position);
        };
    }
}

void PGK_Scene::parseLight(const QJsonObject& light) {
    auto phongLight = std::make_shared<PGK_Light>();

    QJsonArray positionArray = light.value("position").toArray();
    phongLight->setLocalPosition(Vec3(positionArray[0].toDouble(), positionArray[1].toDouble(), positionArray[2].toDouble()));

    QJsonArray colorArray = light.value("color").toArray();
    phongLight->diffuseColor = cVec3(colorArray[0].toInt(), colorArray[1].toInt(), colorArray[2].toInt());

    phongLight->diffusePower = light.value("intensity").toDouble();
    phongLight->ambientColor = this->sceneBackgroundColor;

    if (light.contains("components")) {
        QJsonArray componentsArray = light.value("components").toArray();
        for (const QJsonValue& componentValue : componentsArray) {
            QJsonObject component = componentValue.toObject();
            parseComponent(phongLight, component);
        }
    }
    if (light.contains("attachTo")) {
        QJsonArray attachArray = light.value("attachTo").toArray();
        for (const QJsonValue& attachValue : attachArray) {
            QJsonObject attach = attachValue.toObject();
            QString parentName = attach.value("object").toString();
            QJsonArray positionArray = attach.value("position").toArray();
            Vec3 offset(positionArray[0].toDouble(), positionArray[1].toDouble(), positionArray[2].toDouble());

            auto parent = findObjectByName(parentName);
            if (parent) {
                parent->addChild(phongLight);
                phongLight->setLocalPosition(offset);
            } else {
                qWarning() << "Parent object not found:" << parentName;
            }
        }
    }

    if(light.contains("type")){
        QString type = light.value("type").toString();
        if(type == "Directional") phongLight->lightType = PGK_Light::Type::Directional;
        if(type == "Point") phongLight->lightType = PGK_Light::Type::Point;
        if(type == "Spot") phongLight->lightType = PGK_Light::Type::Spot;
    }

    if(light.contains("decay")) phongLight->decay = light.value("decay").toDouble();
    if(light.contains("distance")) phongLight->distance = light.value("distance").toDouble();
    if(light.contains("angle")) phongLight->angle = light.value("angle").toDouble();
    if(light.contains("penumbra")) phongLight->penumbra = light.value("penumbra").toDouble();

    bool castShadows = light.value("cast_shadow").toBool();
    if(castShadows) phongLight->castShadows = true;

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
        if(camera.contains("position")){
            QJsonArray positionArray = camera.value("position").toArray();
            Vec3 position(positionArray[0].toDouble(), positionArray[1].toDouble(), positionArray[2].toDouble());
            this->camera->setLocalPosition(position);
        }
        if(camera.contains("speed")){
            this->camera->setMoveSpeed(camera.value("speed").toDouble());
        }
    }
    this->camera->setName("camera");
}

std::shared_ptr<PGK_GameObject> PGK_Scene::findObjectByName(const QString& name) {
    // recursive search
    std::function<std::shared_ptr<PGK_GameObject>(std::shared_ptr<PGK_GameObject>)> search;
    search = [&search, &name](std::shared_ptr<PGK_GameObject> obj) -> std::shared_ptr<PGK_GameObject> {
        if (obj->getName() == name) {
            return obj;
        }
        for (const auto& child : obj->getChildren()) {
            auto result = search(child);
            if (result) {
                return result;
            }
        }
        return nullptr;
    };

    return search(rootObject);
}
