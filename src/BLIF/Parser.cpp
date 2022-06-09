#include "BLIF/Parser.hpp"


namespace LLS1::BLIF
{
    std::vector<Model> parseBLIF(const char* filename)
    {
        std::vector<Model> models;
        std::optional<std::string> token;
        Tokenizer tokenizer{ filename };

        while (token = tokenizer.pop())
        {
            if (*token == "#")
            {
                tokenizer.skipLine();
            }
            else if (*token == ".model")
            {
                std::optional<std::string> name = tokenizer.viewTop();
                tokenizer.skipLine();
                Model model = parseModel(tokenizer);

                if (name != "\n")
                    model.name = *name;
                else
                    model.name = filename;

                models.push_back(model);
            }
        }

        return models;
    }

    Model parseModel(Tokenizer& tokenizer)
    {
        Model model;
        std::optional<std::string> token;

        while (token = tokenizer.pop())
        {
            if (!token)
                throw std::runtime_error("parseModel: unexpected EOF");

            if (*token == ".end")
                break;

            if (*token == ".inputs")
                model.inputs = parseNames(tokenizer);
            else if (*token == ".outputs")
                model.outputs = parseNames(tokenizer);
            else if (*token == ".names")
                model.modules.push_back(
                    std::shared_ptr<Module>(
                        new BooleanFunction{ parseBooleanFunction(tokenizer) }
                    )
                );
        }

        return model;
    }


    std::vector<std::string> parseNames(Tokenizer& tokenizer)
    {
        std::vector<std::string> names;
        std::optional<std::string> token;

        while (token = tokenizer.pop())
        {
            if (!token)
                throw std::runtime_error("parseNames: unexpected EOF");

            if (*token == "#")
            {
                tokenizer.skipLine();
                break;
            }
            else if (*token == "\n")
            {
                break;
            }

            names.push_back(*token);
        }

        return names;
    }


    BooleanFunction parseBooleanFunction(Tokenizer& tokenizer)
    {
        BooleanFunction boolean_function;
        std::optional<std::string> token;
        std::vector<std::string> names = parseNames(tokenizer);
        boolean_function.inputNames = { names.begin(), names.end() - 1 };
        boolean_function.outputName = names.back();

        while (token = tokenizer.viewTop())
        {
            if (!token)
                throw std::runtime_error("parseBooleanFunction: unexpected EOF");

            if ((const char)(*token)[0] == '.')
                break;

            if (*token == "#")
                tokenizer.skipLine();
            else if (*token == "\n")
                tokenizer.pop();
            else
            {
                uint64_t input_values = std::stoi(
                    *(tokenizer.pop()), nullptr, 2
                );

                try
                {
                    bool output_value = static_cast<bool>(
                        std::stoi(*(tokenizer.pop()), nullptr, 2)
                    );

                    if (output_value)
                        boolean_function.minterms.push_back(input_values);
                }
                catch(const std::exception& e)
                {
                    boolean_function.minterms.push_back(input_values);
                }
            }
        }

        return boolean_function;
    }
} // namespace LLS1::BLIF
