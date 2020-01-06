#include <utils/utils.h>

#include <iostream>

namespace utils {

std::vector<std::string> split(std::string const& str, std::string const& delim)
{
  auto ret = std::vector<std::string> {};

  auto start = 0;
  auto end   = std::string::npos;

  while (true) {
    end = str.find(delim, start);
    ret.push_back(str.substr(start, end - start));

    if (end == std::string::npos) {
      break;
    }

    start = end + delim.size();
  }

  return ret;
}

}
