#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"

#include <math.h>
#include <QGLFormat>
#include <QFileDialog>
#include <QMessageBox>

#include "utils/CS123XmlSceneParser.h"
#include "utils/Databinding.h"
#include "utils/settings.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup SIGNAL and SLOT connections
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));

    // setup databindings
    setupDataBindings();
}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings) {
        delete b;
    }
    delete ui;
}

void MainWindow::fileOpen() {
    QString file = QFileDialog::getOpenFileName(this, QString(), "/course/cs123/data/");
    if (file.isNull()) {
        return;
    }

    if (!file.endsWith(".xml")) {
        QMessageBox::critical(this, "Error", "Unsupported file extension");
    } else {
        m_sceneParser = std::unique_ptr<CS123XmlSceneParser>(new CS123XmlSceneParser(file.toLatin1().data()));
        if (m_sceneParser->parse()) {
            // TODO: setup the canvas with the scenefile
//            m_canvas3D->loadSceneviewSceneFromParser(*m_sceneParser);

            // Set the camera for the new scene
            CS123SceneCameraData camera;
            if (m_sceneParser->getCameraData(camera)) {
                camera.pos[3] = 1;
                camera.look[3] = 0;
                camera.up[3] = 0;

//                CamtransCamera *cam = m_canvas3D->getCamtransCamera();
//                cam->orientLook(camera.pos, camera.look, camera.up);
//                cam->setHeightAngle(camera.heightAngle);
            }
        } else {
            QMessageBox::critical(this, "Error", "Could not load scene \"" + file + "\"");
        }
    }
}

void MainWindow::settingsChanged() {
    std::cout << settings.antiAliasing << ' ' << settings.depthOfField << ' ' << settings.hdr << ' ' << settings.shadowMapping << std::endl;
}

void MainWindow::setupDataBindings() {
#define BIND(b) { \
    DataBinding *_b = (b); \
    m_bindings.push_back(_b); \
    assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); \
}
    BIND(BoolBinding::bindCheckbox(ui->antiAliasing,    settings.antiAliasing));
    BIND(BoolBinding::bindCheckbox(ui->depthOfField,    settings.depthOfField));
    BIND(BoolBinding::bindCheckbox(ui->hdr,             settings.hdr));
    BIND(BoolBinding::bindCheckbox(ui->shadowMapping,   settings.shadowMapping));

#undef BIND
}
