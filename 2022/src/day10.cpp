#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <optional>
#include <sstream>

class cpu {
public:
  cpu(std::vector<std::string> p)
      : program_(p)
      , x_(1)
      , cycle_(1)
      , important_signal_(0)
      , crt_ {}
      , pixel_(0)
  {
  }

  long execute()
  {
    auto cycle = [&] {
      if (is_important_cycle()) {
        important_signal_ += cycle_ * x_;
      }

      if (std::abs(x_ - pixel_) <= 1) {
        crt_ << "█";
      } else {
        crt_ << " ";
      }

      if ((++pixel_) % 40 == 0) {
        pixel_ = 0;
        crt_ << '\n';
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

  std::string message() const { return crt_.str(); }

private:
  bool is_important_cycle() const { return (cycle_ - 20) % 40 == 0; }

  std::vector<std::string> program_;

  long x_;
  long cycle_;

  long important_signal_;

  std::ostringstream crt_;
  long               pixel_;
};

int main()
{
  auto c = cpu(utils::get_lines());

  fmt::print("{}\n", c.execute());
  fmt::print("{}\n", c.message());

  /* utils::for_each_line([&](auto const& line) { */
  /*   if (c.accepts()) { */
  /*     c.execute(line); */
  /*   } */
  /* }); */
}
