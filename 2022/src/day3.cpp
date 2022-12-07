#include <utils/utils.h>

#include <cassert>
#include <cctype>
#include <iostream>
#include <map>
#include <numeric>
#include <set>

int priority(char c)
{
  if (std::islower(c)) {
    return c - 96;
  } else if (std::isupper(c)) {
    return c - 38;
  } else {
    abort();
  }
}

class rucksack {
public:
  rucksack(std::string_view line)
  {
    assert(line.size() % 2 == 0 && "Odd size line");

    for (auto i = 0u; i < line.size(); ++i) {
      if (i < line.size() / 2) {
        left_.try_emplace(line[i], 0);
        left_[line[i]] += 1;
      } else {
        right_.try_emplace(line[i], 0);
        right_[line[i]] += 1;
      }
    }
  }

  char shared() const
  {
    for (auto c : left_) {
      if (right_.find(c.first) != right_.end()) {
        return c.first;
      }
    }

    assert(false && "Bad rucksack!");
  }

private:
  std::map<char, int> left_;
  std::map<char, int> right_;
};

int main()
{
  auto data = utils::construct_lines<rucksack>();
  auto sum  = std::accumulate(
       data.begin(), data.end(), 0,
       [](auto acc, auto const& r) { return acc + priority(r.shared()); });
  std::cout << sum << '\n';
}
