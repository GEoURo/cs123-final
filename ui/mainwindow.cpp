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
    connect(ui->resetCameraButton, SIGNAL(clicked()), this, SLOT(resetCamera()));

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
    QString file = QFileDialog::getOpenFileName(this, QString(), "./scenefiles/scenes");
    if (file.isNull()) {
        return;
    }

    if (!file.endsWith(".xml")) {
        QMessageBox::critical(this, "Error", "Unsupported file extension");
    } else {
        m_sceneParser = std::unique_ptr<CS123XmlSceneParser>(new CS123XmlSceneParser(file.toLatin1().data()));
        if (m_sceneParser->parse()) {
            // setup the canvas with the scenefile
            ui->view->loadFromParser(m_sceneParser.get());
            ui->view->update();
        } else {
            QMessageBox::critical(this, "Error", "Could not load scene \"" + file + "\"");
        }
    }
}

void MainWindow::resetCamera() {
    ui->view->resetCamera();
}


void MainWindow::settingsChanged() {
    ui->view->settingsChanged();
}

void MainWindow::setupDataBindings() {
#define BIND(b) { \
    DataBinding *_b = (b); \
    m_bindings.push_back(_b); \
    assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); \
}
    BIND(BoolBinding::bindCheckbox(ui->depthOfField,    settings.depthOfField));
    BIND(BoolBinding::bindCheckbox(ui->hdr,             settings.hdr));
    BIND(BoolBinding::bindCheckbox(ui->shadowMapping,   settings.shadowMapping));
    BIND(BoolBinding::bindCheckbox(ui->textureMapping,  settings.textureMapping));

    BIND(IntBinding::bindSliderAndTextbox(ui->shapeSlider1, ui->shapeParam1, settings.shapeParameter1, 0, 100));
    BIND(IntBinding::bindSliderAndTextbox(ui->shapeSlider2, ui->shapeParam2, settings.shapeParameter2, 0, 100));
    BIND(FloatBinding::bindSliderAndTextbox(ui->shapeSlider3, ui->shapeParam3, settings.shapeParameter3, 0.f, 100.f));

    BIND(FloatBinding::bindSliderAndTextbox(ui->fovSlider, ui->fovInput, settings.cameraFov, 1.f, 179.f));
    BIND(FloatBinding::bindSliderAndTextbox(ui->nearSlider, ui->nearInput, settings.cameraNear, 0.1f, 50.f));
    BIND(FloatBinding::bindSliderAndTextbox(ui->farSlider, ui->farInput, settings.cameraFar, 0.1f, 50.f));
    BIND(FloatBinding::bindSliderAndTextbox(ui->dofSlider, ui->dofInput, settings.cameraDof, 0.f, 30.f));

    BIND(FloatBinding::bindSliderAndTextbox(ui->exposureSlider, ui->exposureInput, settings.exposure, 0.1f, 10.f));
    BIND(FloatBinding::bindSliderAndTextbox(ui->gammaSlider, ui->gammaInput, settings.gamma, 0.1f, 5.f));

    initializeCameraFrustum();
    initializeHDRSettings();

#undef BIND
}

void MainWindow::initializeCameraFrustum() {
    ui->fovInput->setText(QString::number(55, 'f', 1));
    ui->nearInput->setText(QString::number(.1, 'f', 1));
    ui->farInput->setText(QString::number(50, 'f', 1));
}

void MainWindow::initializeHDRSettings() {
    ui->exposureInput->setText(QString::number(1, 'f', 1));
    ui->gammaInput->setText(QString::number(2.2, 'f', 1));
}
