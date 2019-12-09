#include <intcode.h>

#include <iostream>
#include <string>

int main()
{
  ic::tests();

  std::string program;
  std::getline(std::cin, program);

  auto c = ic::computer(program);
  c.input(1);

  c.run();
}
