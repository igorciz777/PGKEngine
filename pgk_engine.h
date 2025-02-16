// pgk_engine.h
#ifndef PGK_ENGINE_H
#define PGK_ENGINE_H

#include <QObject>
#include <QTimer>
#include "pgk_scene.h"

class PGK_Engine : public QObject {
    Q_OBJECT

public:
    PGK_Engine(PGK_Scene *scene, PGK_View *view, QObject *parent = nullptr);
    void start();

private slots:
    void update();

private:
    PGK_View* view;
    PGK_Scene* scene;
    QTimer timer;
    qint64 lastTime;
};

#endif // PGK_ENGINE_H
