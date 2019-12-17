#include <iostream>
#include <string>
#include <vector>

struct material {
  std::string name;
  int         number;
};

struct reaction {
  std::vector<material> lhs;
  material              rhs;
};

int main() {}
