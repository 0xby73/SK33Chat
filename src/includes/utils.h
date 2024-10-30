#pragma once

#include <iostream>

namespace Utils
{
	static void Clear_Screen() noexcept
	{
		constexpr auto clear = "\033[2J\033[1;1H";
		std::cout << clear << std::flush;
	}
}

