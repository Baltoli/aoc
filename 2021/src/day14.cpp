#include <utils/ctre.h>
#include <utils/utils.h>

#include <array>
#include <unordered_map>

std::string make_str(char a, char b)
{
  auto arr = std::array {a, b};
  return std::string(arr.begin(), arr.end());
}

struct polymer_system {
  std::unordered_map<std::string, long> state;
  std::unordered_map<std::string, char> reactions;
  std::unordered_map<char, long>        result;

  void step(long n)
  {
    for (long i = 0; i < n; ++i) {
      auto next_state = state;

      for (auto const& r : reactions) {
        if (state.contains(r.first) && state.at(r.first) > 0) {
          result.try_emplace(r.second, 0);
          result.at(r.second) += state.at(r.first);

          auto next_a = make_str(r.first[0], r.second);
          auto next_b = make_str(r.second, r.first[1]);

          next_state.try_emplace(next_a, 0);
          next_state.try_emplace(next_b, 0);

          next_state.at(next_a) += state.at(r.first);
          next_state.at(next_b) += state.at(r.first);
        }
      }

      for (auto const& r : reactions) {
        if (state.contains(r.first) && state.at(r.first) > 0) {
          next_state.at(r.first) -= state.at(r.first);
        }
      }

      state = next_state;
    }
  }

  long evaluate() const
  {
    auto cmp = [](auto a, auto b) { return a.second < b.second; };

    return std::max_element(result.begin(), result.end(), cmp)->second
           - std::min_element(result.begin(), result.end(), cmp)->second;
  }
};

polymer_system load()
{
  auto lines = utils::get_lines();

  auto state  = std::unordered_map<std::string, long> {};
  auto reacts = std::unordered_map<std::string, char> {};
  auto result = std::unordered_map<char, long> {};

  for (auto c : lines[0]) {
    result.try_emplace(c, 0);
    result.at(c)++;
  }

  for (auto i = 0; i < lines[0].size() - 1; ++i) {
    auto pair = lines[0].substr(i, 2);
    state.try_emplace(pair, 0);
    state.at(pair)++;
  }

  for (auto it = lines.begin() + 2; it != lines.end(); ++it) {
    auto m                   = ctre::match<R"((.+) -> (.+))">(*it);
    reacts[m.get<1>().str()] = m.get<2>().str()[0];
  }

  return {state, reacts, result};
}

long part_1(polymer_system p)
{
  p.step(10);
  return p.evaluate();
}

long part_2(polymer_system p)
{
  p.step(40);
  return p.evaluate();
}

int main()
{
  auto sys = load();

  std::cout << part_1(sys) << '\n';
  std::cout << part_2(sys) << '\n';
}
