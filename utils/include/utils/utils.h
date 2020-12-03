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

std::vector<std::string> get_lines();

std::string md5_sum(std::string const& input);

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...) -> overload<Ts...>;

} // namespace utils
