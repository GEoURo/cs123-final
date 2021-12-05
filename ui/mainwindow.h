#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

class CS123XmlSceneParser;
class DataBinding;
class View;

namespace Ui {
    class MainWindow;
}

/**
 * @class MainWindow
 *
 * The main graphical user interface class (GUI class) for our application.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    // Displays a dialog box to open a 3D scene file.
    void fileOpen();

    // A callback function to indicate certain value changes in the settings
    void settingsChanged();

private:
    // Auto-generated by Qt. DO NOT RENAME!
    Ui::MainWindow *ui;
    QList<DataBinding*> m_bindings;

    std::unique_ptr<CS123XmlSceneParser> m_sceneParser;

    void setupDataBindings();
    void initializeCameraFrustum();
    void initializeHDRSettings();
};

#endif // MAINWINDOW_H
