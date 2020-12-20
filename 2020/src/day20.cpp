#include <utils/utils.h>

#include <bitset>
#include <iostream>
#include <string>
#include <unordered_set>

struct tile {
};

namespace std {
template <>
struct hash<tile> {
  size_t operator()(tile const& t) const { return 0; }
};
} // namespace std

using tile_set = std::unordered_set<tile>;

tile_set get_input()
{
  auto ret = tile_set {};
  return ret;
}

int main() { }
