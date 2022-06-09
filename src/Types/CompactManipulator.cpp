#include "Types/CompactManipulator.hpp"


namespace LLS1::Types
{
    CompactManipulator::CompactManipulator(unsigned long long num_inputs)
    {
        for (unsigned long long input = 0; input < num_inputs; ++input)
        {
            boost::dynamic_bitset<> mask{1UL << num_inputs};

            int num_constant_val = 1 << input;
            for (
                int consecutive_ones = num_constant_val;
                consecutive_ones < (1UL << num_inputs);
                consecutive_ones += (num_constant_val << 1)
            ) {
                for (
                    int  offset = 0;
                    offset < num_constant_val;
                    ++offset
                ) {
                    mask.set(consecutive_ones + offset);
                }
            }

            index_masks.push_back({num_inputs, mask});
        }
    }


    CompactBoolFunc CompactManipulator::negateInput(
        const CompactBoolFunc& function,
        int index
    ) {
        int shift = 1 << index;
        CompactBoolFunc result = 
            ((index_masks[index] & function) >> shift)
            | (((index_masks[index] >> shift) & function) << shift);
        return result;
    }


    CompactBoolFunc CompactManipulator::swap(
        const CompactBoolFunc& function, int i, int j
    ) {
        if (i == j)
            return function;

        if (i < j)
        {
            int tmp = i;
            i = j;
            j = tmp;
        }

        int shift = (1 << i) - (1 << j);
        CompactBoolFunc i_one_j_zero = 
            ~index_masks[i] & index_masks[j];
        CompactBoolFunc i_zero_j_one = 
            index_masks[i] & ~index_masks[j];

        
        CompactBoolFunc result = 
            ~(i_one_j_zero | i_zero_j_one) & function;
        result = result | (i_one_j_zero & function) << shift;
        result = result | (i_zero_j_one & function) >> shift;

        return result;
    }


    std::optional<CompactBoolFunc> CompactManipulator::applyTransformation(
        const CompactBoolFunc& function, const NPNTransformation& transformation
    ) {
        if (function.num_inputs != transformation.permutation.size())
            return {};

        CompactBoolFunc result = function;
        for (int i = 0; i < function.num_inputs; ++i)
        {
            if (transformation.negation[i])
                result = negateInput(result, i);
        }

        if (transformation.negation[result.num_inputs])
            result = ~result;

        for (int i = 0; i < function.num_inputs; ++i)
            result = swap(result, i, transformation.permutation[i]);

        return result;
    }
} // namespace LLS1::Types
