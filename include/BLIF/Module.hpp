#pragma once

#include <cinttypes>


namespace LLS1::BLIF
{
	/*
		Modules which models consists of
		For extension purpopses
	*/
	class Module
	{
	public:
		/*
			Check whether object is of type T
		*/
		template<typename T>
		bool isInstanceOf() {
			return dynamic_cast<T*>(this) != nullptr;
		};

		virtual ~Module() {};
	private:
		static uint64_t id_cnt;
	};
} // namespace LLS1::BLIF
