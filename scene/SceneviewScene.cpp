#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include <sstream>

#include "view.h"

#include "camera/Camera.h"

#include "utils/settings.h"
#include "utils/CS123XmlSceneParser.h"
#include "utils/ResourceLoader.h"

#include "gl/shaders/CS123Shader.h"
#include "gl/textures/Texture2D.h"
#include "gl/textures/ShadowMap.h"
#include "gl/textures/ShadowCube.h"
#include "gl/util/TextureManager.h"

#include "glm/gtx/transform.hpp"

#include "shapes/primitives/ConeShape.h"
#include "shapes/primitives/CubeShape.h"
#include "shapes/primitives/CylinderShape.h"
#include "shapes/primitives/SphereShape.h"

using namespace CS123::GL;
using namespace std;
using namespace glm;

static float pointLightNear = 1.0f;
static float pointLightFar = 25.0f;

SceneviewScene::SceneviewScene() :
    m_globalData({1, 1, 1, 1}),
    m_dirShadowID(-1),
    m_pointShadowID(-1)
{
    // Set up Sceneview scene
    loadShadow_directionShader();
    loadShadow_pointShader();
    loadPhongShader();
//    loadWireframeShader();

    // setup texture manager
    m_textureManager = std::unique_ptr<TextureManager>(new TextureManager());

    setupPrimitives();
    updatePrimitives(true);
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadScene(CS123ISceneParser *parser) {
    // clear historical data
    m_primitives.clear();
    m_primitiveTrans.clear();

    // load global data from the parser
    CS123SceneGlobalData globalData;
    parser->getGlobalData(globalData);
    m_globalData = globalData;

    // obtain the number of lights
    int numLights = parser->getNumLights();

    for (int i = 0; i < numLights; i++) {
        CS123SceneLightData lightData;
        parser->getLightData(i, lightData);
        m_lights.push_back(lightData);
    }

    CS123SceneNode *root = parser->getRootNode();
    glm::mat4 matrix(1.0f);
    dfsParseSceneNode(root, matrix);

    loadTextures();
}

void SceneviewScene::loadTextures() {
    // load the textures
    m_textures.resize(m_primitives.size());

    for (size_t i = 0; i < m_primitives.size(); i++) {
        if (m_primitives[i].material.textureMap.isUsed) {
            string filename = m_primitives[i].material.textureMap.filename;
            m_textures[i] = m_textureManager->getGLTexture(filename);
        } else {
            m_textures[i] = nullptr;
        }
    }
}


void SceneviewScene::dfsParseSceneNode(CS123SceneNode *node, glm::mat4 matrix) {
    if (node == nullptr) {
        return;
    }

    for (CS123SceneTransformation *transform : node->transformations) {
        glm::mat4 transMatrix;
        switch (transform->type) {
        case TRANSFORMATION_TRANSLATE:
        {
            transMatrix = glm::translate(transform->translate);
            break;
        }
        case TRANSFORMATION_SCALE:
        {
            transMatrix = glm::scale(transform->scale);
            break;
        }
        case TRANSFORMATION_ROTATE:
        {
            transMatrix = glm::rotate(transform->angle, transform->rotate);
            break;
        }
        case TRANSFORMATION_MATRIX:
        {
            transMatrix = transform->matrix;
            break;
        }
        default:
            ;
        }
        matrix = matrix * transMatrix;
    }

    for (CS123ScenePrimitive * primitive : node->primitives) {
        CS123ScenePrimitive copyPrimitive = *primitive;
        m_primitives.push_back(copyPrimitive);
        m_primitiveTrans.push_back(matrix);
    }

    for (CS123SceneNode * child : node->children) {
        dfsParseSceneNode(child, matrix);
    }
    return;
}

void SceneviewScene::loadShadow_directionShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/shadowmap.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/shadowmap.frag");
    m_shadow_direcitonShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadShadow_pointShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/pointShadow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/pointShadow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/pointShadow.frag");
    m_shadow_pointShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::setupPrimitives() {
    // setup all the shape primitives
    m_cone = make_unique<ConeShape>(settings.shapeParameter1, settings.shapeParameter2);
    m_cube = make_unique<CubeShape>(settings.shapeParameter1);
    m_cylinder = make_unique<CylinderShape>(settings.shapeParameter1, settings.shapeParameter2);
    m_sphere = make_unique<SphereShape>(settings.shapeParameter1, settings.shapeParameter2);
}

void SceneviewScene::updatePrimitives(bool force) {
    int param1 = settings.shapeParameter1;
    int param2 = settings.shapeParameter2;
    float param3 = settings.shapeParameter3;

    // update all the shape primitives if needed
    m_cube->update(param1, param2, param3, force);
    m_cone->update(param1, param2, param3, force);
    m_cylinder->update(param1, param2, param3, force);
    m_sphere->update(param1, param2, param3, force);
}

void SceneviewScene::settingsChanged() {
    updatePrimitives(false);
}

void SceneviewScene::renderShadow(View *context) {
    // go through each light and render the shadow map
    int w = context->width();
    int h = context->height();

    int shadowMapW = w * 4;
    int shadowMapH = h * 4;

    m_dirShadowMap = make_unique<ShadowMap>(shadowMapW, shadowMapH);
    m_pointShadowMap = make_unique<ShadowCube>(std::max(shadowMapW, shadowMapH));

    m_dirShadowID = -1;
    m_pointShadowID = -1;

    for (size_t i = 0; i < m_lights.size(); i++) {
        // render the shadow in the light's direction
        switch (m_lights[i].type) {
            case LightType::LIGHT_DIRECTIONAL:
            {
                if (m_dirShadowID == -1 && m_lights[i].id < 10) {
                    // only consider the first encountered light
                    m_dirShadowID = m_lights[i].id;
                    renderDirectionShadow(context, m_lights[i]);
                }
                break;
            }
            case LightType::LIGHT_POINT:
            {
                if (m_pointShadowID == -1 && m_lights[i].id < 10) {
                    // only consider the first encountered light
                    m_pointShadowID = m_lights[i].id;
                    renderPointShadow(context, m_lights[i]);
                }
                break;
            }
            default:
                break;
        }
    }
    return;
}

void SceneviewScene::render(View *context) {
    setClearColor();

    // render the primitives
    renderPhongPass(context);

    // render the wireframes
    if (settings.drawWireframe) {
        renderWireframePass(context);
    }
}

void SceneviewScene::setGlobalData(){
    // pass global data to shader.vert using m_phongShader
    m_phongShader->setUniform("ka", m_globalData.ka);
    m_phongShader->setUniform("kd", m_globalData.kd);
    m_phongShader->setUniform("ks", m_globalData.ks);
}

void SceneviewScene::setSceneUniforms(View *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("cameraPos", camera->getPosition());
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, View *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //

    for (CS123SceneLightData &light : m_lights) {
        m_phongShader->setLight(light);
    }
}

void SceneviewScene::clearLights() {
    for (int i = 0; i < MAX_NUM_LIGHTS; i++) {
        std::ostringstream os;
        os << i;
        std::string indexString = "[" + os.str() + "]"; // e.g. [0], [1], etc.
        m_phongShader->setUniform("lightColors" + indexString, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void SceneviewScene::renderDirectionShadow(View *context , CS123SceneLightData &light) {
    glm::mat4 lightProjection, lightView, lightSpaceMatrix;
    float near_plane = 1.f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(glm::vec3(-light.dir),
                                      glm::vec3( 0.0f, 0.0f,  0.0f),
                                      glm::vec3( 0.0f, 1.0f,  0.0f));
    lightSpaceMatrix = lightProjection * lightView;

    m_dirShadowMap->bind();
    m_shadow_direcitonShader->bind();

    m_shadow_direcitonShader->setUniform("lightSpaceMatrix", lightSpaceMatrix);
    glClear(GL_DEPTH_BUFFER_BIT);

    renderScene_directionShadow();

    m_shadow_direcitonShader->unbind();
    m_dirShadowMap->unbind();
}

void SceneviewScene::renderPointShadow(View *context , CS123SceneLightData &light) {
    int shadowW = context->width() * 4;
    int shadowH = context->height() * 4;
    // basic config for the light space perspective
    float aspect = (float)shadowW/(float)shadowH;

    glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), aspect, pointLightNear, pointLightFar);
    vec3 lightPos = light.pos.xyz();
    // setup shadow transformation for each face
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProjection * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    // bind the shadow map and the shader
    m_dirShadowMap->bind();
    m_shadow_pointShader->bind();

    // set uniform variables
    for (int i = 0; i < 6; i++) {
        m_shadow_pointShader->setUniform("shadowmatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    m_shadow_pointShader->setUniform("farPlane", pointLightFar);
    m_shadow_pointShader->setUniform("lightPos", lightPos);

    // render the scene
    for (size_t i = 0; i < m_primitives.size(); i++) {
        // setup CTM
        m_shadow_pointShader->setUniform("model", m_primitiveTrans[i]);

        // draw the primitive
        renderPrimitive(m_primitives[i].type);
    }

    // unbind the shadow map and the shader
    m_shadow_pointShader->unbind();
    m_dirShadowMap->unbind();
}

void SceneviewScene::renderPhongPass(View *context) {
    m_phongShader->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setGlobalData();
    clearLights();
    setLights();
    setSceneUniforms(context);
    setMatrixUniforms(m_phongShader.get(), context);

    renderGeometryAsFilledPolygons();

    m_phongShader->unbind();
}

void SceneviewScene::renderWireframePass(View *context) {
    m_wireframeShader->bind();

    setMatrixUniforms(m_wireframeShader.get(), context);
    renderGeometryAsWireframe();

    m_wireframeShader->unbind();
}

void SceneviewScene::renderScene_directionShadow() {
    // setup polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (size_t i = 0; i < m_primitives.size(); i++) {

        // setup CTM
        m_shadow_direcitonShader->setUniform("model", m_primitiveTrans[i]);

        // draw the primitive
        renderPrimitive(m_primitives[i].type);
    }
}

void SceneviewScene::renderGeometryAsFilledPolygons() {
    // setup polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for (size_t i = 0; i < m_primitives.size(); i++) {
        // setup the material
        CS123SceneMaterial material = m_primitives[i].material;
        material.cSpecular = material.shininess == 0 ? vec4(0.f) : material.cSpecular;
        m_phongShader->applyMaterial(material);

        // setup CTM
        m_phongShader->setUniform("m", m_primitiveTrans[i]);

        // setup repeatUV for texture mapping
        CS123SceneFileMap textureMap = m_primitives[i].material.textureMap;
        if (settings.textureMapping && textureMap.isUsed && m_textures[i] != nullptr) {
            // setup texture and bind the texture
            m_phongShader->setUniform("useTexture", true);
            m_phongShader->setTexture("tex", *(m_textures[i].get()));
        } else {
            m_phongShader->setUniform("useTexture", false);
        }

        // setup shadow mapping uniforms
        if (!settings.shadowMapping) {
            m_phongShader->setUniform("useShadow", false);
        } else {
            // global switch
            m_phongShader->setUniform("useShadow", true);
            m_phongShader->setUniform("pointLightID", m_pointShadowID);
            if (m_pointShadowID != -1) {
                // if id is -1, then it means no shadow map for point light
                m_phongShader->setUniform("pointLightFarPlane", pointLightFar);
                m_phongShader->setTexture("pointLightShadowMap", m_pointShadowMap->getDepthCube());
            }
            // point light uniforms

            // dir light uniforms
            // TODO:
            //m_phongShader->setUniform("dirLightSpaceMat", lightSpaceMatrix);
            //m_phongShader->setTexture("dirLightShadowMap", m_dirShadowMap->getDepthMap());
        }

        // draw the primitive
        renderPrimitive(m_primitives[i].type);

        // unbind the texture
        if (settings.textureMapping && textureMap.isUsed && m_textures[i] != nullptr) {
            m_textures[i]->unbind();
        }
    }
}

void SceneviewScene::renderGeometryAsWireframe() {
    // setup polygon mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (size_t i = 0; i < m_primitives.size(); i++) {
        // setup CTM
        m_wireframeShader->setUniform("m", m_primitiveTrans[i]);

        // draw primitive
        renderPrimitive(m_primitives[i].type);
    }
}

void SceneviewScene::renderPrimitive(PrimitiveType type) {
    switch (type) {
        case PrimitiveType::PRIMITIVE_CONE:
        {
            m_cone->draw();
            break;
        }
        case PrimitiveType::PRIMITIVE_CUBE:
        {
            m_cube->draw();
            break;
        }
        case PrimitiveType::PRIMITIVE_CYLINDER:
        {
            m_cylinder->draw();
            break;

        }
        case PrimitiveType::PRIMITIVE_SPHERE:
        {
            m_sphere->draw();
            break;
        }
        case PrimitiveType::PRIMITIVE_TORUS:
        case PrimitiveType::PRIMITIVE_MESH:
        default:
        {
            m_cube->draw();
            break;
        }
    }
}
