#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace ic {

class computer {
public:
  explicit computer(std::string const&);

  template <typename Iterator>
  computer(Iterator begin, Iterator end);

private:
  std::vector<int> program_;
};

template <typename Iterator>
computer::computer(Iterator begin, Iterator end)
    : program_(begin, end)
{
}

}
