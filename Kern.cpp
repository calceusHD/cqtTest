

#include "Kern.h"

Kern::Kern(int sampleRate, int bpo, float fMin, float fMax) {
    using namespace std::complex_literals;
    float r = std::pow(2.0, 1.0/bpo);
    float Q = 1 / ( r - 1 );
    unsigned int B = std::ceil( std::log( fMax / fMin ) / std::log( r ) );
    float oMin = M2PI * fMin / sampleRate;
    float o[B];
    for (unsigned int i = 0; i < B; ++i)
        o[i] = oMin * std::pow(r, i);
    unsigned int N[B];
    for (unsigned int i = 0; i < B; ++i)
        N[i] = std::round(M2PI * Q / o[i]);
    unsigned int N0 = N[0];
    std::complex<double>* k = (std::complex<double>*)fftw_malloc(sizeof(std::complex<double>) * N0);
    K = (std::complex<double>*)fftw_malloc(sizeof(std::complex<double>) * N0 * B);
    fftw_plan p;
    p = fftw_plan_dft_1d(N0, reinterpret_cast<fftw_complex*>(k), reinterpret_cast<fftw_complex*>(K), FFTW_FORWARD, FFTW_MEASURE);
    for (unsigned int i = 0; i < B; ++i) {
        for (unsigned int j = 0; j < N0; ++j) {
            float w = hamming(j, N0);
            std::complex<double> tmp = ((double)w * exp(1i * (double)(-o[i] * j)) / (double)N0);
            k[N0 - j - 1] = tmp;
            
        }
        fftw_execute_dft(p, reinterpret_cast<fftw_complex*>(k), reinterpret_cast<fftw_complex*>(K + i * N0));
        //memcpy(K + i * N0, k, sizeof(fftw_complex) * N0);
    }
    mB = B;
    mN0 = N0;
    
    
}

Kern::Kern(const Kern& orig) {
}


float Kern::hamming(unsigned int position, unsigned int length) {
    if (position < length) {
        return 0.53836f - 0.46164f * std::cos(M2PI * (float)position / length);
    } else {
        return 0.0f;
    }
    
}

Kern::~Kern() {
}

