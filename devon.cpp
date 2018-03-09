#include <cmath>

extern "C" double gaussian(double x) {
    return 1.0 / std::sqrt(2 * 3.141592653589793238)
        * std::exp(- 0.5 * x * x);
}
