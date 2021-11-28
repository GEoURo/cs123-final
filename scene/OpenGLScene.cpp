#include "OpenGLScene.h"

#include <GL/glew.h>

OpenGLScene::~OpenGLScene()
{
}

void OpenGLScene::setClearColor() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
}

void OpenGLScene::settingsChanged() {

}
