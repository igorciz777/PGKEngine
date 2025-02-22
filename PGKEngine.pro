QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    pgk_camera.cpp \
    pgk_core.cpp \
    pgk_draw.cpp \
    pgk_engine.cpp \
    pgk_gameobject.cpp \
    pgk_input.cpp \
    pgk_launcher.cpp \
    pgk_light.cpp \
    pgk_math.cpp \
    pgk_obj.cpp \
    pgk_raycast.cpp \
    pgk_rigidbody.cpp \
    pgk_scene.cpp \
    pgk_view.cpp

HEADERS += \
    pgk_camera.h \
    pgk_core.h \
    pgk_draw.h \
    pgk_engine.h \
    pgk_gameobject.h \
    pgk_input.h \
    pgk_launcher.h \
    pgk_light.h \
    pgk_math.h \
    pgk_obj.h \
    pgk_raycast.h \
    pgk_rigidbody.h \
    pgk_scene.h \
    pgk_view.h

# remove other opt flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O
QMAKE_CXXFLAGS_DEBUG -= -O1
QMAKE_CXXFLAGS_DEBUG -= -O2

# add -O3
QMAKE_CXXFLAGS_RELEASE *= -O3
QMAKE_CXXFLAGS_DEBUG *= -O3

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
