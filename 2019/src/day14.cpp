#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>

struct material {
  std::string name;
  int         number;

  material(std::string const& line)
  {
    auto end_num = line.find(" ");
    name         = line.substr(end_num + 1);
    number       = std::stoi(line.substr(0, end_num));
  }

  friend std::ostream& operator<<(std::ostream& os, material const& m)
  {
    os << m.number << ' ' << m.name;
    return os;
  }
};

struct reaction {
  std::vector<material> lhs;
  material              rhs;

  reaction(std::string const& line)
      : lhs {}
      , rhs("-1 BAD")
  {
    auto rhs_s = line.substr(line.find(">") + 2, std::string::npos);
    rhs        = material(rhs_s);

    auto lhs_s = line.substr(0, line.find("=") - 1);
    auto ptr   = 0;

    while (true) {
      auto next_comma = lhs_s.find(",", ptr);
      auto start      = ptr + (ptr == 0 ? 0 : 1);
      auto str        = lhs_s.substr(start, next_comma - start);

      lhs.emplace_back(str);

      if (next_comma == std::string::npos) {
        break;
      }

      ptr = next_comma + 1;
    }
  }

  friend std::ostream& operator<<(std::ostream& os, reaction const& r)
  {
    auto comma = "";
    for (auto const& m : r.lhs) {
      os << comma << m;
      comma = ", ";
    }
    os << " => " << r.rhs;
    return os;
  }
};

int main()
{
  auto reacts = std::vector<reaction> {};
  for (std::string line; std::getline(std::cin, line);) {
    reacts.emplace_back(line);
  }
}
