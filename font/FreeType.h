#ifndef FREETYPE_H
#define FREETYPE_H
extern "C" {
#include "ft2build.h"
#include FT_FREETYPE_H
}
class FreeType
{
    public:
        unsigned int _size;
        FT_Library _library;
        FT_Face _face;
        const char *_fontName;

        FreeType(unsigned int size, const char *fontName);

        virtual ~FreeType();
        FT_GlyphSlot createChar(unsigned long code);
        FT_GlyphSlot createCharNoRender(unsigned long code);
        FT_ULong getFirstChar(FT_UInt *gIndex);
        FT_ULong getNextChar(FT_ULong charCode, FT_UInt *gIndex);
    protected:
    private:
};

#endif // FREETYPE_H
