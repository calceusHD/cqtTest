/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BlockMatrix.h
 * Author: pc
 *
 * Created on 16 August 2015, 11:46
 */

#ifndef BLOCKMATRIX_H
#define BLOCKMATRIX_H

#include <functional>

template<class T>
class BlockMatrix {
protected:
    unsigned int mWidth;
    unsigned int mHeight;
    unsigned int* mStarts;
    unsigned int* mLengths;
    T** mRows;    
public:
    
    
    BlockMatrix(T* data, unsigned int width, unsigned int height, double minVal);
    
    BlockMatrix(const BlockMatrix& orig);
    
    T transformAndSum(std::function<T(int)> src, unsigned int row) const;
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    
    virtual ~BlockMatrix();
private:

};

#endif /* BLOCKMATRIX_H */

