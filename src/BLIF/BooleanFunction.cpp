#include "BLIF/BooleanFunction.hpp"


namespace LLS1::BLIF
{
    BooleanFunction::BooleanFunction(const BooleanFunction& other)
    {
        inputNames = other.inputNames;
        outputName = other.outputName;
        minterms = other.minterms;
    }


    BooleanFunction BooleanFunction::apply(std::string name, bool value)
    {
        if (inputNames.size() < 1)
            return *this;

        BooleanFunction result;
        result.outputName = outputName;
        
        int index = -1;
        auto iterator = inputNames.begin();
        while (iterator != inputNames.end())
        {
            if (*iterator == name)
                index = iterator - inputNames.begin();
            else
                result.inputNames.push_back(*iterator);
            ++iterator;
        }

        if (index < 0)
            throw std::runtime_error(
                "Exception in BooleanFunction::apply : Variable to be applied not defined"
            );

        int shiftToLeft = inputNames.size() - index - 1;
        for (uint64_t minterm : minterms)
        {
            if (((minterm & (1LL << shiftToLeft)) >> shiftToLeft) == value)
            {
                uint64_t upper = (minterm >> (shiftToLeft + 1)) << shiftToLeft;
                uint64_t lower = minterm & ((1 << shiftToLeft) - 1);
                result.minterms.push_back(upper | lower);
            }
        }

        return result;
    }


    std::optional<bool> BooleanFunction::constantValue()
    {
        if (inputNames.size() > 0)
            return {};

        return minterms.size() > 0;
    }
} // namespace LLS1::BLIF
