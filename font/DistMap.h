#ifndef DISTMAP_H
#define DISTMAP_H

#include <vector>
#include <iostream>
#include "Vec2.h"

class DistMap
{
    public:
        unsigned int size;
        unsigned char *data;
        unsigned int rWidth;
        unsigned int rHeight;
        unsigned int offX, offY;
        bool valid;

        DistMap(unsigned char *data, unsigned int size, unsigned int rWidth, unsigned int rHeight);
        virtual ~DistMap();

        void createChangeList();
        bool getColor(unsigned int x, unsigned int y);
        bool checkAround(unsigned int x, unsigned int y);

        float *generate(unsigned int sizeo);
        float findNearest(unsigned int x, unsigned int y);

        float *getFullImage();

        std::vector<Vec2<unsigned int>> whiteList;
        std::vector<Vec2<unsigned int>> blackList;



    protected:
    private:
};

#endif // DISTMAP_H
