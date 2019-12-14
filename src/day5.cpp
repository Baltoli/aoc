#include <intcode.h>

#include <iostream>
#include <string>

int main()
{
  ic::tests();

  std::string program;
  std::getline(std::cin, program);

  auto c = ic::computer(program, [](auto) {});
  c.input(1);

  while (true) {
    auto out = c.run();
    if (out != 0) {
      std::cout << out << '\n';
      break;
    }
  }

  auto c2 = ic::computer(program, [](auto) {});
  c2.input(5);
  std::cout << c2.run() << '\n';
}
