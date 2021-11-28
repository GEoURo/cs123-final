#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

#include "camera/OrbitingCamera.h"
#include "scene/SceneviewScene.h"

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(),
    m_timer(),
    m_captureMouse(false),
    m_camera(new OrbitingCamera()),
    m_scene(nullptr)
{
    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

View::~View()
{
}

Camera* View::getCamera() {
    return m_camera.get();
}

void View::initializeGL() {
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }
    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_camera->updateMatrices();
}

void View::paintGL() {
    if (m_scene == nullptr) {
        return;
    }

    m_scene->render(this);
}

void View::resizeGL(int w, int h) {
    float ratio = w / (float)h;
    m_camera->setAspectRatio(ratio);
    glViewport(0, 0, w, h);
}

void View::loadFromParser(CS123ISceneParser *parser) {
    m_scene = std::make_unique<SceneviewScene>();
    m_scene->loadScene(parser);
}

void View::settingsChanged() {
    if (m_scene == nullptr) {
        return;
    }

    m_scene->settingsChanged();
    update();
}

void View::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        m_camera->mouseDown(event->x(), event->y());
        m_captureMouse = true;

        update();
    }
}

void View::mouseMoveEvent(QMouseEvent *event) {
    // This starter code implements mouse capture, which gives the change in
    // mouse position since the last mouse movement. The mouse needs to be
    // recentered after every movement because it might otherwise run into
    // the edge of the screen, which would stop the user from moving further
    // in that direction. Note that it is important to check that deltaX and
    // deltaY are not zero before recentering the mouse, otherwise there will
    // be an infinite loop of mouse move events.
    if(m_captureMouse) {
        m_camera->mouseDragged(event->x(), event->y());

        update();
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {
    if (m_captureMouse && event->button() == Qt::RightButton) {
        m_camera->mouseUp(event->x(), event->y());
        m_captureMouse = false;

        update();
    }
}

void View::wheelEvent(QWheelEvent *event) {
    m_camera->mouseScrolled(event->delta());
    update();
}

void View::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W){
        std::cout<<"you pressed W!"<<std::endl;
        m_camera->moveForward();
        update();
    }

    if (event->key() == Qt::Key_S){
        std::cout<<"you pressed W!"<<std::endl;
        m_camera->moveBackward();
        update();
    }

    if (event->key() == Qt::Key_D){
        std::cout<<"you pressed W!"<<std::endl;
        m_camera->moveRight();
        update();
    }
    if (event->key() == Qt::Key_A){
        std::cout<<"you pressed W!"<<std::endl;
        m_camera->moveLeft();
        update();
    }

}

void View::keyReleaseEvent(QKeyEvent *event) {

}

void View::tick() {
    // Get the number of seconds since the last tick (variable update rate)
//    float seconds = m_time.restart() * 0.001f;

    // TODO: Implement the demo update here

    // Flag this view for repainting (Qt will call paintGL() soon after)
//    update();
}
