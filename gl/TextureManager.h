#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H


#include <vector>
#include <GL/glew.h>

class TextureManager
{
    public:
        std::vector<int> _usedTextures;
        int _nextTexture;
        int _maxTextures;

        int getFreeTexture();
        int findFirstTexture();

        TextureManager();
        virtual ~TextureManager();
    protected:
    private:
};

#endif // TEXTUREMANAGER_H
