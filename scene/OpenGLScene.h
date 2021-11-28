#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

// Maximum number of lights, as defined in shader.
const int MAX_NUM_LIGHTS = 10;

class View;

/**
 * @class  OpenGLScene
 *
 * Basic Scene abstract class that supports OpenGL. Students will extend this class in ShapesScene
 * and SceneviewScene.
 */
class OpenGLScene {
public:
    virtual ~OpenGLScene();
    virtual void settingsChanged() = 0;
    virtual void render(View *context) = 0;

protected:

    void setClearColor();
};

#endif // OPENGLSCENE_H
