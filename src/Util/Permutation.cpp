#include "Util/Permutation.hpp"


namespace LLS1::Util
{
    HeapPermutation::HeapPermutation(int num_elements)
    {
        c.resize(num_elements, 0);
        this->num_elements = num_elements;
        reset();
    }

    
    std::pair<int, int> HeapPermutation::getSwappingPair()
    {
        if (pointer == num_elements)
        {
            for(int& elem : c)
                elem = 0;
            
            pointer = 1;
            return {0, 0};
        }
        
        while (pointer < num_elements)
        {
            if (c[pointer] < pointer)
            {
                std::pair<int, int> swap;
                if (pointer % 2 == 0)
                    swap = {0, pointer};
                else
                    swap = {c[pointer], pointer};

                c[pointer] += 1;
                pointer = 1;

                return swap;
            } else {
                c[pointer] = 0;
                ++pointer;
            }
        }
        return {0, 0};
    }

    void HeapPermutation::reset()
    {
        for(int& elem : c)
            elem = 0;
        pointer = num_elements;
    }
} // namespace LLS1::Util
