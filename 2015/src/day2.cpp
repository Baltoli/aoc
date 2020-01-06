#include <cassert>
#include <iostream>
#include <string>

class parcel {
public:
  parcel(std::string const&);

private:
  int length_ = -1;
  int width_  = -1;
  int height_ = -1;
};

parcel::parcel(std::string const& spec)
{
  assert(length_ > 0 && "Parse error");
  assert(width_ > 0 && "Parse error");
  assert(height_ > 0 && "Parse error");
}

int main()
{
  for (std::string line; std::getline(std::cin, line);) {
    auto p = parcel(line);
  }
}
