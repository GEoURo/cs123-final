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

static float pointLightNear = 0.1f;
static float pointLightFar = 60.0f;
static glm::mat4 dirLightSpaceMatrix = glm::mat4(1.f);

SceneviewScene::SceneviewScene() :
    m_globalData({1, 1, 1, 1}),
    m_dirShadowID(-1),
    m_pointShadowID(-1)
{
    // Set up Sceneview scene
    loadShadow_directionShader();
    loadShadow_pointShader();
    loadPhongShader();

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
    m_dirShadowShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadShadow_pointShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/pointShadow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/pointShadow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/pointShadow.frag");
    m_pointShadowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
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

void SceneviewScene::updateFBO(int w, int h) {
    int shadowMapW = w;
    int shadowMapH = h;

    m_dirShadowMap = make_unique<ShadowMap>(shadowMapW, shadowMapH);
    m_pointShadowMap = make_unique<ShadowCube>(std::max(shadowMapW, shadowMapH));
}

void SceneviewScene::renderShadow(View *context) {
    // go through each light and render the shadow map
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
                    renderPointShadow(m_lights[i]);
                }
                break;
            }
            default:
                break;
        }
    }
    return;
}

void SceneviewScene::renderDirectionShadow(View *context , CS123SceneLightData &light) {
    // put the directional light at some where 50 units away from the origin
    glm::vec3 lightPos = -glm::normalize(glm::vec3(light.dir)) * 50.f;
    // let the orthographic view volume cover twice the size
    float near_plane = 0.1f, far_plane = 75.f;
    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
    // calculate the light space matrix
    dirLightSpaceMatrix = lightProjection * lightView;

    m_dirShadowMap->bind();
    m_dirShadowShader->bind();

    m_dirShadowShader->setUniform("lightSpaceMatrix", dirLightSpaceMatrix);
    glClear(GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < m_primitives.size(); i++) {
        // setup CTM
        m_dirShadowShader->setUniform("model", m_primitiveTrans[i]);

        // draw the primitive
        renderPrimitive(m_primitives[i].type);
    }

    m_dirShadowShader->unbind();
    m_dirShadowMap->unbind();
}

void SceneviewScene::renderPointShadow(CS123SceneLightData &light) {
    // basic config for the light space perspective
    float aspect = 1.f;

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
    m_pointShadowMap->bind();
    m_pointShadowShader->bind();

    // clear depth buffer to prepare for render
    glClear(GL_DEPTH_BUFFER_BIT);

    // set uniform variables
    for (int i = 0; i < 6; i++) {
        m_pointShadowShader->setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    m_pointShadowShader->setUniform("farPlane", pointLightFar);
    m_pointShadowShader->setUniform("lightPos", lightPos);

    // render the scene
    for (size_t i = 0; i < m_primitives.size(); i++) {
        // setup CTM
        m_pointShadowShader->setUniform("model", m_primitiveTrans[i]);

        // draw the primitive
        renderPrimitive(m_primitives[i].type);
    }

    // unbind the shadow map and the shader
    m_pointShadowShader->unbind();
    m_pointShadowMap->unbind();
}

void SceneviewScene::render(View *context) {
    setClearColor();

    // render the primitives
    renderPhongPass(context);
}

void SceneviewScene::renderPhongPass(View *context) {
    m_phongShader->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    setGlobalData();
    setShadowMaps();
    clearLights();
    setLights();
    setSceneUniforms(context);

    renderGeometryAsFilledPolygons();

    m_phongShader->unbind();
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
            m_phongShader->setTexture("diffuseTexture", *(m_textures[i].get()));
        } else {
            m_phongShader->setUniform("useTexture", false);
        }

        // draw the primitive
        renderPrimitive(m_primitives[i].type);

        // unbind the texture
        if (settings.textureMapping && textureMap.isUsed && m_textures[i] != nullptr) {
            m_textures[i]->unbind();
        }
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

void SceneviewScene::setShadowMaps() {
    // global switch
    m_phongShader->setUniform("useShadow", settings.shadowMapping);

    // point light uniforms
    m_phongShader->setUniform("pointLightID", m_pointShadowID);
    m_phongShader->setUniform("pointLightFarPlane", pointLightFar);
    m_phongShader->setTexture("pointLightShadowMap", m_pointShadowMap->getDepthCube());

    // dir light uniforms
    m_phongShader->setUniform("dirLightID", m_dirShadowID);
    m_phongShader->setUniform("dirLightSpaceMat", dirLightSpaceMatrix);
    m_phongShader->setTexture("dirLightShadowMap", m_dirShadowMap->getDepthMap());
}

void SceneviewScene::setLights() {
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
