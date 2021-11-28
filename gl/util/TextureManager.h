#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <memory>
#include <unordered_map>
#include <QImage>

namespace CS123 { namespace GL {
    class Texture2D;
}}

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    std::shared_ptr<QImage> getTextureImage(std::string fileName);

    std::shared_ptr<CS123::GL::Texture2D> getGLTexture(std::string fileName);

private:
    std::unordered_map<std::string, std::weak_ptr<QImage>> m_textures;
    std::unordered_map<std::string, std::weak_ptr<CS123::GL::Texture2D>> m_glTextures;

};

#endif // TEXTUREMANAGER_H
