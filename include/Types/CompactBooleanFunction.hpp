#pragma once

#include <boost/dynamic_bitset.hpp>
#include <cinttypes>
#include <iostream>

#include "BLIF/BooleanFunction.hpp"


namespace LLS1::Types
{
    /*
        A more compact representation of a Boolean Function
        in truth table representation. Instead of saving
        the minterms, the truth table is saved in a single bitset.
        Each bit represent a minterm of a function with n inputs.
        If the bit is set to 1 the minterm satisfies the function,
        otherwise the minterm does not satisfy the function.
        The class boost::multiprecision::cpp_int was chosen
        as the internal bitset data structure to allow
        arbitary number of inputs. Variable names are not
        kept in this class.

        Ex.:
            Function with 2 inputs: a & b
            Representation: 1000
            Interpretation: Beginning from the LSB, the minterms are
                increased by 1, as if they were a number. Here
                the LSB would mean not a, not b. Bit 1 would mean
                not a, b. Bit 2 would mean a, not b. Finally 
                bit 3 is to be interpreted as a, b. In other words
                only the output bit of the truth table is contained
                in this representation.
    */
    class CompactBoolFunc
    {
    public:
        using TruthTable = boost::dynamic_bitset<>;

        CompactBoolFunc() : num_inputs{0}, truth_table{0} {};
        CompactBoolFunc(uint64_t num_inputs);
        CompactBoolFunc(uint64_t num_inputs, TruthTable truth_table);
        CompactBoolFunc(const LLS1::BLIF::BooleanFunction&);
        CompactBoolFunc(const CompactBoolFunc&);
        ~CompactBoolFunc() = default;

        /*
            Return a new BooleanFunction equal to this
            where the output is negated.
        */
        CompactBoolFunc operator~();

        /*
            Return a new BooleanFunction equal to this where the truth
            table bitset is shifted n bits to the left
            (Used for NPN equivalence check)
        */
        CompactBoolFunc operator<<(size_t);

        /*
            Shift truth table to left by given amount
        */
        void operator<<=(size_t);

        /*
            Return a new BooleanFunction equal to this where the truth
            table bitset is shifted n bits to the right
            (Used for NPN equivalence check)
        */
        CompactBoolFunc operator>>(size_t);

        /*
            Shift truth table to righ by given amount
        */
        void operator>>=(size_t);

        /*
            Returns true if the lexicograhically biggest assignment
            of this is smaller than the lexicograhically biggest
            assignment of the given function.
            Returns 0 otherwise.
        */
        bool operator<(const CompactBoolFunc&);

        /*
            Returns true if the lexicograhically biggest assignment
            of this is bigger than the lexicograhically biggest
            assignment of the given function.
            Returns 0 otherwise.
        */
        bool operator>(const CompactBoolFunc&);

        /*
            Return a new BooleanFunction where the truth table of this
            is conjuncted with the truth table of the given function
        */
        CompactBoolFunc operator&(const CompactBoolFunc&);


        /*
            Assign the conjuction of this and given function to this
        */
        void operator&=(const CompactBoolFunc&);


        /*
            Return a new BooleanFunction where the truth table of this
            is disjuncted with the truth table of the given function
        */
        CompactBoolFunc operator|(const CompactBoolFunc&);


        /*
            Assign the disjunction of this and given function to this
        */
        void operator|=(const CompactBoolFunc&);

        
        /*
            Return a new BooleanFunction where the truth table of this
            is XOR'ed with the truth table of the given function
        */
        CompactBoolFunc operator^(const CompactBoolFunc&);


        /*
            Assign the XOR of this and given function to this
        */
        void operator^=(const CompactBoolFunc&);
        
        /*
            Returns true if the given function is equal to this  
        */
        bool operator==(const CompactBoolFunc&) const;

        /*
            Get the lexicographically biggest assignment of this
            function.
        */
        unsigned int getMaxSat() const;

        uint64_t num_inputs;
        TruthTable truth_table;

        /*
            FormatPrint function to an ostream.
        */
        friend std::ostream& operator<<(
            std::ostream& os, const CompactBoolFunc& function
        );
    };


    std::ostream& operator<<(
        std::ostream& os, const CompactBoolFunc& function
    );
} // namespace LLS1::Types
