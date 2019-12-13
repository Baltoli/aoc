#include <array>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using vec3 = std::array<int, 3>;

struct planet {
  planet(std::string const& pos);

  int energy() const;

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
  ;
  ;
}

int planet::energy() const { return 0; }

void orbit::step()
{
  ;
  ;
}

int orbit::energy() const
{
  return std::accumulate(
      planets.begin(), planets.end(), 0,
      [](auto acc, auto p) { return acc + p.energy(); });
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
