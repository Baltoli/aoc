#include <algorithm>
#include <array>
#include <iostream>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using vec3 = std::array<long, 3>;

struct planet {
  planet(std::string const& pos);

  long energy() const;

  void dump() const;

  vec3 position;
  vec3 velocity = {0, 0, 0};
};

struct orbit {
  orbit() = default;

  void step();
  long energy() const;
  long get_period() const;

  std::vector<planet> planets = {};

  std::array<long, 3>                        period      = {0, 0, 0};
  long                                       steps       = 0;
  std::array<std::set<std::vector<long>>, 3> past_states = {};
};

planet::planet(std::string const& pos)
{
  auto coords = std::array {'x', 'y', 'z'};
  for (long i = 0; i < 3; ++i) {
    auto start  = pos.find(coords[i]) + 2;
    auto end    = pos.find(',', start);
    position[i] = std::stoi(pos.substr(start, end - start));
  }
}

long planet::energy() const
{
  auto abs_acc = [](auto a, auto c) { return a + std::abs(c); };

  auto pot = std::accumulate(position.begin(), position.end(), 0, abs_acc);
  auto kin = std::accumulate(velocity.begin(), velocity.end(), 0, abs_acc);

  return pot * kin;
}

void orbit::step()
{
  auto states = std::array<std::vector<long>, 3> {};
  for (long i = 0; i < 3; ++i) {
    for (auto const& p : planets) {
      states[i].push_back(p.position[i]);
      states[i].push_back(p.velocity[i]);
    }

    if (period[i] == 0
        && past_states[i].find(states[i]) != past_states[i].end()) {
      period[i] = steps;
    }

    past_states[i].insert(states[i]);
  }

  for (auto& p1 : planets) {
    for (auto& p2 : planets) {
      if (&p1 < &p2) {
        for (long i = 0; i < 3; ++i) {
          if (p1.position[i] > p2.position[i]) {
            p1.velocity[i]--;
            p2.velocity[i]++;
          } else if (p1.position[i] < p2.position[i]) {
            p1.velocity[i]++;
            p2.velocity[i]--;
          }
        }
      }
    }
  }

  for (auto& p : planets) {
    for (long i = 0; i < 3; ++i) {
      p.position[i] += p.velocity[i];
    }
  }

  steps++;
}

long orbit::energy() const
{
  return std::accumulate(
      planets.begin(), planets.end(), 0,
      [](auto acc, auto p) { return acc + p.energy(); });
}

long orbit::get_period() const
{
  if (std::any_of(
          period.begin(), period.end(), [](auto p) { return p == 0; })) {
    return 0;
  }

  return std::accumulate(
      period.begin(), period.end(), 1L,
      [](auto acc, auto p) { return std::lcm(acc, p); });
}

void planet::dump() const
{
  std::cout << "pos<x=" << position[0] << ", y=" << position[1]
            << ", z=" << position[2] << ">, vel=<x=" << velocity[0]
            << ", y=" << velocity[1] << ", z=" << velocity[2] << ">\n";
}

int main()
{
  auto sys = orbit();
  for (std::string line; std::getline(std::cin, line);) {
    sys.planets.emplace_back(line);
  }
  auto s2 = sys;

  for (long i = 0; i < 1000; ++i) {
    sys.step();
  }

  std::cout << sys.energy() << '\n';

  while (s2.get_period() == 0) {
    s2.step();
  }

  std::cout << s2.get_period() << '\n';
}
