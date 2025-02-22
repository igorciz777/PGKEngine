#include "pgk_view.h"
#include "pgk_core.h"
#include "pgk_input.h"
#include <QPainter>
#include <QKeyEvent>

PGK_View::PGK_View(QWidget *parent) : QWidget(parent)
{
    aspectRatio = g_pgkCore.ASPECT_RATIO;
    resWidth = g_pgkCore.RESOLUTION_WIDTH;
    resHeight = g_pgkCore.RESOLUTION_HEIGHT;
    refreshRate = g_pgkCore.REFRESH_RATE;
    scalable = g_pgkCore.SCALABLE;

    _zbuffer = std::vector<float>(resWidth*resHeight,std::numeric_limits<float>::lowest());
    _emptyZbuffer = std::vector<float>(resWidth*resHeight,std::numeric_limits<float>::lowest());

    canvas = QImage(resWidth, resHeight, QImage::Format_RGB32);
    this->resize(resWidth,resHeight);
    this->setMouseTracking(true);
}

PGK_View::~PGK_View() 
{

}

void PGK_View::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    if(scalable){
        screen = canvas.copy().scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
        painter.drawImage(0,0,screen);
    }else{
        painter.drawImage(0,0,canvas);
    }

    painter.end();
}

void PGK_View::keyPressEvent(QKeyEvent* event) {
    PGK_Input::instance().keyPressEvent(event->key());
    QWidget::keyPressEvent(event);
}

void PGK_View::keyReleaseEvent(QKeyEvent* event) {
    PGK_Input::instance().keyReleaseEvent(event->key());
    QWidget::keyReleaseEvent(event);
}

void PGK_View::mouseMoveEvent(QMouseEvent* event) {
    PGK_Input::instance().mouseMoveEvent(event->pos());
    QWidget::mouseMoveEvent(event);
}

void PGK_View::mousePressEvent(QMouseEvent* event) {
    PGK_Input::instance().mousePressEvent(event->button());
    QWidget::mousePressEvent(event);
}

void PGK_View::mouseReleaseEvent(QMouseEvent* event) {
    PGK_Input::instance().mouseReleaseEvent(event->button());
    QWidget::mouseReleaseEvent(event);
}

void PGK_View::focusOutEvent(QFocusEvent* event) {
    PGK_Input::instance().keyReleaseEvent(Qt::Key_unknown);
    QWidget::focusOutEvent(event);
}
