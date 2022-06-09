#pragma once

#include <chrono>
#include <optional>
#include <utility>

#include "Types/CompactBooleanFunction.hpp"
#include "Types/CompactManipulator.hpp"
#include "Types/NPNTranformation.hpp"
#include "Util/MathFunction.hpp"
#include "Util/Permutation.hpp"


namespace LLS1::Util
{
    using LLS1::Types::CompactBoolFunc;
    using LLS1::Types::CompactManipulator;
    using LLS1::Types::NPNTransformation;
    

    class NPNChecker
    {
    public:
        int64_t elapsed_microseconds;
        int64_t transformations;
    };

    /*
        FlipSwap heuristic for NPN equivalence check.
    */
    class FlipSwap : public NPNChecker
    {
    public:
        FlipSwap(unsigned long long num_inputs) : func_manipulator{num_inputs} {};


        /*
            Returns the NPN tranformations needed to tranform
            the given funcitons 
        */
        std::optional<std::pair<NPNTransformation, NPNTransformation>> equal(
            const CompactBoolFunc&, const CompactBoolFunc&
        );


        /*
            Return the representant function of the given function
        */
        NPNTransformation getRepresentant(
            const CompactBoolFunc&
        );


        CompactManipulator func_manipulator;
    };


    /*
        FlipSwap heuristic for NPN equivalence check.
    */
    class Sifting : public NPNChecker
    {
    public:
        Sifting(unsigned long long num_inputs) : func_manipulator{num_inputs} {};


        /*
            Returns the NPN tranformations needed to tranform
            the given funcitons 
        */
        std::optional<std::pair<NPNTransformation, NPNTransformation>> equal(
            const CompactBoolFunc&, const CompactBoolFunc&
        );


        /*
            Return the representant function of the given function
        */
        NPNTransformation getRepresentant(
            const CompactBoolFunc&
        );


        CompactManipulator func_manipulator;
    };


    class Exhaustive : public NPNChecker
    {
    public:
        Exhaustive(unsigned long long num_inputs) : func_manipulator{num_inputs} {};
        /*
            Implemenation of the singleton method
        */
        std::optional<NPNTransformation> equal(
            const CompactBoolFunc&, const CompactBoolFunc&
        );
        
        CompactManipulator func_manipulator;
    };
} //  namespace LLS1::Util