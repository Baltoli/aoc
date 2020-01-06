#include <array>
#include <iostream>
#include <map>
#include <vector>

class santa {
public:
  using loc = std::pair<int, int>;

  template <typename T>
  santa(T const& insts);

  int unique_houses() const;
  int unique_houses(santa const& other) const;

private:
  std::map<loc, int> deliveries_ {};
};

template <typename T>
santa::santa(T const& insts)
{
  auto ptr = std::pair {0, 0};
  for (auto c : insts) {
    deliveries_.try_emplace(ptr, 0);
    deliveries_[ptr]++;

    switch (c) {
    case '^':
      ptr.second++;
      break;
    case '>':
      ptr.first++;
      break;
    case 'v':
      ptr.second--;
      break;
    case '<':
      ptr.first--;
      break;
    }
  }
}

int santa::unique_houses() const { return deliveries_.size(); }

int santa::unique_houses(santa const& other) const
{
  auto all_ds = deliveries_;
  for (auto const& [k, v] : other.deliveries_) {
    all_ds.try_emplace(k, 0);
    all_ds[k] += v;
  }
  return all_ds.size();
}

std::array<santa, 2> make_robo_pair(std::string const& line)
{
  auto ss = std::vector<char> {};
  auto rs = std::vector<char> {};

  for (auto i = 0; i < line.size(); ++i) {
    if (i % 2 == 0) {
      ss.push_back(line[i]);
    } else {
      rs.push_back(line[i]);
    }
  }

  return {ss, rs};
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto s = santa(line);
  std::cout << s.unique_houses() << '\n';

  auto [ss, rs] = make_robo_pair(line);
  std::cout << ss.unique_houses(rs) << '\n';
}
