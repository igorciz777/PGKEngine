#ifndef PGK_INPUT_H
#define PGK_INPUT_H

#include <QSet>
#include <QPoint>

class PGK_Input {
public:
    static PGK_Input& instance();

    // Keyboard
    bool getKey(int key) const;
    bool getKeyDown(int key) const;

    // Mouse
    QPoint mousePosition() const;
    QPoint mouseDelta() const;
    bool getMouseButton(int button) const;

    // Called from view class
    void keyPressEvent(int key);
    void keyReleaseEvent(int key);
    void mouseMoveEvent(const QPoint& pos);
    void mousePressEvent(int button);
    void mouseReleaseEvent(int button);
    void update();

    bool setPosEvent=false;

private:
    PGK_Input() = default;

    QSet<int> currentKeys;
    QSet<int> previousKeys;
    QSet<int> mouseButtons;
    QPoint mousePos;
    QPoint lastMousePos;
    QPoint mouseDeltaValue;
};

#endif // PGK_INPUT_H
