#include <utils/utils.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct entry {
  template <typename It>
  entry(It begin, It end)
  {
    auto key_begin = begin;
    auto key_end   = key_begin;

    while (key_end + 1 < end && *++key_end != ':') {
    }

    key = std::string(key_begin, key_end);

    auto val_begin = key_end + 1;
    auto val_end   = val_begin;

    while (val_end + 1 < end && !std::isspace(*val_end++)) {
    }

    value = std::string(val_begin, val_end);

    length = std::distance(key_begin, val_end);
  }

  std::string key;
  std::string value;
  int         length;
};

struct group {
  bool is_valid() const
  {
    auto fields = std::vector<std::string> {"byr", "iyr", "eyr", "hgt",
                                            "hcl", "ecl", "pid"};

    return std::all_of(fields.begin(), fields.end(), [&](auto const& field) {
      return std::find_if(
                 entries.begin(), entries.end(),
                 [&](auto const& ent) { return ent.key == field; })
             != entries.end();
    });
  }

  std::vector<entry> entries;
};

int main()
{
  auto passports     = std::vector<group> {};
  auto current_group = group {};

  utils::for_each_line([&](auto const& line) {
    if (line.empty()) {
      passports.push_back(current_group);
      current_group = group {};
      return;
    }

    auto it = line.begin();
    while (it != line.end()) {
      auto ent = entry(it, line.end());
      current_group.entries.push_back(ent);
      it += ent.length;
    }
  });

  std::cout << std::count_if(
      passports.begin(), passports.end(), [](auto const& g) {
        return g.is_valid();
      }) << '\n';
}
