#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

struct location {
  std::uint8_t height;
  std::int64_t score;
};

constexpr location loc_from_char(char c) noexcept
{
  return {
      .height = utils::c_to<std::uint8_t>(c),
      .score  = 0,
  };
}

class topo_grid : public utils::grid<utils::no_pad, location> {
public:
  template <typename... Args>
  topo_grid(Args&&... args)
      : utils::grid<utils::no_pad, location>(std::forward<Args>(args)...)
  {
  }
};

int main()
{
  auto in = topo_grid(utils::lines(), loc_from_char);
  fmt::print("{}\n", in.at({3, 0}).height);
}
