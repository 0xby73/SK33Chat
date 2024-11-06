#pragma once

#include <iostream>

namespace Ascii
{
static void Header_Client() noexcept
{
  std::cout << LIGHT_BLUE << R"(
  ____  _  ____________  ____ _   _    _  _____
 / ___|| |/ /___ /___ / / ___| | | |  / \|_   _|
 \___ \| ' /  |_ \ |_ \| |   | |_| | / _ \ | |
  ___) | . \ ___) |__) | |___|  _  |/ ___ \| |
 |____/|_|\_\____/____/ \____|_| |_/_/   \_\_|

)" << RESET;

  std::cout << "- Version 1.0\n";
  std::cout << "- 2024 - Triptych <0xby73>\n\n";

  std::cout << "- Type '!help' for the command list\n\n";
}

}
