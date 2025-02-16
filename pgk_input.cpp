#include "pgk_input.h"

PGK_Input& PGK_Input::instance() {
    static PGK_Input instance;
    return instance;
}

bool PGK_Input::getKey(int key) const { return currentKeys.contains(key); }
bool PGK_Input::getKeyDown(int key) const { return currentKeys.contains(key) && !previousKeys.contains(key); }
QPoint PGK_Input::mousePosition() const { return mousePos; }
QPoint PGK_Input::mouseDelta() const { return mouseDeltaValue; }
bool PGK_Input::getMouseButton(int button) const { return mouseButtons.contains(button); }

void PGK_Input::keyPressEvent(int key) { currentKeys.insert(key); }
void PGK_Input::keyReleaseEvent(int key) { currentKeys.remove(key); }
void PGK_Input::mouseMoveEvent(const QPoint& pos) { mousePos = pos; }
void PGK_Input::mousePressEvent(int button) { mouseButtons.insert(button); }
void PGK_Input::mouseReleaseEvent(int button) { mouseButtons.remove(button); }

void PGK_Input::update() {
    if(setPosEvent) return;
    mouseDeltaValue = mousePos - lastMousePos;
    lastMousePos = mousePos;
    previousKeys = currentKeys;
}
