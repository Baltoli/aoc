#include <iostream>
#include <limits>
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

int multiplier(int req, int supply)
{
  if (req % supply == 0) {
    return req / supply;
  } else {
    return (req / supply) + 1;
  }
}

int best_cost(material m, std::vector<reaction> const& recs)
{
  std::cout << "Making " << m << '\n';

  if (m.name == "ORE") {
    return m.number;
  }

  auto best = std::numeric_limits<int>::max();

  for (auto const& r : recs) {
    if (r.rhs.name == m.name) {
      int total = 0;
      int mult  = multiplier(m.number, r.rhs.number);

      for (auto mat : r.lhs) {
        mat.number *= mult;
        total += best_cost(mat, recs);
      }

      if (total < best) {
        best = total;
      }
    }
  }

  return best;
}

int main()
{
  auto reacts = std::vector<reaction> {};
  for (std::string line; std::getline(std::cin, line);) {
    reacts.emplace_back(line);
  }

  std::cout << best_cost(material("1 E"), reacts) << '\n';
}
