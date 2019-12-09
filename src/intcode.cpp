#include "intcode.h"

#include <iostream>
#include <string>

namespace ic {

computer::computer(std::string const& code)
    : program_ {}
{
  auto ptr = 0;

  while (true) {
    auto next_comma = code.find(",", ptr);
    auto str        = code.substr(ptr, next_comma - ptr);

    program_.push_back(std::stoi(str));

    if (next_comma == std::string::npos) {
      break;
    }

    ptr = next_comma + 1;
  }
}

int& computer::operator[](size_t i) { return program_[i]; }

int const& computer::operator[](size_t i) const { return program_[i]; }

}
