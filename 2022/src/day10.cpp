#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <optional>

class cpu {
public:
  cpu(std::vector<std::string> p)
      : program_(p)
      , x_(1)
      , cycle_(1)
      , important_signal_(0)
  {
  }

  long execute()
  {
    auto cycle = [&] {
      if (is_important_cycle()) {
        important_signal_ += cycle_ * x_;
      }

      cycle_ += 1;
    };

    for (auto const& inst : program_) {
      if (ctre::match<"noop">(inst)) {
        cycle();
      } else if (auto m = ctre::match<"addx (.*)">(inst)) {
        auto new_x = utils::svtoi(m.template get<1>());

        cycle();
        cycle();

        x_ += new_x;
      }
    }

    return important_signal_;
  }

private:
  bool is_important_cycle() const { return (cycle_ - 20) % 40 == 0; }

  std::vector<std::string> program_;

  long x_;
  long cycle_;

  long important_signal_;
};

int main()
{
  auto c = cpu(utils::get_lines());

  fmt::print("{}\n", c.execute());

  /* utils::for_each_line([&](auto const& line) { */
  /*   if (c.accepts()) { */
  /*     c.execute(line); */
  /*   } */
  /* }); */
}
