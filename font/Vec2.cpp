#include "Vec2.h"

template <class type>
Vec2<type>::Vec2()
{
    this->y = 0.0;
    this->x = 0.0;
}

template <class type>
Vec2<type>::Vec2(type k)
{
    this->x = k;
    this->y = k;
}

template <class type>
Vec2<type>::Vec2(type x, type y)
{
    this->x = x;
    this->y = y;
}

template <class type>
Vec2<type>::~Vec2()
{
    //dtor
}

template <class type>
float Vec2<type>::length()
{
    return sqrt(x*x + y*y);
}

template <class type>
type Vec2<type>::lengthSq()
{
    return x*x + y*y;
}


template <class type>
bool Vec2<type>::operator== (const Vec2& V2) const
{
    return (x == V2.x && y == V2.y);
}

template <class type>
Vec2<type> Vec2<type>::operator+ (const Vec2& V2) const
{
    return Vec2( x + V2.x,  y + V2.y);
}

template <class type>
Vec2<type> Vec2<type>::operator- (const Vec2& V2) const
{
    return Vec2( x - V2.x,  y - V2.y);
}

template <class type>
Vec2<type> Vec2<type>::operator- ( ) const
{
    return Vec2(-x, -y);
}

template <class type>
Vec2<type> Vec2<type>::operator/ (float S ) const
{
    float fInv = 1.0f / S;
    return Vec2 (x * fInv , y * fInv);

}

template <class type>
Vec2<type> Vec2<type>::operator/ (const Vec2& V2) const
{
    return Vec2 (x / V2.x,  y / V2.y);
}

template <class type>
Vec2<type> Vec2<type>::operator* (const Vec2& V2) const
{
    return Vec2 (x * V2.x,  y * V2.y);
}

template <class type>
Vec2<type> Vec2<type>::operator* (float S) const
{
    return Vec2 (x * S,  y * S);
}

template <class type>
void Vec2<type>::operator+= ( const Vec2& V2 )
{
    x += V2.x;
    y += V2.y;
}

template <class type>
void Vec2<type>::operator-= ( const Vec2& V2 )
{
    x -= V2.x;
    y -= V2.y;
}

template <class type>
type Vec2<type>::operator[] ( int i )
{
    if ( i == 0 ) return x;
    else return y;
}

template <class type>
void Vec2<type>::Normalize()
{
    double fMag = ( x*x + y*y );
    if (fMag == 0) {return;}

    double fMult = 1.0/sqrt(fMag);
    x *= fMult;
    y *= fMult;
    return;
}

template class Vec2<unsigned int>;
template class Vec2<float>;
template class Vec2<double>;
