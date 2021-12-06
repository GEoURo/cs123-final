#ifndef SCENEVIEWSCENE_H
#define SCENEVIEWSCENE_H

#include "OpenGLScene.h"
#include "utils/CS123SceneData.h"
#include <memory>
#include <unordered_map>

namespace CS123 { namespace GL {
    class Shader;
    class ShadowMap;
    class ShadowCube;
    class CS123Shader;
    class Texture2D;
}}

class CubeShape;
class ConeShape;
class CylinderShape;
class SphereShape;
class TorusShape;
class TextureManager;
class CS123ISceneParser;

/**
 *
 * @class SceneviewScene
 *
 * A complex scene consisting of multiple objects. Students will implement this class in the
 * Sceneview assignment.
 *
 * Here you will implement your scene graph. The structure is up to you - feel free to create new
 * classes and data structures as you see fit. We are providing this SceneviewScene class for you
 * to use as a stencil if you like.
 *
 * Keep in mind that you'll also be rendering entire scenes in the next two assignments, Intersect
 * and Ray. The difference between this assignment and those that follow is here, we are using
 * OpenGL to do the rendering. In Intersect and Ray, you will be responsible for that.
 */
class SceneviewScene : public OpenGLScene {
public:
    SceneviewScene();
    virtual ~SceneviewScene();

    // render the scene with Phong Illumination model
    virtual void render(View *context) override;

    // render the shadow map of the scene of each light source
    void renderShadow(View *context);

    // settings update function
    virtual void settingsChanged() override;

    // load the scene from a parser
    void loadScene(CS123ISceneParser *parser);

private:
    CS123SceneGlobalData m_globalData;
    std::vector<CS123SceneLightData> m_lights;
    std::vector<CS123ScenePrimitive> m_primitives;
    std::vector<glm::mat4> m_primitiveTrans;

    void dfsParseSceneNode(CS123SceneNode *node, glm::mat4 matrix);

    // load texture
    void loadTextures();

    std::vector<std::shared_ptr<CS123::GL::Texture2D>> m_textures;
    std::unique_ptr<TextureManager> m_textureManager;

    // shader loader functions
    void loadShadow_directionShader();
    void loadShadow_pointShader();
    void loadPhongShader();
    void loadWireframeShader();

    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;
    std::unique_ptr<CS123::GL::Shader> m_wireframeShader;
    std::unique_ptr<CS123::GL::Shader> m_shadow_direcitonShader;
    std::unique_ptr<CS123::GL::Shader> m_shadow_pointShader;

    // shape primitives operation
    void setupPrimitives();
    void updatePrimitives(bool force);

    // gl shapes
    std::unique_ptr<CubeShape> m_cube;
    std::unique_ptr<ConeShape> m_cone;
    std::unique_ptr<CylinderShape> m_cylinder;
    std::unique_ptr<SphereShape> m_sphere;

    void setSceneUniforms(View *context);
    void setMatrixUniforms(CS123::GL::Shader *shader, View *context);

    void setGlobalData();
    void setLights();
    void clearLights();

    void renderDirectionShadow(View *context , CS123SceneLightData &light);
    void renderPointShadow(View *context , CS123SceneLightData &light);
    void renderPhongPass(View *context);
    void renderWireframePass(View *context);

    void renderGeometryAsFilledPolygons();
    void renderGeometryAsWireframe();

    void renderPrimitive(PrimitiveType type);

    // shadow maps
    std::unique_ptr<CS123::GL::ShadowMap> m_dirShadowMap;
    std::unique_ptr<CS123::GL::ShadowCube> m_pointShadowMap;
    int m_dirShadowID;
    int m_pointShadowID;
};

#endif // SCENEVIEWSCENE_H
