

#ifndef KERN_H
#define KERN_H

#include <vector>
#include <cmath>
#include <complex>
#include "fftw3.h"
#define M2PI 6.28318530718
#ifdef __STDC_NO_COMPLEX__
#error error
#endif
class Kern {
public:
    std::complex<double>* K;
    unsigned int mB;
    unsigned int mN0;
    Kern(int sampleRate, int bpo, float fMin, float fMax);
    Kern(const Kern& orig);
    static float hamming(unsigned int position, unsigned int length);
    virtual ~Kern();
   
private:

};

#endif /* KERN_H */

