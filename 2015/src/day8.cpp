#include <utils/utils.h>

#include <cassert>
#include <iostream>
#include <sstream>
#include <string_view>

int memory_req(std::string_view str)
{
  assert(str[0] == '"' && str[str.size() - 1] == '"' && "Not a string literal");

  str = str.substr(1, str.size() - 2);

  auto mem = 0;
  for (auto idx = 0; idx < str.size();) {
    auto chr = str[idx];

    if (chr == '\\') {
      if (str[idx + 1] == 'x') {
        mem += 1;
        idx += 4;
      } else {
        mem += 1;
        idx += 2;
      }
    } else {
      mem += 1;
      idx += 1;
    }
  }

  return mem;
}

int encode_req(std::string_view str)
{
  int chars = 2;
  for (auto ch : str) {
    if (ch == '\\' || ch == '"') {
      chars += 2;
    } else {
      chars += 1;
    }
  }
  return chars;
}

int main()
{
  int literal_chars = 0;
  int memory_chars  = 0;
  int enc_chars     = 0;

  utils::for_each_line([&](auto const& line) {
    literal_chars += line.size();
    memory_chars += memory_req(line);
    enc_chars += encode_req(line);
  });

  std::cout << (literal_chars - memory_chars) << '\n';
  std::cout << (enc_chars - literal_chars) << '\n';
}
