#ifndef PGK_VIEW_H
#define PGK_VIEW_H


#include "pgk_math.h"
#include <QWidget>
#include <QHBoxLayout>

class PGK_View : public QWidget
{
    Q_OBJECT

public:
    PGK_View(QWidget *parent = nullptr);
    ~PGK_View();

    QImage canvas;
    QImage screen;
    std::shared_ptr<cVec3> backgroundColor = std::make_shared<cVec3>(80,80,80);
    float aspectRatio;
    int resWidth;
    int resHeight;
    float nearClip = 0.1f;
    float farClip = 300.f;
    float refreshRate;
    bool scalable = false;
    std::vector<float> _zbuffer;
    std::vector<float> _emptyZbuffer;

    void lockMouse() {
        setCursor(Qt::BlankCursor);
        QCursor::setPos(mapToGlobal(QPoint(width() / 2, height() / 2)));
    }

    void unlockMouse() {
        setCursor(Qt::ArrowCursor);
    }
private:
    QHBoxLayout mainLayout = QHBoxLayout();
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
};
#endif // PGK_VIEW_H
