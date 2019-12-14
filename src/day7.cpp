#include <intcode.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

int max_signal(std::string const& program)
{
  int  output    = 0;
  auto initial_c = ic::computer(program, [](auto) {});

  int  max_s  = 0;
  auto phases = std::vector {0, 1, 2, 3, 4};
  do {
    output = 0;
    for (auto p : phases) {
      auto comp = initial_c;

      comp.input(p);
      comp.input(output);
      output = comp.run();
    }

    if (output > max_s) {
      max_s = output;
    }
  } while (std::next_permutation(phases.begin(), phases.end()));

  return max_s;
}

int max_feedback(std::string const& program)
{
  int  max_f  = 0;
  auto phases = std::vector {5, 6, 7, 8, 9};
  do {
    auto outs = std::array<int, 5> {0, 0, 0, 0, 0};

    auto amps = std::vector<ic::computer> {};
    for (auto i = 0; i < 5; ++i) {
      amps.emplace_back(
          program, [&outs, i](int o) {}, true);
    }

    auto halted = [&amps] {
      return std::all_of(amps.begin(), amps.end(), [](auto const& comp) {
        return comp.halted();
      });
    };

    for (auto i = 0; i < phases.size(); ++i) {
      amps[i].input(phases[i]);
    }

    auto prev = std::array {4, 0, 1, 2, 3};
    while (!halted()) {
      for (int i = 0; i < 5; ++i) {
        amps[i].input(outs[prev[i]]);

        auto out = amps[i].run();
        if (!amps[i].halted()) {
          outs[i] = out;
        }
      }
    }

    if (outs[4] > max_f) {
      max_f = outs[4];
    }

  } while (std::next_permutation(phases.begin(), phases.end()));

  return max_f;
}

int main()
{
  ic::tests();

  std::string program;
  std::getline(std::cin, program);

  std::cout << max_signal(program) << '\n';
  std::cout << max_feedback(program) << '\n';
}
