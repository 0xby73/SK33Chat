#pragma once

#include <iostream>

namespace Utils
{
	static void Clear_Screen() noexcept
	{
		constexpr auto clear = "\033[2J\033[1;1H";
		std::cout << clear << std::flush;
	}

    const std::string Get_Time() noexcept
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);

        strftime(buf, sizeof(buf), "[ %X ]", &tstruct);

        return buf;
    }

}
