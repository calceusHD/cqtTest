

#ifndef CQT_H
#define CQT_H
#include "Kern.h"
#include "complex"
#include <cstring>
#include "fftw3.h"
#include "BlockMatrix.h"

class CQT {
public:
    CQT();
    CQT(const CQT& orig);
    
    static std::complex<double> getFromSymmetry(std::complex<double>* data, unsigned int length, unsigned int pos);
    
    static void transform(double *in, std::complex<double>* out, const BlockMatrix<std::complex<double>>& k, unsigned int n);
    
    virtual ~CQT();
private:

};

#endif /* CQT_H */

