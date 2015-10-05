#include "TextureManager.h"

TextureManager::TextureManager() :
    _nextTexture(0)
{
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &_maxTextures);
}

TextureManager::~TextureManager()
{
    //dtor
}

int TextureManager::getFreeTexture()
{
    int texOut = 0;
    for (int i : _usedTextures)
    {
        if ( i == _nextTexture )
        {
            texOut = findFirstTexture();
            break;
        }
    }
    if ( texOut == 0 )
    {
        texOut = _nextTexture;
        _nextTexture++;
        if (_nextTexture >= _maxTextures)
            _nextTexture = 0;
    }
    return texOut;
}

int TextureManager::findFirstTexture()
{
    int lastUsed = 0;
    for (int i : _usedTextures)
    {
        if ( i == lastUsed )
        {
            lastUsed = i + 1;
        }
    }
    return lastUsed;
}
