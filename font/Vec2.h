#ifndef VEC2_H
#define VEC2_H

#include <cmath>

template <class type>
class Vec2
{
    public:
        Vec2();
        Vec2(type k);
        Vec2(type x, type y);
        virtual ~Vec2();

        float length();

        type lengthSq();

        bool operator== (const Vec2&) const;

        Vec2 operator+ (const Vec2&) const;

        Vec2 operator- (const Vec2&) const;

        Vec2 operator- ( ) const;

        Vec2 operator/ (float) const;

        Vec2 operator/ (const Vec2&) const;

        Vec2 operator* (const Vec2&) const;

        Vec2 operator* (float) const;

        void operator+= ( const Vec2&);

        void operator-= ( const Vec2&);

        type operator[] ( int);

        void Normalize();
    //protected:
        type x,y;
    private:
};

#endif // VEC2_H
