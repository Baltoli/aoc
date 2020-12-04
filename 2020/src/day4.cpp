#include <utils/utils.h>

#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <set>
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

    while (val_end != end) {
      if (std::isspace(*val_end)) {
        break;
      }

      val_end++;
    }

    value = std::string(val_begin, val_end);

    length = std::distance(key_begin, val_end);
    if (val_end != end) {
      ++length;
    }
  }

  std::string key;
  std::string value;
  int         length;
};

struct group {
  std::string const& get(std::string const& field) const
  {
    for (auto const& e : entries) {
      if (e.key == field) {
        return e.value;
      }
    }

    assert(false && "Looking up bad field");
  }

  bool fields_present() const
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

  // I'm not proud of this
  bool is_valid() const
  {
    if (!fields_present()) {
      return false;
    }

    auto valid = true;

    auto byr = std::stoi(get("byr"));
    valid    = valid && (byr >= 1920 && byr <= 2002);

    auto iyr = std::stoi(get("iyr"));
    valid    = valid && (iyr >= 2010 && iyr <= 2020);

    auto eyr = std::stoi(get("eyr"));
    valid    = valid && (eyr >= 2020 && eyr <= 2030);

    auto hgt = get("hgt");
    if (hgt.length() < 4) {
      return false;
    }

    auto unit = hgt.substr(hgt.length() - 2, 2);
    if (unit == "cm") {
      auto val = std::stoi(hgt.substr(0, 3));
      valid    = valid && (val >= 150 && val <= 193);
    } else if (unit == "in") {
      auto val = std::stoi(hgt.substr(0, 2));
      valid    = valid && (val >= 59 && val <= 76);
    } else {
      valid = false;
    }

    auto hcl = get("hcl");
    if (hcl.length() != 7) {
      return false;
    }

    if (hcl[0] != '#') {
      return false;
    }

    for (auto i = 1; i < 6; ++i) {
      valid = valid
              && ((hcl[i] >= 97 && hcl[i] <= 102)
                  || (hcl[i] >= 48 && hcl[i] <= 57));
    }

    auto ecl  = get("ecl");
    ecl       = ecl.substr(0, std::min(ecl.size(), 3ul));
    auto ecls = std::set<std::string> {"amb", "blu", "brn", "gry",
                                       "grn", "hzl", "oth"};
    valid     = valid && (ecls.find(ecl) != ecls.end());

    auto pid = get("pid");
    if (pid.size() != 9) {
      return false;
    }

    for (auto i = 0; i < 9; ++i) {
      valid = valid && (pid[i] >= 48 && pid[i] <= 57);
    }

    return valid;
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
        return g.fields_present();
      }) << '\n';

  std::cout << std::count_if(
      passports.begin(), passports.end(), [](auto const& g) {
        return g.is_valid();
      }) << '\n';
}
