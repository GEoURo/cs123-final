#include "CS123Shader.h"

#include "utils/CS123SceneData.h"
#include "utils/settings.h"


#include "gl/GLDebug.h"
#include <sstream>

namespace CS123 { namespace GL {


CS123Shader::CS123Shader(const std::string &vertexSource, const std::string &fragmentSource) :
    Shader(vertexSource, fragmentSource)
{
}

CS123Shader::CS123Shader(const std::string &vertexSource, const std::string &geometrySource, const std::string &fragmentSource) :
    Shader(vertexSource, geometrySource, fragmentSource)
{
}

glm::vec3 toGLMVec3(const CS123SceneColor &c) {
    return glm::vec3(c.r, c.g, c.b);
}

void CS123Shader::applyMaterial(const CS123SceneMaterial &material) {
    setUniform("material.ambient", toGLMVec3(material.cAmbient));
    setUniform("material.diffuse", toGLMVec3(material.cDiffuse));
    setUniform("material.specular", toGLMVec3(material.cSpecular));
    setUniform("material.shininess", material.shininess);
    setUniform("material.blend", material.blend);
    setUniform("material.repeatUV", glm::vec2(material.textureMap.repeatU, material.textureMap.repeatV));
}

void CS123Shader::setLight(const CS123SceneLightData &light) {
    bool ignoreLight = false;

    GLint lightType;
    std::string name;
    glm::vec3 ndir;
    switch(light.type) {
        case LightType::LIGHT_POINT:
            lightType = 0;
            name = "lightPositions";
            setUniformArrayByIndex(name, light.pos.xyz(), light.id);
            break;
        case LightType::LIGHT_DIRECTIONAL:
            lightType = 1;
            ndir = glm::normalize(light.dir.xyz());
            name = "lightDirections";
            setUniformArrayByIndex(name, ndir, light.id);
            break;
        default:
            lightType = 0;
            ignoreLight = true; // Light type not supported
            break;
    }

    CS123SceneColor color = light.color;
    if (ignoreLight) color.r = color.g = color.b = 0;

    setUniformArrayByIndex("lightTypes", lightType, light.id);
    setUniformArrayByIndex("lightColors", glm::vec3(color.r, color.g, color.b), light.id);
    setUniformArrayByIndex("lightAttenuations", light.function, light.id);
}

}}
