/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <complex>


/* 
 * File:   BlockMatrix.cpp
 * Author: pc
 * 
 * Created on 16 August 2015, 11:46
 */

#include "BlockMatrix.h"

template<class T>
BlockMatrix<T>::BlockMatrix(T* data, unsigned int width, unsigned int height, double minVal) {
    mWidth = width;
    mHeight = height;
    mRows = new T*[height];
    mStarts = new unsigned int[height];
    mLengths = new unsigned int[height];
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            if (std::abs(data[y * width + x]) > minVal) {
                mStarts[y] = x;
                break;
            }
        }
        for (unsigned int x = width - 1; x > mStarts[y]; --x) {
            if (std::abs(data[y * width + x]) > minVal) {
                mLengths[y] = x - mStarts[y];
                break;
            }
        }
        printf("Len: %i", mLengths[y]);
        mRows[y] = new T[mLengths[y]];
        for (unsigned int x = 0; x < mLengths[y]; ++x) {
            mRows[y][x] = data[y * width + x + mStarts[y]];
        }
    }
}

template<class T>
BlockMatrix<T>::BlockMatrix(const BlockMatrix& orig) {
}

template<class T>
BlockMatrix<T>::~BlockMatrix() {
}

template<class T>
T BlockMatrix<T>::transformAndSum(std::function<T(int)> src, unsigned int row) const {
    T temp;
    for (int i = 0; i < mLengths[row]; ++i) {
        temp += src(mStarts[row] + i) * mRows[row][i];
    }
    return temp;
}

template<class T>
unsigned int BlockMatrix<T>::getWidth() const {
    return mWidth;
}

template<class T>
unsigned int BlockMatrix<T>::getHeight() const {
    return mHeight;
}

template class BlockMatrix<std::complex<double>>;
