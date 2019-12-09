#include <intcode.h>

#include <iostream>
#include <string>

int main()
{
  std::string program;
  std::getline(std::cin, program);

  auto c = ic::computer(program);
}
