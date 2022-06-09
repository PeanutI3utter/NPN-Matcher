#pragma once

#include <cinttypes>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "BLIF/Module.hpp"


namespace LLS1::BLIF
{
	/*
		BooleanFunction Module inside a BLIF model
	*/
	class BooleanFunction : public Module
	{
	public:
		BooleanFunction() {};
		~BooleanFunction() = default;
		BooleanFunction(const BooleanFunction& other);

		/*
			Returns a new BooleanFunction where the input with
			given name is set to given value
		*/
		BooleanFunction apply(std::string name, bool value);

		/*
			Evaluate a constant function. A function is constant
			if there is no input variables. If function is constant
			returns the constant boolean value of the function.
			Otherwise returns an empyt Optional.
		*/
		std::optional<bool> constantValue();

		std::vector<std::string> inputNames;
		std::string outputName;

		// truth table containing all minterms
		std::vector<uint64_t> minterms;
	};
} // namespace LLS1::BLIF
