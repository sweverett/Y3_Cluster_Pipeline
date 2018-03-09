#include <cmath>

extern "C" double double_gaussian(double x) {
    // note we can still use C++ features--we just have to supply a
    // C-like function signature
    auto first = 1.0 / std::sqrt(2 * 3.141592653589793238)
        * std::exp(- 0.5 * std::pow(x - 1, 2));
    auto second = 1.0 / std::sqrt(2 * 3.141592653589793238)
        * std::exp(- 0.5 * std::pow(x + 1, 2));
    return 0.5 * first + 0.5 * second;
}

extern "C" double hmf(double x) {
  return 2*x;
}
