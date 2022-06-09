#pragma once

#include <cinttypes>
#include <iostream>
#include <vector>


namespace LLS1::Types
{

    /*
        Transformation consisting of Negation of input,
        Permutation of input and/or Negation of output.
    */
    class NPNTransformation
    {
    public:
        NPNTransformation() = default;
        NPNTransformation(const NPNTransformation&);

        /*
            Create identity transformation for boolean
            functions with given num of inputs
        */
        NPNTransformation(uint64_t);

        /*
            Returns a new NPNTransformation which is
            an inverse of this
        */
        NPNTransformation operator~();
        
        /*
            Concatenate this tranformation with another transformation
        */
        NPNTransformation operator*(const NPNTransformation&);

        NPNTransformation swap(int, int);

        std::vector<bool> negation;
        std::vector<int> permutation;

        /*
            Format/Print
        */
        friend std::ostream& operator<<(
            std::ostream&, const NPNTransformation&
        );
    };


    std::ostream& operator<<(
        std::ostream& os, const NPNTransformation& function
    );
} // namespace LLS1::Types
