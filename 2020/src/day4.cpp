#include <utils/utils.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct entry {
  template <typename It>
  entry(It begin)
  {
    auto key_begin = begin;
    auto key_end   = key_begin;

    while (*++key_end != ':') {
    }

    key = std::string(key_begin, key_end);

    auto val_begin = key_end + 1;
    auto val_end   = val_begin;

    while (!std::isspace(*++val_end)) {
    }

    value = std::string(val_begin, val_end);

    length = std::distance(key_begin, val_end) + 1;
  }

  std::string key;
  std::string value;
  int         length;
};

struct group {
  bool is_valid() const { return true; }

  std::vector<entry> entries;
};

int main()
{
  auto passports = std::vector<group> {};
  std::cout << std::count_if(
      passports.begin(), passports.end(), [](auto const& g) {
        return g.is_valid();
      }) << '\n';
}
