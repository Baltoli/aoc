#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

constexpr int width  = 25;
constexpr int height = 6;

std::vector<int> read_image()
{
  std::string line;
  std::getline(std::cin, line);

  auto ret = std::vector<int> {};

  for (auto c : line) {
    ret.push_back(c - 48);
  }

  return ret;
}

int n_layers(std::vector<int> const& img)
{
  assert(img.size() % (width * height) == 0 && "Incorrect image size");
  return img.size() / (width * height);
}

int count(int pix_v, int layer, std::vector<int> const& img)
{
  auto layer_size = (width * height);
  auto offset     = layer_size * layer;

  return std::count_if(
      img.begin() + offset, img.begin() + offset + layer_size,
      [pix_v](auto p) { return p == pix_v; });
}

void checksum(std::vector<int> const& img)
{

  int min_zeroes  = (width * height) + 1;
  int min_z_layer = -1;
  for (int i = 0; i < n_layers(img); ++i) {
    auto c = count(0, i, img);
    if (c < min_zeroes) {
      min_zeroes  = c;
      min_z_layer = i;
    }
  }

  std::cout << (count(1, min_z_layer, img) * count(2, min_z_layer, img))
            << '\n';
}

std::array<int, width * height> render(std::vector<int> const& img)
{
  auto ret = std::array<int, width * height> {};

  auto layers = n_layers(img);
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      auto visible = 2;
      for (int layer = 0; layer < layers && visible == 2; ++layer) {
        auto idx = (layer * width * height) + (row * width) + col;
        visible  = img[idx];
      }
      ret[col + row * width] = visible;
    }
  }

  return ret;
}

void print(std::array<int, width * height> const& img)
{
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      std::cout << img[col + row * width];
    }
    std::cout << '\n';
  }
}

int main()
{
  auto img = read_image();
  checksum(img);
  print(render(img));
}
