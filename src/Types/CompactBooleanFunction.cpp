#include "Types/CompactBooleanFunction.hpp"


namespace LLS1::Types
{
    CompactBoolFunc::CompactBoolFunc(uint64_t num_inputs)
    {
        this->num_inputs = num_inputs;

        truth_table = TruthTable{1UL << num_inputs};
    }

    
    CompactBoolFunc::CompactBoolFunc(
        uint64_t num_inputs, TruthTable truth_table
    ) {
        this->num_inputs = num_inputs;
        this->truth_table = truth_table;
    }
    
    
    CompactBoolFunc::CompactBoolFunc(const LLS1::BLIF::BooleanFunction& bf)
    {
        num_inputs = bf.inputNames.size();
        truth_table.resize(1 << num_inputs);
        
        for (unsigned long minterm : bf.minterms)
        {
            truth_table.set(minterm);
        }
    }
    
    
    CompactBoolFunc::CompactBoolFunc(const CompactBoolFunc& other)
    {
        num_inputs = other.num_inputs;
        truth_table = other.truth_table;
    }


    CompactBoolFunc CompactBoolFunc::operator~()
    {
        return {num_inputs, ~truth_table};
    }
    
    
    CompactBoolFunc CompactBoolFunc::operator<<(size_t shift_cnt)
    {
        return {num_inputs, truth_table << shift_cnt};
    }


    void CompactBoolFunc::operator<<=(size_t shift_cnt)
    {
        truth_table <<= shift_cnt;
    }


    CompactBoolFunc CompactBoolFunc::operator>>(
        size_t shift_cnt
    ) {
        return {num_inputs, truth_table >> shift_cnt};
    }


    void CompactBoolFunc::operator>>=(size_t shift_cnt)
    {
        truth_table >>= shift_cnt;
    }


    bool CompactBoolFunc::operator<(
        const CompactBoolFunc& other
    ) {
        return truth_table < other.truth_table;
    }


    bool CompactBoolFunc::operator>(
        const CompactBoolFunc& other
    ) {
        return truth_table > other.truth_table;
    }


    CompactBoolFunc CompactBoolFunc::operator&(
        const CompactBoolFunc& other
    ) {
        return {num_inputs, truth_table & other.truth_table};
    }


    void CompactBoolFunc::operator&=(
        const CompactBoolFunc& other
    ) {
        if (other.num_inputs != num_inputs)
            return;
            
        truth_table &= other.truth_table;
    }


    CompactBoolFunc CompactBoolFunc::operator|(
        const CompactBoolFunc& other
    ) {
        return {num_inputs, truth_table | other.truth_table};
    }


    void CompactBoolFunc::operator|=(
        const CompactBoolFunc& other
    ) {
        if (other.num_inputs != num_inputs)
            return;
            
        truth_table |= other.truth_table;
    }


    CompactBoolFunc CompactBoolFunc::operator^(
        const CompactBoolFunc& other
    ) {
        return {num_inputs, truth_table ^ other.truth_table};
    }


    void CompactBoolFunc::operator^=(
        const CompactBoolFunc& other
    ) {
        if (other.num_inputs != num_inputs)
            return;
            
        truth_table ^= other.truth_table;
    }


    bool CompactBoolFunc::operator==(
        const CompactBoolFunc& other
    ) const {
        return truth_table == other.truth_table;
    }


    unsigned int CompactBoolFunc::getMaxSat() const
    {
        int pos = truth_table.find_first();
        while(truth_table.find_next(pos) != truth_table.npos)
            pos = truth_table.find_next(pos);
        return pos;
    }


    std::ostream& operator<<(
            std::ostream& os, const CompactBoolFunc& function
    ) {
        unsigned long long max_index = (1LL << function.num_inputs) - 1;
        for (
            long long i = 0;
            i < max_index + 1;
            ++i
        ) {
            if (function.truth_table[i])
            {
                for (
                    int j = (1L << function.num_inputs - 1);
                    j > 0;
                    j >>= 1
                )
                    os << (bool)(j & i);
                os << std::endl;
            }
        }
        return os;
    }
} // namespace LLS1::Types
