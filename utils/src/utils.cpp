#include "md5.h"

#include <utils/utils.h>

#include <array>
#include <iomanip>
#include <iostream>
#include <sstream>

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

std::string md5_sum(std::string const& input)
{
  constexpr auto buf_size = 64;

  MD5_CTX ctx;
  MD5Init(&ctx);

  auto end = input.size();
  for (int i = 0; i < input.size(); i += buf_size) {
    auto len = (i + buf_size >= end) ? (end - i) : buf_size;
    MD5Update(&ctx, (unsigned char*)(input.c_str() + i), len);
  }

  auto ret = std::array<unsigned char, 16> {};
  MD5Final(ret.data(), &ctx);

  std::stringstream ss;
  for (auto i : ret) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)i;
  }

  return ss.str();
}

}
