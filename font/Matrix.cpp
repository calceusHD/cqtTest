/*
 * Matrix.cpp
 *
 *  Created on: Mar 17, 2013
 *      Author: ICH
 */

#include "Matrix.h"



template <class type>
Matrix<type>::Matrix(unsigned int fill):data(4 * 4) {
    set(fill);


}

template <class type>
void Matrix<type>::set(unsigned int x, unsigned int y, type val)
{
	this->data[x * 4 + y] = val;
}

template <class type>
void Matrix<type>::set(unsigned int fill)
{
    type num = 0;

	switch (fill)
	{
	case IDENTITY_MATRIX:
		num = 1;
		break;
	case EMPTY_MATRIX:
		num = 0;
		break;
	}
	for (unsigned int i = 0;i < 4; i++)
	{
		for (unsigned int j = 0;j < 4; j++)
		{
			if ( i == j)
			{
				this->set(i, j, num);
			}
			else
			{
				this->set(i, j, 0);
			}
		}
	}
}

template <class type>
Matrix<type>::~Matrix() {

}

template <class type>
Matrix<type> Matrix<type>::operator* (const Matrix& V1) const
{
    Matrix<type> out(EMPTY_MATRIX);
    for (unsigned int i = 0;i < 4; i++)
        for (unsigned int j = 0;j < 4; j++)
            out.set(j, i, this->data[j * 4 + 0] * V1.data[i + 0] +
                          this->data[j * 4 + 1] * V1.data[i + 4] +
                          this->data[j * 4 + 2] * V1.data[i + 8] +
                          this->data[j * 4 + 3] * V1.data[i + 12]);
    return out;
}

template <class type>
Matrix<type> Matrix<type>::operator*= (const Matrix& V1)
{
    return *this = *this * V1;
}

template <class type>
void Matrix<type>::translate(type x, type y, type z)
{
    Matrix<type> tmp(IDENTITY_MATRIX);
    tmp.data[12] = x;
    tmp.data[13] = y;
    tmp.data[14] = z;

    *this *= tmp;
}

template <class type>
void Matrix<type>::scale(type x, type y, type z)
{
    Matrix<type> tmp(IDENTITY_MATRIX);
    tmp.data[0] = x;
    tmp.data[5] = y;
    tmp.data[10] = z;

    *this *= tmp;
}

template <class type>
void Matrix<type>::ortho(type left, type right, type bottom, type top, type near, type far)
{
    Matrix<type> tmp(EMPTY_MATRIX);
    tmp.data[ 0] = 2 / ( right - left );
    tmp.data[ 5] = 2 / ( top - bottom );
    tmp.data[10] = - 2 / ( far - near );
    tmp.data[12] = - ( right + left ) / ( right - left );
    tmp.data[13] = - ( top + bottom ) / ( top - bottom );
    tmp.data[14] = - ( far + near ) / ( far - near );
    tmp.data[15] = 1;

    *this *= tmp;
}


template class Matrix<float>;
template class Matrix<double>;
