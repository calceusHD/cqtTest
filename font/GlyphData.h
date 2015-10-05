#ifndef GLYPHDATA_H
#define GLYPHDATA_H

#include "Vec2.h"

class GlyphData
{
    public:
        Vec2<float> advance;
        Vec2<float> size;
        Vec2<float> offset;
        GlyphData();
        void set(Vec2<float> advance, Vec2<float> size, Vec2<float> offset);
        virtual ~GlyphData();
    protected:
    private:
};

#endif // GLYPHDATA_H
