#ifndef PGK_LAUNCHER_H
#define PGK_LAUNCHER_H

#include "pgk_core.h"

#include <QDialog>
#include <QMenu>
#include <QMenuBar>
#include <QComboBox>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QCheckBox>

class PGK_Launcher : public QDialog
{
    Q_OBJECT
public:
    explicit PGK_Launcher(QWidget *parent = nullptr);

    QString getCoreSettings() const;

private:
    QVBoxLayout mainLayout = QVBoxLayout(this);

    QMenuBar menuBar = QMenuBar(this);
    QMenu menuFile = QMenu("File");
    QAction menuOpenScene = QAction("Open Scene");
    QAction menuExit = QAction("Exit");

    QHBoxLayout settingsLayout = QHBoxLayout();

    QVBoxLayout settingsLeftLayout = QVBoxLayout();
    QComboBox resolutionCBox = QComboBox();
    QComboBox refreshRateCBox = QComboBox();

    QVBoxLayout settingsRightLayout = QVBoxLayout();
    QCheckBox windowedCheck = QCheckBox("Windowed");
    QCheckBox scalingCheck = QCheckBox("Scalable Window");
    QCheckBox texFilterCheck = QCheckBox("Texture Filtering");
    QComboBox shadingModeCBox = QComboBox();
    QCheckBox raycastShadowCheck = QCheckBox("Raycast Shadows");
    QCheckBox renderFogCheck = QCheckBox("Render Fog");

    QListWidget sceneListWidget = QListWidget();

    QHBoxLayout buttonsLayout = QHBoxLayout();
    QPushButton cancelButton = QPushButton("Cancel");
    QPushButton startButton = QPushButton("Start");

    void startScene();
    void openScene();

signals:
};

#endif // PGK_LAUNCHER_H
