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

  int&       operator[](size_t);
  int const& operator[](size_t) const;

  decltype(auto) begin() const { return program_.begin(); }
  decltype(auto) end() const { return program_.end(); }

private:
  std::vector<int> program_;
};

template <typename Iterator>
computer::computer(Iterator begin, Iterator end)
    : program_(begin, end)
{
}

}
