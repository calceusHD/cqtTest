/*
 * Matrix.h
 *
 *  Created on: Mar 17, 2013
 *      Author: ICH
 */

#ifndef MATRIX_H_
#define MATRIX_H_

#define IDENTITY_MATRIX 1
#define EMPTY_MATRIX 2

#include <vector>



template <class type>
class Matrix {
public:
	std::vector<type> data;

	Matrix(unsigned int fill);
	virtual ~Matrix();

    void translate(type x, type y, type z);
    void scale(type x, type y, type z);
    void ortho(type left, type right, type bottom, type top, type near, type far);

	void set(unsigned int x, unsigned int y, type val);
	void set(unsigned int fill);

	Matrix operator *(const Matrix&) const;
	Matrix operator *=(const Matrix&);
};

#endif /* MATRIX_H_ */
