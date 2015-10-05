#ifndef FONT_H
#define FONT_H

#include <vector>
#include <atomic>
#include <iostream>
#include <fstream>
#include <ctime>
#include <unordered_map>
#include <iomanip>
#ifdef __WIN32
#include <dir.h>
#include "windows.h"
#else
#include <sys/stat.h>
#endif
#include <thread>
#include <mutex>

#include "GL/glew.h"

#include "GlyphData.h"
#include "FreeType.h"
#include "DistMap.h"
#include "gl/TextureManager.h"
#include "Hash.h"


class Font
{
    struct ThreadData {
    	std::unordered_map<unsigned int, unsigned int>::iterator it;
        Font *me;
        std::atomic<unsigned int> cnt;
        const char *name;
        unsigned int size;
        unsigned int textureSize;
        float *dataOut;

    };
    public:
    	std::mutex ctSec;
    	std::unordered_map<unsigned int, unsigned int> m_charToIndex;
        std::vector<float> _data;
        std::vector<GlyphData> _glyphs;
        unsigned int _size;
        unsigned int _textureNumber;
        const char *_name;
        FreeType m_freeType;

        Font(unsigned int size, const char *name, unsigned int textureSize, TextureManager *textureManager);
        virtual ~Font();

        unsigned int getIdFromChar(unsigned int c);
        void fillGlyph(char id, FT_GlyphSlot data);
        FT_GlyphSlot createGlyph(unsigned int id, unsigned int c, FreeType *ft, bool render);
        void createTexFromGlyph(unsigned int textureSize, FT_GlyphSlot gl, float *totalMap, unsigned int id);
        void createGlyphsThreaded(FreeType *ft, unsigned int textureSize, float *totalMap);
        static uint16_t threadFunc(void* lpParam );
        void populateIndex();
    protected:
    private:
};

#endif // FONT_H
