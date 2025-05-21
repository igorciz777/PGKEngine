#include "pgk_launcher.h"
#include "pgk_view.h"
#include "pgk_core.h"
#include "pgk_scene.h"
#include "pgk_engine.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    PGK_Launcher launcher;
    if (launcher.exec() == QDialog::Accepted) {
        QString scenePath = launcher.getCoreSettings();

        PGK_View view(nullptr);
        if (g_pgkCore.WINDOWED) {
            view.show();
        } else {
            view.showFullScreen();
        }
        view.lockMouse();

        PGK_Scene scene(scenePath);
        PGK_Engine engine(&scene,&view);

        return a.exec();

    }

    return 0;
}

