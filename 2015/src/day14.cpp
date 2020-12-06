#include <utils/ctre.h>
#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace ctre::literals;

constexpr ctll::fixed_string name_  = "name";
constexpr ctll::fixed_string speed_ = "speed";
constexpr ctll::fixed_string time_  = "time";
constexpr ctll::fixed_string rest_  = "rest";

constexpr auto problem_time = 2503;

struct reindeer {
  template <typename Match>
  reindeer(Match const& m)
      : name(m.template get<name_>())
      , speed(utils::svtoi(m.template get<speed_>()))
      , fly_time(utils::svtoi(m.template get<time_>()))
      , rest_time(utils::svtoi(m.template get<rest_>()))
  {
  }

  int distance(int time) const
  {
    auto period   = fly_time + rest_time;
    auto complete = time / period;
    auto remain   = time % period;

    auto remain_fly = std::min(remain, fly_time);

    return (speed * fly_time * complete) + (speed * remain_fly);
  }

  std::string name;
  int         speed;
  int         fly_time;
  int         rest_time;
};

int part_1(std::vector<reindeer> const& rs, int time)
{
  auto best_r = std::max_element(
      rs.begin(), rs.end(), [&](auto const& a, auto const& b) {
        return a.distance(time) < b.distance(time);
      });

  return best_r->distance(time);
}

int part_2(std::vector<reindeer> const& rs)
{
  auto scores = std::unordered_map<std::string, int> {};

  for (auto i = 1; i <= problem_time; ++i) {
    auto max_dist = part_1(rs, i);

    for (auto const& r : rs) {
      scores.try_emplace(r.name, 0);
      if (r.distance(i) == max_dist) {
        scores[r.name] += 1;
      }
    }
  }

  return std::max_element(
             scores.begin(), scores.end(),
             [](auto const& p1, auto const& p2) {
               return p1.second < p2.second;
             })
      ->second;
}

int main()
{
  constexpr auto line_pattern
      = "(?<name>[A-Z][a-z]+) can fly (?<speed>\\d+) km/s for (?<time>\\d+) seconds, but then must rest for (?<rest>\\d+) seconds."_ctre;

  auto rs = std::vector<reindeer> {};
  utils::for_each_line(
      [&](auto const& line) { rs.emplace_back(line_pattern.match(line)); });

  std::cout << part_1(rs, problem_time) << '\n';
  std::cout << part_2(rs) << '\n';
}
