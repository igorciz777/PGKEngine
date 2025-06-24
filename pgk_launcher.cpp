#include "pgk_launcher.h"

#include <QDir>
#include <QFileDialog>
#include <QLabel>


PGK_Launcher::PGK_Launcher(QWidget *parent)
    : QDialog{parent}
{
    // Layout init
    //mainLayout.addWidget(&menuBar);
    mainLayout.addLayout(&settingsLayout);
    mainLayout.addWidget(&sceneListWidget);
    mainLayout.addLayout(&buttonsLayout);

    menuBar.addMenu(&menuFile);
    menuFile.addAction(&menuOpenScene);
    menuFile.addAction(&menuExit);

    settingsLayout.addLayout(&settingsLeftLayout);
    settingsLayout.addLayout(&settingsRightLayout);

    settingsLeftLayout.addWidget(&resolutionCBox);
    settingsLeftLayout.addWidget(&refreshRateCBox);

    settingsRightLayout.addWidget(&windowedCheck);
    // settingsRightLayout.addWidget(&scalingCheck);
    settingsRightLayout.addWidget(&texFilterCheck);
    settingsRightLayout.addWidget(&raycastShadowCheck);

    QLabel shadingModeLabel("Shading Mode:");
    settingsRightLayout.addWidget(&shadingModeLabel);
    settingsRightLayout.addWidget(&shadingModeCBox);

    settingsRightLayout.setAlignment(Qt::AlignTop);
    windowedCheck.setCheckState(Qt::Checked);
    scalingCheck.setCheckState(Qt::Checked);
    texFilterCheck.setCheckState(Qt::Checked);
    raycastShadowCheck.setCheckState(Qt::Unchecked);

    buttonsLayout.addWidget(&startButton);
    buttonsLayout.addWidget(&cancelButton);
    buttonsLayout.setAlignment(Qt::AlignRight);

    mainLayout.setMenuBar(&menuBar);
    this->setLayout(&mainLayout);

    // Resolution combobox
    resolutionCBox.addItem("320x240");
    resolutionCBox.addItem("426x240");
    resolutionCBox.addItem("640x480");
    resolutionCBox.addItem("852x480");
    resolutionCBox.addItem("800x600");
    resolutionCBox.addItem("1024x768");
    resolutionCBox.addItem("1280x720");
    resolutionCBox.addItem("1280x800");
    resolutionCBox.addItem("1366x768");
    resolutionCBox.addItem("1440x900");
    resolutionCBox.addItem("1600x900");
    resolutionCBox.addItem("1680x1050");
    resolutionCBox.addItem("1920x1080");

    //Set default
    resolutionCBox.setCurrentIndex(2);

    // Refresh rate combobox
    refreshRateCBox.addItem("10Hz");
    refreshRateCBox.addItem("30Hz");
    refreshRateCBox.addItem("60Hz");
    refreshRateCBox.addItem("75Hz");
    refreshRateCBox.addItem("120Hz");
    refreshRateCBox.addItem("144Hz");
    refreshRateCBox.addItem("165Hz");
    refreshRateCBox.addItem("240Hz");

    // Set default
    refreshRateCBox.setCurrentIndex(2);

    // Scene list widget
    sceneListWidget.addItem("Default");
    sceneListWidget.addItem("Helicopter demo");
    sceneListWidget.addItem("Normal mapping demo");
    sceneListWidget.addItem("Lightning demo");
    sceneListWidget.addItem("Stanford Dragon benchmark");

    // Cancel button
    cancelButton.connect(&cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // Start button
    startButton.connect(&startButton, &QPushButton::clicked, this, &QDialog::accept);

    // Menu actions
    menuOpenScene.connect(&menuOpenScene, &QAction::triggered, this, &PGK_Launcher::openScene);
    menuExit.connect(&menuExit, &QAction::triggered, this, &QDialog::reject);

    shadingModeCBox.addItem("Flat");
    shadingModeCBox.addItem("Blinn-Phong");
    shadingModeCBox.addItem("GGX");
    shadingModeCBox.setCurrentIndex(2); // Default to GGX
}

QString PGK_Launcher::getCoreSettings() const
{
    g_pgkCore.RESOLUTION_WIDTH = this->resolutionCBox.currentText().split("x")[0].toInt();
    g_pgkCore.RESOLUTION_HEIGHT = this->resolutionCBox.currentText().split("x")[1].toInt();
    g_pgkCore.REFRESH_RATE = this->refreshRateCBox.currentText().split("H")[0].toInt();
    g_pgkCore.WINDOWED = this->windowedCheck.isChecked();
    g_pgkCore.SCALABLE = this->scalingCheck.isChecked();
    g_pgkCore.TEX_FILTERING = this->texFilterCheck.isChecked();
    g_pgkCore.SHADING_MODE = this->shadingModeCBox.currentIndex(); // Update shading mode
    g_pgkCore.RAYCAST_SHADOWS = this->raycastShadowCheck.isChecked();
    g_pgkCore.ASPECT_RATIO = (float)g_pgkCore.RESOLUTION_WIDTH / (float)g_pgkCore.RESOLUTION_HEIGHT;
    return sceneListWidget.currentItem()->text();
}

void PGK_Launcher::openScene()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Scene", QDir::currentPath() + "/scenes", "Scene files (*.json)");
    if (!fileName.isEmpty())
    {
        sceneListWidget.addItem(fileName);
        sceneListWidget.setCurrentRow(sceneListWidget.count() - 1);
    }
}


