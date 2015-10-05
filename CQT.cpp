
#include <GLFW/glfw3.h>

#include "CQT.h"
#include "BlockMatrix.h"

CQT::CQT() {
}

CQT::CQT(const CQT& orig) {
}

CQT::~CQT() {
}

std::complex<double> CQT::getFromSymmetry(std::complex<double>* data, unsigned int length, unsigned int pos) {
    if (pos < length) {
        if (pos < 0 || pos >= length) {
            printf("error");
        }
        return data[pos];
    } else {
        if (2 * length - pos < 0 || 2 * length - pos > length) {
            printf("error");
        }
        return std::conj(data[2 * length - pos]);
    }
}

void CQT::transform(double* in, std::complex<double>* out, const BlockMatrix<std::complex<double>>& m, unsigned int n) {
    double* tmp;
    if (n == m.getWidth()) {
       tmp = in; 
    } else {
        tmp = (double*)fftw_malloc(sizeof(double) * m.getWidth());
        memcpy(tmp, in, n * sizeof(double));
        memset(tmp + n, 0, sizeof(double) * (m.getWidth() - n));
    }
    std::complex<double>* x = (std::complex<double>*)fftw_malloc(sizeof(std::complex<double>) * ( m.getWidth() / 2 + 1 ) );
    fftw_plan p;
    p = fftw_plan_dft_r2c_1d(m.getWidth(), tmp, reinterpret_cast<fftw_complex*>(x), FFTW_ESTIMATE);
    
    

    
    fftw_execute(p);
    for (unsigned int i = 0; i < m.getHeight(); ++i) {
        out[i] = m.transformAndSum(([&x, &m](unsigned int j)-> std::complex<double>{return getFromSymmetry(x, m.getWidth() / 2 + 1, j);}), i) / (double)m.getWidth();
    }
    fftw_destroy_plan(p);
    if (n != m.getWidth()) {
        fftw_free(tmp);
    }
    fftw_free(x);
}
