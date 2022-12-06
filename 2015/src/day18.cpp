#include <utils/utils.h>

#include <string>
#include <vector>

struct display {
  static constexpr auto width  = 100;
  static constexpr auto height = 100;

  display()
      : lights(width * height, 0)
      , blit(lights)
  {
  }

  auto& at(int x, int y) { return lights.at(y * width + x); }
  auto& b_at(int x, int y) { return blit.at(y * width + x); }

  void step()
  {
    blit = lights;

    for (auto x = 0; x < width; ++x) {
      for (auto y = 0; y < height; ++y) {
        auto neighbours = 0;

        for (auto dx = -1; dx <= 1; ++dx) {
          for (auto dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
              continue;
            }

            auto nx = x + dx;
            auto ny = y + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
              neighbours += at(nx, ny);
            }
          }
        }

        switch (neighbours) {
        case 2: break;
        case 3: b_at(x, y) = 1; break;
        default: b_at(x, y) = 0; break;
        }
      }
    }

    lights = blit;
    if (stick) {
      at(0, 0)                  = 1;
      at(0, width - 1)          = 1;
      at(height - 1, 0)         = 1;
      at(height - 1, width - 1) = 1;
    }
  }

  int count() const { return std::count(lights.begin(), lights.end(), 1); }

  bool              stick;
  std::vector<char> lights;
  std::vector<char> blit;
};

display get_input()
{
  auto d = display();

  auto y = 0;
  utils::for_each_line([&](auto const& line) {
    auto x = 0;
    for (auto c : line) {
      d.at(x, y) = (c == '#' ? 1 : 0);
      x++;
    }
    y++;
  });

  return d;
}

int main()
{
  auto d  = get_input();
  auto d2 = d;

  for (int i = 0; i < 100; ++i) {
    d.step();
  }
  std::cout << d.count() << '\n';

  d2.stick                                       = true;
  d2.at(0, 0)                                    = 1;
  d2.at(0, display::width - 1)                   = 1;
  d2.at(display::height - 1, 0)                  = 1;
  d2.at(display::height - 1, display::width - 1) = 1;
  for (int i = 0; i < 100; ++i) {
    d2.step();
  }
  std::cout << d2.count() << '\n';
}
