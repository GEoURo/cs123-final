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

namespace CS123 { namespace GL {
class FBO;
class Shader;
class FullScreenQuad;
}}

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

    bool m_captureMouse;

    int m_fboW;
    int m_fboH;

    std::unique_ptr<OrbitingCamera> m_camera;
    std::unique_ptr<SceneviewScene> m_scene;

    std::unique_ptr<CS123::GL::FBO> m_colorBuffer;

    std::unique_ptr<CS123::GL::FBO> m_toneMappingBuffer;
    std::unique_ptr<CS123::GL::Shader> m_toneMappingShader;

    std::unique_ptr<CS123::GL::Shader> m_quadShader;
    std::unique_ptr<CS123::GL::FullScreenQuad> m_quad;


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

    void loadQuadShader();
    void loadToneMappingShader();
    void updateFBO();

private slots:
    void tick();
};

#endif // VIEW_H
