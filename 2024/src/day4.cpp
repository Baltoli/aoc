#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <generator>
#include <ranges>

namespace views = std::ranges::views;

class xmas_grid : public utils::grid<utils::fill_pad<'.'>> {
public:
  template <std::size_t N>
  std::generator<std::string> words_at(utils::point p) const
  {
    for (auto nd : neighbour_deltas()) {
      auto buf = std::array<char, N> {};
      for (auto i = 0; i < buf.size(); ++i) {
        buf[i] = at(p + nd * i);
      }

      co_yield std::string(buf.begin(), buf.end());
    }
  }

  template <std::size_t N>
  std::generator<std::string> words() const
  {
    for (auto p : coords()) {
      for (auto w : words_at<N>(p)) {
        co_yield w;
      }
    }
  }

  bool xmas_at(utils::point p) const
  {
    auto a = at(p + utils::point {-1, -1});
    auto b = at(p + utils::point {1, -1});
    auto c = at(p + utils::point {-1, 1});
    auto d = at(p + utils::point {1, 1});

    auto down = ((a == 'M') && (d == 'S')) || ((a == 'S') && (d == 'M'));
    auto up   = ((c == 'M') && (b == 'S')) || ((c == 'S') && (b == 'M'));

    return at(p) == 'A' && down && up;
  }
};

std::int64_t part_1(xmas_grid const& g)
{
  auto words
      = g.words<4>() | views::filter([](auto const& w) { return w == "XMAS"; });

  return utils::count(words);
}

std::int64_t part_2(xmas_grid const& g)
{
  auto crosses
      = g.coords() | views::filter([&](auto const& p) { return g.xmas_at(p); });

  return utils::count(crosses);
}

int main()
{
  auto in = xmas_grid(utils::get_lines());
  fmt::print("{}\n", part_1(in));
  fmt::print("{}\n", part_2(in));
}
