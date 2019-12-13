#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using vec3 = std::array<int, 3>;

struct planet {
  planet(std::string const& pos);

  int energy() const;

  void dump() const;

  vec3 position;
  vec3 velocity = {0, 0, 0};
};

struct orbit {
  orbit() = default;

  void step();
  int  energy() const;

  std::vector<planet> planets = {};
};

planet::planet(std::string const& pos)
{
  auto coords = std::array {'x', 'y', 'z'};
  for (int i = 0; i < 3; ++i) {
    auto start  = pos.find(coords[i]) + 2;
    auto end    = pos.find(',', start);
    position[i] = std::stoi(pos.substr(start, end - start));
  }
}

int planet::energy() const
{
  auto abs_acc = [](auto a, auto c) { return a + std::abs(c); };

  auto pot = std::accumulate(position.begin(), position.end(), 0, abs_acc);
  auto kin = std::accumulate(velocity.begin(), velocity.end(), 0, abs_acc);

  return pot * kin;
}

void orbit::step()
{
  for (auto& p1 : planets) {
    for (auto& p2 : planets) {
      if (&p1 < &p2) {
        for (int i = 0; i < 3; ++i) {
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
    for (int i = 0; i < 3; ++i) {
      p.position[i] += p.velocity[i];
    }
  }
}

int orbit::energy() const
{
  return std::accumulate(
      planets.begin(), planets.end(), 0,
      [](auto acc, auto p) { return acc + p.energy(); });
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

  for (int i = 0; i < 1000; ++i) {
    sys.step();
  }

  std::cout << sys.energy() << '\n';
}
