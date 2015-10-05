#include "GlyphData.h"

GlyphData::GlyphData()
{
}

void GlyphData::set(Vec2<float> advance, Vec2<float> size, Vec2<float> offset)
{
    this->advance = advance;
    this->size = size;
    this->offset = offset;
}

GlyphData::~GlyphData()
{
    //dtor
}
