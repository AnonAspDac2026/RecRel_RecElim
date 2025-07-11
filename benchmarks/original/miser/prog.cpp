#include "header.hpp"

#include <cmath>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <iostream>


class r_miser_generic {
public:
    template <unsigned Dim, class Func, class Rng>
    static float_pair_t eval(Func func,
                             float regn[Dim * 2],
                             unsigned long npts,
                             float dith,
                             Rng& rng,
                             RegionAllocator<float> region) {
        const float PFAC = 0.1f;
        const int MNPT = 15;
        const int MNBS = 60;
        const float TINY = 1e-30f;
        const float BIG  = 1e30f;
        float pt[Dim];

        if (npts < MNBS) {
            float sum = 0, sum2 = 0;
            for (unsigned long i = 0; i < npts; ++i) {
                rng.ranpt(pt, regn, Dim);
                float v = func(pt);
                sum += v;
                sum2 += v * v;
            }
            float ave = sum / npts;
            float var = std::max(TINY, (sum2 - sum * sum / npts) / (npts * npts));
            return {ave, var};
        } else {
            float *rmid = region.alloc(Dim);
            unsigned long npre = std::max((unsigned long)(npts * PFAC), (unsigned long)MNPT);
            float fminl[Dim], fmaxl[Dim], fminr[Dim], fmaxr[Dim];
            for (unsigned j = 0; j < Dim; ++j) {
                float s = rng() * dith;
                rmid[j] = (0.5f + s) * regn[j] + (0.5f - s) * regn[Dim + j];
                fminl[j] = fminr[j] = BIG;
                fmaxl[j] = fmaxr[j] = -BIG;
            }
            float pt[Dim];
            for (unsigned long i = 0; i < npre; ++i) {
                rng.ranpt(pt, regn, Dim);
                float v = func(pt);
                for (unsigned j = 0; j < Dim; ++j) {
                    if (pt[j] <= rmid[j]) {
                        fminl[j] = std::min(fminl[j], v);
                        fmaxl[j] = std::max(fmaxl[j], v);
                    } else {
                        fminr[j] = std::min(fminr[j], v);
                        fmaxr[j] = std::max(fmaxr[j], v);
                    }
                }
            }
            unsigned jb = 0;
            float sumb = BIG, siglb=1, sigrb=1;
            for (unsigned j = 0; j < Dim; ++j) {
                if (fmaxl[j] > fminl[j] && fmaxr[j] > fminr[j]) {
                    float sl = std::max(TINY, std::pow(fmaxl[j] - fminl[j], 1.5f));
                    float sr = std::max(TINY, std::pow(fmaxr[j] - fminr[j], 1.5f));
                    float sum_sr = sl + sr;
                    if (sum_sr < sumb) {
                        sumb = sum_sr;
                        jb = j;
                        siglb = sl;
                        sigrb = sr;
                    }
                }
            }
            if (sumb == BIG) jb = rng.NextUInt() % Dim;
            float rgl = regn[jb], rgm = rmid[jb], rgr = regn[Dim + jb];
            float fracl = std::fabs((rgm - rgl) / (rgr - rgl));
            unsigned long nptl = (unsigned long)(MNPT + (npts - npre - 2*MNPT) * fracl * siglb
                                / (fracl * siglb + (1-fracl) * sigrb));
            unsigned long nptr = npts - npre - nptl;

            float *subreg = region.alloc(2*Dim);
            std::copy(regn, regn + 2*Dim, subreg);

            // Left subregion
            subreg[Dim + jb] = rmid[jb];
            float_pair_t left = eval<Dim, Func, Rng>(func, subreg, nptl, dith, rng, region.NewRegion());
            // Right subregion
            subreg[jb] = rmid[jb];
            subreg[Dim + jb] = rgl + (rgr - rgl);
            float_pair_t right = eval<Dim, Func, Rng>(func, subreg, nptr, dith, rng, region.NewRegion());

            float ave = fracl * left.first + (1 - fracl) * right.first;
            float var = fracl * fracl * left.second + (1 - fracl) * (1 - fracl) * right.second;
            return {ave, var};
        }
    }
};

// Top-level HLS wrapper
float_pair_t process_top(float regn[2 * miser_test_config::N], unsigned long npts,
                         float dith, typename miser_test_config::rng_t& rng,
                         RegionAllocator<float> region) {
    #pragma HLS INTERFACE m_axi port=regn offset=slave bundle=gmem
    return r_miser_generic::eval<miser_test_config::N,
                                  decltype(miser_test_config::f),
                                  miser_test_config::rng_t>(
               miser_test_config::f, regn, npts, dith, rng, region);
}
