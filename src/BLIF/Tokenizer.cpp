#include "BLIF/Tokenizer.hpp" 


namespace LLS1::BLIF
{
    Tokenizer::Tokenizer(const char* filename)
    {
        std::ifstream input;
        std::string line;
        input.open(filename);

        while (std::getline(input, line))
        {
            std::istringstream tokenizer{ line };
            std::string token;
            while (tokenizer >> token)
            {
                tokens.insert(tokens.begin(), token);
            }
            tokens.insert(tokens.begin(), "\n");
        }

        if (tokens.size() > 1)
        {
            current = tokens.back();
            tokens.pop_back();
        }
        else
        {
            current = {};
        }
    }

    std::optional<std::string> Tokenizer::viewTop()
    {
        return current;
    }

    std::optional<std::string> Tokenizer::pop()
    {
        std::optional<std::string> value = current;

        if (tokens.size() > 1)
        {
            current = tokens.back();
            tokens.pop_back();
        }
        else
        {
            current = {};
        }

        return value;
    }

    void Tokenizer::skipLine()
    {
        std::optional<std::string> value;

        while (value = pop())
        {
            if (value == "\n")
                return;
        }
    }
} // namespace LLS1::BLIF
