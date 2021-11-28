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
#include "gl/util/TextureManager.h"

#include "glm/gtx/transform.hpp"

#include "shapes/primitives/ConeShape.h"
#include "shapes/primitives/CubeShape.h"
#include "shapes/primitives/CylinderShape.h"
#include "shapes/primitives/SphereShape.h"

using namespace CS123::GL;
using namespace std;
using namespace glm;

SceneviewScene::SceneviewScene() :
    m_globalData({1, 1, 1, 1})
{
    // Set up Sceneview scene
    loadPhongShader();
//    loadWireframeShader();
//    loadNormalsShader();
//    loadNormalsArrowShader();

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

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
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
    m_phongShader->setUniform("useArrowOffsets", false);
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
            m_phongShader->setUniform("useTexture", 1);
            m_phongShader->setUniform("repeatUV", vec2(textureMap.repeatU, textureMap.repeatV));
            m_phongShader->setTexture("tex", *(m_textures[i].get()));
        } else {
            m_phongShader->setUniform("useTexture", 0);
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
