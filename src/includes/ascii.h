#pragma once

#include <iostream>

namespace Ascii
{
static void Header_Client() noexcept
{
  std::cout << LIGHT_BLUE << R"(
   __ _ _ __  _ __ ___  _ __
  / _` | '_ \| '_ ` _ \| '_ \
 | (_| | | | | | | | | | |_) |
  \__,_|_| |_|_| |_| |_| .__/
                       | |
                       |_|
)" << RESET;

  std::cout << "- Version 1.0\n";
  std::cout << "- 2024 - triptych <0xby73>\n\n";

  std::cout << "- Type '!help' for the command list\n\n";
}

}
