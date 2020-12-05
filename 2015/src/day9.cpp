#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>

using dist_t = std::map<std::pair<std::string, std::string>, int>;

int total_dist(std::vector<std::string> const& seq, dist_t const& dists)
{
  auto d = 0;
  for (auto i = 0; i < seq.size() - 1; ++i) {
    d += dists.at({seq[i], seq[i + 1]});
  }
  return d;
}

int main()
{
  auto dists = dist_t {};
  auto locs  = std::set<std::string> {};

  utils::for_each_line([&](auto const& line) {
    auto s1   = utils::split(line, " to ");
    auto from = s1[0];
    auto s2   = utils::split(s1[1], " = ");
    auto to   = s2[0];
    auto d    = std::stoi(s2[1]);

    dists.insert({{from, to}, d});
    dists.insert({{to, from}, d});

    locs.insert(from);
    locs.insert(to);
  });

  auto perm = std::vector<std::string>(locs.begin(), locs.end());
  std::sort(perm.begin(), perm.end());

  auto min_dist = std::numeric_limits<int>::max();
  auto max_dist = 0;

  while (std::next_permutation(perm.begin(), perm.end())) {
    auto d = total_dist(perm, dists);
    if (d < min_dist) {
      min_dist = d;
    }

    if (d > max_dist) {
      max_dist = d;
    }
  }

  std::cout << min_dist << '\n';
  std::cout << max_dist << '\n';
}
