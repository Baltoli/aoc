#include <intcode.h>

#include <iostream>
#include <string>

int main()
{
  auto test_1 = "109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99";
  auto test_2 = "1102,34915192,34915192,7,4,7,99,0";
  auto test_3 = "104,1125899906842624,99";

  ic::tests();

  std::string program;
  std::getline(std::cin, program);

  auto comp = ic::computer(test_3);
  /* comp.input(1); */

  comp.run();
}
