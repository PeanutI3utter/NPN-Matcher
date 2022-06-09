#pragma once

#include <vector>


namespace LLS1::Util
{
    class HeapPermutation
    {
    public:
        HeapPermutation(int num_elements);

        std::pair<int, int> getSwappingPair();
        void reset();

    private:
        int pointer;
        int num_elements;
        std::vector<int> c;
    };
} // namespace LLS1::Util
