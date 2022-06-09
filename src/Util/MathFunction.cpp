#include "Util/MathFunction.hpp"


namespace LLS1::Util
{
    uint64_t factorial(uint64_t num)
    {
        return num > 0ULL ? num * factorial(num - 1ULL) : 1ULL;
    }
} // namespace LLS1::Util
