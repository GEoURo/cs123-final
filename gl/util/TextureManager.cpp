#include "TextureManager.h"
#include <iostream>
#include "gl/textures/Texture2D.h"
#include "gl/textures/TextureParametersBuilder.h"

using namespace std;
using namespace CS123::GL;

TextureManager::TextureManager() :
    m_textures(unordered_map<string, weak_ptr<QImage>>())
{

}

TextureManager::~TextureManager() {
    cout << "TEXTUREMANAGER DESTRUCTOR" << endl;
}

std::shared_ptr<QImage> TextureManager::getTextureImage(std::string fileName) {
    if (m_textures.count(fileName) && !m_textures[fileName].expired()) {
        return m_textures[fileName].lock();
    }


    QString qFileName = QString(fileName.c_str());
    shared_ptr<QImage> ret = make_shared<QImage>(qFileName);
    if (ret == nullptr || ret->isNull()) {
        cout << "load texture image fail:" << fileName << endl;
        return nullptr;
    }

    weak_ptr<QImage> weakRef(ret);
    m_textures[fileName] = weakRef;
    return ret;
}

std::shared_ptr<Texture2D> TextureManager::getGLTexture(std::string fileName) {
    if (m_glTextures.count(fileName) && !m_glTextures[fileName].expired()) {
        return m_glTextures[fileName].lock();
    }


    QString qFileName = QString(fileName.c_str());
    QImage textureImage(qFileName);

    if (textureImage.isNull()) {
        cout << "load texture image fail:" << fileName << endl;
        return nullptr;
    }
    textureImage = textureImage.convertToFormat(QImage::Format::Format_RGBA8888);

    shared_ptr<Texture2D> ret = make_shared<Texture2D>(textureImage.bits(), textureImage.width(), textureImage.height());

    // apply texture parameters
    TextureParametersBuilder builder;
    builder.setFilter(TextureParameters::FILTER_METHOD::LINEAR);
    builder.setWrap(TextureParameters::WRAP_METHOD::REPEAT);

    builder.build().applyTo(*ret);

    weak_ptr<Texture2D> weakRef(ret);
    m_glTextures[fileName] = weakRef;
    return ret;
}
