#include "header.hpp"


using complex_t = std::complex<float>;

void r_fft_aux(int n,
               const complex_t& wn,
               const complex_t* pIn,
               int sIn,
               complex_t* pOut,
               int sOut) {
    if (n == 1) {
        pOut[0] = pIn[0];
    } else if (n == 2) {
        // Base case: 2-point DFT
        complex_t a = pIn[0] + pIn[sIn];
        complex_t b = pIn[0] - pIn[sIn];
        pOut[0]       = a;
        pOut[sOut]    = b;
    } else {
        int half = n / 2;
        // Recursive calls
        r_fft_aux(half, wn * wn, pIn,      sIn * 2, pOut,             sOut);
        r_fft_aux(half, wn * wn, pIn + sIn, sIn * 2, pOut + half * sOut, sOut);

        // Combine step
        complex_t w(1.0f, 0.0f);
        for (int j = 0; j < half; ++j) {
            complex_t even = pOut[j * sOut];
            complex_t odd  = pOut[(j + half) * sOut];
            complex_t t    = w * odd;
            pOut[j * sOut]            = even + t;
            pOut[(j + half) * sOut]  = even - t;
            w *= wn;
        }
    }
}

void fft(int log2n, const complex_t* pIn, complex_t* pOut) {
    int n = 1 << log2n;
    const double PI = 3.14159265358979323846;
    // Primitive n-th root of unity: exp(-2*pi*i/n)
    complex_t wn = std::polar(1.0f, static_cast<float>(-2 * PI / n));
    r_fft_aux(n, wn, pIn, 1, pOut, 1);
}

// Top-level function for HLS
int process_top(int log2n, const complex_t* pIn, complex_t* pOut) {
    #pragma HLS INTERFACE port=pIn  ap_memory depth=8
    #pragma HLS INTERFACE port=pOut ap_memory depth=8
    fft(log2n, pIn, pOut);
    return 0;
}