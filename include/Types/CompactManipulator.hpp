#pragma once

#include <optional>

#include "Types/CompactBooleanFunction.hpp"
#include "Types/NPNTranformation.hpp"


namespace LLS1::Types
{
    class CompactManipulator
    {
    public:
        CompactManipulator(unsigned long long);

        CompactBoolFunc negateInput(const CompactBoolFunc&, int);
        CompactBoolFunc swap(const CompactBoolFunc&, int, int);
        std::optional<CompactBoolFunc> applyTransformation(
            const CompactBoolFunc&, const NPNTransformation&
        );

        std::vector<CompactBoolFunc> index_masks;
    };
} // namespace LLS1::Types
