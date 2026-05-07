#include "clogf.h"
#include "../include/logarithm/logarithm.hpp"

namespace dumb_math::logarithm {

extern "C" float logf(float x)
{
    return ln<float>(x);
}

} // namespace dumb_math::logarithm