#include <intcode.h>

#include <iostream>
#include <string>

int main()
{
  ic::tests();

  std::string program;
  std::getline(std::cin, program);

  for (auto inp : {1, 2}) {
    auto comp = ic::computer(program);
    comp.input(inp);
    comp.run();
  }
}
