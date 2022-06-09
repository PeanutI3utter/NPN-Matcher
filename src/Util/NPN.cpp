#include "Util/NPN.hpp"


namespace LLS1::Util
{
    std::optional<std::pair<NPNTransformation, NPNTransformation>> FlipSwap::equal(
        const CompactBoolFunc& a, const CompactBoolFunc& b
    ) {
        if (a.num_inputs != b.num_inputs)
            return {};
        
        std::chrono::steady_clock::time_point begin =
            std::chrono::steady_clock::now();
        
        transformations = 0;

        NPNTransformation trans_repr_a = getRepresentant(a);
        NPNTransformation trans_repr_b = getRepresentant(b);

        CompactBoolFunc repr_a =
            *func_manipulator.applyTransformation(a, trans_repr_a);
        CompactBoolFunc repr_b =
            *func_manipulator.applyTransformation(b, trans_repr_b);

        elapsed_microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - begin
            ).count();

        if (repr_a == repr_b)
            return {{trans_repr_a, trans_repr_b}};

        return {};
    }


    /*
        Implemenation of the FlipSwap algorithm for searching
        the representant of the NPN class of given function.
    */
    NPNTransformation FlipSwap::getRepresentant(
        const CompactBoolFunc& function
    ) {
        
        CompactBoolFunc current = function;
        NPNTransformation transformation{function.num_inputs};

        bool improvement;
        int iter = 0;

        do
        {
            improvement = false;
            CompactBoolFunc candidate;

            candidate = ~current;
            ++transformations;
            if (candidate < current)
            {
                    improvement = true;
                    current = candidate;
                    (*(transformation.negation.end() - 1)).flip();
            }

            for (int i = 0; i < function.num_inputs; ++i)
            {
                candidate = func_manipulator.negateInput(current, i);
                ++transformations;
                if (candidate < current)
                {
                    improvement = true;
                    current = candidate;
                    transformation.negation[i].flip();
                }
            }

            for (
                int distance = 1;
                distance < function.num_inputs;
                ++distance
            ) {
                for (
                    int input = 0;
                    input < function.num_inputs - distance;
                    ++input
                ) {
                    int swap_input = input + distance;
                    candidate = func_manipulator.swap(current, input, swap_input);
                    ++transformations;
                    if (candidate < current)
                    {
                        improvement = true;
                        current = candidate;
                        int tmp = transformation.permutation[input];
                        transformation.permutation[input] =
                            transformation.permutation[swap_input];
                        transformation.permutation[swap_input] = tmp;
                    }
                }
            }
        } while(improvement);

        return transformation;
    }


    std::optional<std::pair<NPNTransformation, NPNTransformation>> Sifting::equal(
        const CompactBoolFunc& a, const CompactBoolFunc& b
    ) {
        if (a.num_inputs != b.num_inputs)
            return {};
        
        std::chrono::steady_clock::time_point begin =
            std::chrono::steady_clock::now();
        
        transformations = 0;

        NPNTransformation trans_repr_a = getRepresentant(a);
        NPNTransformation trans_repr_b = getRepresentant(b);

        CompactBoolFunc repr_a =
            *func_manipulator.applyTransformation(a, trans_repr_a);
        CompactBoolFunc repr_b =
            *func_manipulator.applyTransformation(b, trans_repr_b);

        elapsed_microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - begin
            ).count();

        if (repr_a == repr_b)
            return {{trans_repr_a, trans_repr_b}};

        return {};
    }


    /*
        Implemenation of the FlipSwap algorithm for searching
        the representant of the NPN class of given function.
    */
    NPNTransformation Sifting::getRepresentant(
        const CompactBoolFunc& function
    ) {
        
        CompactBoolFunc current = function;
        NPNTransformation transformation{function.num_inputs};

        bool improvement;
        int iter = 0;

        CompactBoolFunc best_candidate = function;
        NPNTransformation best_transformation;
        do
        {
            improvement = false;

            for (int i = 0; i < function.num_inputs - 1; ++i)
            {
                CompactBoolFunc best_candidate_local = best_candidate;
                NPNTransformation best_transformation_local = best_transformation;
                for (int j = 0; j < 3; ++j)
                {
                    if (j % 2 == 0)
                    {
                        CompactBoolFunc candidate =
                            func_manipulator.negateInput(best_candidate, i);
                        ++transformations;
                        if (candidate < best_candidate_local)
                        {
                            improvement = true;
                            best_candidate_local = candidate;
                            best_transformation_local.negation[i] = 
                                ~best_transformation_local.negation[i];
                        }
                    } else {
                        CompactBoolFunc candidate =
                            func_manipulator.negateInput(best_candidate, i + 1);
                        ++transformations;
                        if (candidate < best_candidate_local)
                        {
                            improvement = true;
                            best_candidate_local = candidate;
                            best_transformation_local.negation[i + 1] = 
                                ~best_transformation_local.negation[i + 1];
                        }
                    }
                }
                
                CompactBoolFunc candidate =
                    func_manipulator.swap(best_candidate, i, i + 1);
                NPNTransformation trans_swapped = best_transformation.swap(i, i + 1);
                trans_swapped.negation[i + 1] = true;
                if (candidate < best_candidate_local)
                {
                    improvement = true;
                    best_candidate_local = candidate;
                    best_transformation_local = trans_swapped;
                }
            }
        } while(improvement);

        return transformation;
    }


    std::optional<NPNTransformation> Exhaustive::equal(
        const CompactBoolFunc& a, const CompactBoolFunc& b
    ) {
        if (a.num_inputs != b.num_inputs)
            return {};

        if (a.operator==(b))
            return {NPNTransformation{a.num_inputs}};
        
        std::chrono::steady_clock::time_point begin =
            std::chrono::steady_clock::now();
        transformations = 0;

        for (int output_negation = 0; output_negation < 2; ++output_negation)
        {
            for (
                uint64_t negation = 0;
                negation < (1ULL << a.num_inputs) - 1;
                ++negation
            ) {
                    
                CompactBoolFunc candidate = a;
                NPNTransformation tranformation{a.num_inputs};
                HeapPermutation permutator{a.num_inputs};
                for (int index = 0; index < a.num_inputs; ++index)
                    if (negation & (1ULL << index))
                    {
                        candidate =
                            func_manipulator.negateInput(candidate, index);
                        ++transformations;
                        tranformation.negation[index] = true;
                    }

                if (output_negation)
                {
                    candidate = ~candidate;
                    ++transformations;
                    tranformation.negation.back() = true;
                }

                for (
                    uint64_t permutation = 0;
                    permutation < factorial(a.num_inputs);
                    ++permutation
                ) {
                    std::pair<int, int> swap_inputs = permutator.getSwappingPair();
                    candidate = func_manipulator.swap(
                        candidate, swap_inputs.first, swap_inputs.second);
                    ++transformations;
                    tranformation = tranformation.swap(
                        swap_inputs.first, swap_inputs.second);
                    
                    if (candidate == b)
                    {
                        elapsed_microseconds =
                            std::chrono::duration_cast<std::chrono::microseconds>(
                                std::chrono::steady_clock::now() - begin
                            ).count();

                        return {tranformation};
                    }
                }
            }
        }
        
        elapsed_microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now() - begin
            ).count();

        return {};
    }
} //  namespace LLS1::Util