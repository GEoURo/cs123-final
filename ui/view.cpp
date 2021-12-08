#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>

#include "gl/datatype/VAO.h"
#include "gl/datatype/FBO.h"
#include "gl/shaders/Shader.h"
#include "gl/textures/Texture2D.h"
#include "gl/util/FullScreenQuad.h"

#include "camera/OrbitingCamera.h"
#include "scene/SceneviewScene.h"

#include "utils/ResourceLoader.h"
#include "utils/settings.h"

using namespace std;
using namespace CS123::GL;

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
//    m_time.start();
//    m_timer.start(1000 / 60);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // setup default color FBO
    int w = width();
    int h = height();

    float ratio = w / (float)h;
    m_camera->setAspectRatio(ratio);
    m_camera->updateMatrices();

#ifdef __APPLE__
    m_fboW = w * 2;
    m_fboH = h * 2;
#else
    m_fboW = w;
    m_fboH = h;
#endif

    updateFBO();

    // config quad
    m_quad = make_unique<FullScreenQuad>();

    // load shaders
    loadQuadShader();
    loadToneMappingShader();
}

void View::loadQuadShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenquad.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenquad.frag");
    m_quadShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void View::loadToneMappingShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/fullscreenquad.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/tonemapping.frag");
    m_toneMappingShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void View::updateFBO() {
    m_colorBuffer = make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::DEPTH_ONLY, m_fboW, m_fboH,
                                     TextureParameters::WRAP_METHOD::REPEAT,
                                     TextureParameters::FILTER_METHOD::LINEAR,
                                     GL_FLOAT);

    m_toneMappingBuffer = make_unique<FBO>(1, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, m_fboW, m_fboH,
                                           TextureParameters::WRAP_METHOD::REPEAT,
                                           TextureParameters::FILTER_METHOD::LINEAR,
                                           GL_FLOAT);

    // make the shadow map twice the size of the screen size
    if (m_scene) { m_scene->updateFBO(m_fboW * 2, m_fboH * 2); }
}

void View::paintGL() {
    if (m_scene == nullptr) {
        return;
    }

    // let the scene prepare the shadow data if needed
    if (settings.shadowMapping) {
        m_scene->renderShadow(this);
//#define DEBUG_DIRECTION_SHADOW
#ifdef DEBUG_DIRECTION_SHADOW
        m_scene->renderDirectionShadowMapDEBUG(this);
        return;
#endif

//#define DEBUG_POINT_SHADOW
#ifdef DEBUG_POINT_SHADOW
        m_scene->renderPointShadowMapDEBUG(this);
        return;
#endif
    }

    // render the scene into the color buffer
    m_colorBuffer->bind();
    m_scene->render(this);
    m_colorBuffer->unbind();

    // render the hdr tone mapping pass
    m_toneMappingBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_toneMappingShader->bind();
    m_toneMappingShader->setUniform("exposure", settings.exposure);
    m_toneMappingShader->setUniform("gamma", settings.gamma);
    m_toneMappingShader->setUniform("hdrEnabled", settings.hdr);
    m_toneMappingShader->setTexture("colorAttachment", m_colorBuffer->getColorAttachment(0));

    m_quad->draw();

    m_toneMappingBuffer->unbind();

    // draw the content of the color buffer using fullscreen quad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef __APPLE__
    glViewport(0, 0, width() * 2, height() * 2);
#else
    glViewport(0, 0, width(), height());
#endif

    m_quadShader->bind();
    m_quadShader->setTexture("colorAttachment", m_toneMappingBuffer->getColorAttachment(0));

    m_quad->draw();

    m_quadShader->unbind();
}

void View::resizeGL(int w, int h) {
    float ratio = w / (float)h;
    m_camera->setAspectRatio(ratio);
    m_fboW = w;
    m_fboH = h;

    updateFBO();
}

void View::loadFromParser(CS123ISceneParser *parser) {
    m_scene = std::make_unique<SceneviewScene>();
    m_scene->loadScene(parser);
    // make the shadow map twice the size of the screen size
    m_scene->updateFBO(m_fboW * 2, m_fboH * 2);
    m_camera->initializeValues();
    update();
}

void View::settingsChanged() {
    if (m_scene == nullptr) {
        return;
    }

    m_scene->settingsChanged();
    m_camera->updateMatrices();
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
        m_camera->moveForward();
        update();
    }

    if (event->key() == Qt::Key_S){
        m_camera->moveBackward();
        update();
    }

    if (event->key() == Qt::Key_D){
        m_camera->moveRight();
        update();
    }

    if (event->key() == Qt::Key_A){
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
