#pragma once

#include <string>
#include <vector>

#include "BLIF/BooleanFunction.hpp"

namespace LLS1::BLIF
{
	/*
		Class representing a single model in a BLIF file
	*/
	class Model
	{
	public:

		std::string name;
		std::vector<std::string> inputs;
		std::vector<std::string> outputs;
		std::vector<std::shared_ptr<Module>> modules;
	};
} // namespace LLS1::BLIF
