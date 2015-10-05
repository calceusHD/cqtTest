#include "DistMap.h"


DistMap::DistMap(unsigned char *data, unsigned int size, unsigned int rWidth, unsigned int rHeight)
{
    this->size = size;
    this->data = data;
    this->rHeight = rHeight;
    this->rWidth = rWidth;
    this->offX = ( size - rWidth ) / 2;
    this->offY = ( size - rHeight ) / 2;

    if ( rWidth == 0 && rHeight == 0 )
        this->valid = false;
    else
        this->valid = true;
    if ( this->valid )
        createChangeList();
}

DistMap::~DistMap()
{
    //dtor
}

void DistMap::createChangeList()
{
    unsigned int cnt = 0;
    for (unsigned int i = 0;i < this->size;i++)
    {
        for (unsigned int j = 0;j < this->size;j++)
        {

            if ( !checkAround(i, j) )
            {
                cnt++;
                if ( getColor(i, j) )
                    this->whiteList.push_back(Vec2<unsigned int>(i ,j));
                else
                    this->blackList.push_back(Vec2<unsigned int>(i ,j));
            }
        }
    }
}


bool DistMap::checkAround(unsigned int x, unsigned int y)
{
    bool color = getColor(x, y);

    if ( x > 0 )
        if ( color != getColor(x - 1, y) )
            return false;
    if ( y > 0 )
        if ( color != getColor(x, y - 1) )
            return false;
    if ( x < size - 1 )
        if ( color != getColor(x + 1, y) )
            return false;
    if ( y < size - 1 )
        if ( color != getColor(x, y + 1) )
            return false;

    return true;
}

bool DistMap::getColor(unsigned int x, unsigned int y)
{
    x -= offX;
    y -= offY;
    if ( x >= this->size || x >= this->rWidth || y >= this->size || y >= this->rHeight )
        return false;
    else
        return ( this->data[y * this->rWidth + x] > 128 ) ? true : false;
}

float DistMap::findNearest(unsigned int x, unsigned int y)
{
    std::vector<Vec2<unsigned int>> *tmp;
    bool a = getColor(x, y);
    if ( a )
        tmp = &whiteList;
    else
        tmp = &blackList;

    unsigned int idist = this->size * this->size;

    unsigned int cnt = tmp->size();

    Vec2<unsigned int> pos(x, y);



    while (cnt--)
    {

        unsigned int k = (pos - (*tmp)[cnt]).lengthSq();
        if ( k < idist )
            idist = k;
    }
    float dist = sqrt(idist);
    return a ? dist : - dist;
}

float *DistMap::generate(unsigned int sizeo)
{
    float *datao = new float[sizeo * sizeo];


    if ( this->valid )



        for (unsigned int j = 0;j < sizeo;++j)
        {
            for (unsigned int i = 0;i < sizeo;++i)
            {
                datao[j * sizeo + i] = findNearest(i * size / sizeo + size / sizeo / 2, j * size / sizeo + size / sizeo / 2);
            }
        }
    else
        for (unsigned int i = 0;i< sizeo * sizeo;i++)
            datao[i] = 0.0f;
    return datao;
}

float *DistMap::getFullImage()
{
    float *datao = new float[size * size];
    for (unsigned int j = 0; j < size; ++j)
        for (unsigned int i = 0; i < size; ++i)
            datao[j * size + i] = getColor(i, j) ? 1.0f : 0.0f;
    return datao;
}
