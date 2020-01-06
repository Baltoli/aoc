#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace utils {

std::vector<std::string>
split(std::string const& str, std::string const& delim);

template <typename Func>
void for_each_line(Func&& f)
{
  for (std::string line; std::getline(std::cin, line);) {
    std::forward<Func>(f)(line);
  }
}

}
