#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <memory>

class Camera;
class OrbitingCamera;
class SceneviewScene;
class CS123ISceneParser;

class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

    Camera *getCamera();

    void loadFromParser(CS123ISceneParser *parser);
    virtual void settingsChanged();

private:
    QTime m_time;
    QTimer m_timer;

    bool m_settingsDirty;
    bool m_captureMouse;

    std::unique_ptr<OrbitingCamera> m_camera;
    std::unique_ptr<SceneviewScene> m_scene;

    // Overridden from QGLWidget
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // Overridden from QWidget
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void tick();
};

#endif // VIEW_H
