#pragma once

#include <fstream>
#include <optional>
#include <sstream>
#include <vector>

#include "BLIF/Tokenizer.hpp"


namespace LLS1::BLIF
{
	/*
		Whitespace Tokenizer
	*/
	class Tokenizer
	{
	public:
		/*
			Instantiate whitespace tokenizer with given file
		*/
		Tokenizer(const char* filename);

		/*
			View token currently on the stream front
		*/
		std::optional<std::string> viewTop();

		/*
			Return token currently on the stream front.
			Pop token.
		*/
		std::optional<std::string> pop();

		/*
			Skip all tokens until the next nextline token.
			The nextline token is removed from the stream
			as well.
		*/
		void skipLine();

	private:
		std::optional<std::string> current;
		std::vector<std::string> tokens;
	};
} // namespace LLS1
