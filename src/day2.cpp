#include <intcode.h>

#include <iostream>
#include <string>

int main()
{
  ic::tests();

  std::string program;
  std::getline(std::cin, program);

  auto prog          = ic::computer(program);
  auto initial_state = prog;

  prog[1] = 12;
  prog[2] = 2;
  prog.run();
  std::cout << prog[0] << '\n';

  for (int noun = 0; noun < 100; ++noun) {
    for (int verb = 0; verb < 100; ++verb) {
      auto test_prog = initial_state;
      test_prog[1]   = noun;
      test_prog[2]   = verb;

      test_prog.run();

      if (test_prog[0] == 19690720) {
        std::cout << (noun * 100 + verb) << '\n';
      }
    }
  }
}
