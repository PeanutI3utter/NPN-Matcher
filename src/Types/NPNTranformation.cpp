#include "Types/NPNTranformation.hpp"


namespace LLS1::Types
{
    NPNTransformation::NPNTransformation(const NPNTransformation& other)
    {
        negation = other.negation;
        permutation = other.permutation;
    }


    NPNTransformation::NPNTransformation(uint64_t num_inputs)
    {
        for (int i = 0; i < num_inputs; ++i)
        {
            negation.push_back(false);
            permutation.push_back(i);
        }
        negation.push_back(false);
    }


    NPNTransformation NPNTransformation::operator~()
    {
        NPNTransformation inverse(permutation.size());

        for (int i = 0; i < permutation.size(); ++i)
        {
            int i_maps_to = permutation[i];
            // inverse mapping
            inverse.permutation[i_maps_to] = i;

            // cancel out negation
            if (negation[i])
                inverse.negation[i_maps_to] = true;
        }

        // cancel out output negation
        if (negation.back())
            *(inverse.negation.end() - 1) = true;

        return inverse;
    }


    NPNTransformation NPNTransformation::operator*(const NPNTransformation& other)
    {
        NPNTransformation cat(permutation.size());   
        for (int outer_input = 0; outer_input < permutation.size(); ++outer_input)
        {
            bool invert = negation[outer_input];

            // input index of mapped outer index
            int inner_input = permutation[outer_input];

            // cancel out negation
            invert ^= other.negation[inner_input];
            cat.negation[outer_input] = invert;

            // map outer_input to variable mapped from inner_input by inner_function
            cat.permutation[outer_input] = other.permutation[inner_input];
        }
        
        // cancel out negation
        *(cat.negation.end() - 1) = negation.back() ^ other.negation.back();
        
        return cat;
    }


    NPNTransformation NPNTransformation::swap(int i, int j)
    {
        NPNTransformation result = *this;
        
        int tmp = result.permutation[i];
        result.permutation[i] = result.permutation[j];
        result.permutation[j] = tmp;

        return result;
    }


    std::ostream& operator<<(
        std::ostream& os, const NPNTransformation& transformation
    ) {
        os << "Negation: ";
        for (int i = 0; i < transformation.negation.size() - 1; ++i)
        {
            if (transformation.negation[i])
                os << i << ", ";
        }
        if (transformation.negation.back())
                os << "output";
        os << std::endl;

        os << "Permutation: ";
        for (int i = 0; i < transformation.permutation.size(); ++i)
        {
            os << "(" << i << " -> " << transformation.permutation[i]
                << ") ";
        }
        os << std::endl;

        return os;
    }
} // namespace LLS1::Types
