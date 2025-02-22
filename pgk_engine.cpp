#include "pgk_draw.h"
#include "pgk_engine.h"
#include "pgk_input.h"
#include <QDateTime>

PGK_Engine::PGK_Engine(PGK_Scene *scene, PGK_View *view, QObject *parent)
    : QObject(parent), view(view), scene(scene), lastTime(QDateTime::currentMSecsSinceEpoch()) {
    connect(&timer, &QTimer::timeout, this, &PGK_Engine::update);
    timer.start(1000 / g_pgkCore.REFRESH_RATE);

    PGK_Input::instance().update();
}

void PGK_Engine::start() {
    timer.start();
}

void PGK_Engine::update() {
    const qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    float deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    scene->update(deltaTime);

    this->view->canvas.fill(PGK_Math::QColorFromcVec3(*this->view->backgroundColor));
    scene->render(view);
    this->view->_zbuffer = this->view->_emptyZbuffer;
    this->view->update();

    const float fps = 1.0f / deltaTime;
    PGK_Draw::drawText(this->view->canvas, "FPS: " + QString::number(fps), 10, 10, 20, Qt::white);
    
}
