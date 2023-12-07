#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <algorithm>
#include <limits>
#include <vector>

using namespace ctre::literals;

constexpr auto seeds_pattern    = "seeds: (.*)"_ctre;
constexpr auto map_head_pattern = "(.*)-to-(.*) map:"_ctre;
constexpr auto range_pattern    = "(\\d+) (\\d+) (\\d+)"_ctre;

struct range {
  long dest;
  long source;
  long size;
};

class range_map {
public:
  range_map()
      : ranges_ {}
  {
  }

  long get(long input) const
  {
    for (auto const& range : ranges_) {
      if (input >= range.source && input < range.source + range.size) {
        auto diff = input - range.source;
        return range.dest + diff;
      }
    }

    return input;
  }

  void add(range r) { ranges_.push_back(r); }

private:
  std::vector<range> ranges_;
};

long chained_lookup(long input, std::vector<range_map> const& maps)
{
  for (auto const& map : maps) {
    input = map.get(input);
  }

  return input;
}

struct input {
  std::vector<long>      seeds;
  std::vector<range_map> maps;

  input(std::vector<std::string> const& lines)
      : seeds {}
      , maps {}
  {
    auto current_map = range_map {};
    auto started     = false;

    for (auto const& line : lines) {
      if (line.size() == 0) {
        if (started) {
          maps.push_back(current_map);
        }
      } else if (auto [sh, s] = seeds_pattern.match(line); sh) {
        for (auto const& seed_id : utils::split(s.str(), " ")) {
          seeds.push_back(utils::stol(seed_id));
        }
      } else if (auto [mh, _from, _to] = map_head_pattern.match(line); mh) {
        current_map = range_map {};
        started     = true;
      } else if (auto [rh, dst, src, len] = range_pattern.match(line); rh) {
        current_map.add(
            {utils::stol(dst.str()), utils::stol(src.str()),
             utils::stol(len.str())});
      }
    }

    maps.push_back(current_map);
  }
};

long part_1(input const& in)
{
  auto score = [&](auto seed) { return chained_lookup(seed, in.maps); };

  auto min_seed = std::min_element(
      in.seeds.begin(), in.seeds.end(),
      [&](auto sa, auto sb) { return score(sa) < score(sb); });

  return score(*min_seed);
}

long part_2(input const& in)
{
  auto best_loc = std::numeric_limits<long>::max();

  for (auto i = 0u; i < in.seeds.size(); i += 2) {
    auto start = in.seeds[i];
    auto len   = in.seeds[i + 1];

    for (auto seed = start; seed < start + len; ++seed) {
      auto loc = chained_lookup(seed, in.maps);
      if (loc < best_loc) {
        best_loc = loc;
      }
    }
  }

  return best_loc;
}

int main()
{
  auto in = input(utils::get_lines());

  fmt::print("{}\n", part_1(in));
  fmt::print("{}\n", part_2(in));
}
