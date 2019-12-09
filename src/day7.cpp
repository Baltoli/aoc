#include <intcode.h>

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  ic::tests();

  std::string program;
  std::getline(std::cin, program);

  int  output    = 0;
  auto initial_c = ic::computer(program, [&output](int o) { output = o; });

  int  max_s  = 0;
  auto phases = std::vector {0, 1, 2, 3, 4};
  do {
    output = 0;
    for (auto p : phases) {
      auto comp = initial_c;

      comp.input(p);
      comp.input(output);
      comp.run();
    }

    if (output > max_s) {
      max_s = output;
    }
  } while (std::next_permutation(phases.begin(), phases.end()));

  std::cout << max_s << '\n';
}
