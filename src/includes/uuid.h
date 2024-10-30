#pragma once

#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <cctype>

namespace Uuid
{
static std::string UUID_Gen() noexcept
{
  std::string temp = "abcdefghijklmnopqrstuvwxyz";

  std::string::iterator it;

  std::random_device rd;
  std::mt19937 g(rd());
  std::uniform_real_distribution<> dist(1, 25);

  
  std::stringstream ss;
  ss << g;
  std::string gfull = ss.str();
  
  gfull.erase(std::remove_if(gfull.begin(), gfull.end(), ::isspace), gfull.end());

  std::string gmain = gfull.substr(0, 32);

  std::string uuidres = temp += gmain;
  
  std::shuffle(uuidres.begin(), uuidres.end(), g);

  uuidres = uuidres.substr(0, 10); 
  

  return uuidres;
}
}
