#pragma once

#include "BLIF/BooleanFunction.hpp"
#include "BLIF/Model.hpp"
#include "BLIF/Tokenizer.hpp"


namespace LLS1::BLIF
{
    /*
        Methods for parsing BLIF Files
    */

    /*
        Parse list of BLIF models from BLIF file
    */
    std::vector<Model> parseBLIF(const char* filename);

    /*
        Parse BLIF model from current token
    */
    Model parseModel(Tokenizer& tokenizer);
    
    /*
        Parse variable names from current line
    */
    std::vector<std::string> parseNames(Tokenizer&);

    /*
        Parse boolean function description from current token onwards
    */
    BooleanFunction parseBooleanFunction(Tokenizer&);
} // namespace LLS1::BLIF
