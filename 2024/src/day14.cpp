#include <utils/ctre.h>
#include <utils/grid.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <chrono>
#include <thread>
#include <unordered_map>

using namespace ctre::literals;

constexpr auto line_pattern = "p=(-?\\d+),(-?\\d+) v=(-?\\d+),(-?\\d+)"_ctre;

static_assert(line_pattern.match("p=0,4 v=3,-3"));

constexpr std::int64_t width  = 101;
constexpr std::int64_t height = 103;

struct robot {
  utils::point position;
  utils::point velocity;

  robot(std::string_view line) noexcept
  {
    auto [all, px, py, vx, vy] = line_pattern.match(line);
    assert(all);

    position
        = {utils::to_int<std::int64_t>(px), utils::to_int<std::int64_t>(py)};
    velocity
        = {utils::to_int<std::int64_t>(vx), utils::to_int<std::int64_t>(vy)};
  }

  void step(std::int64_t n) noexcept
  {
    position += (velocity * n);

    position.x %= width;
    position.y %= height;

    if (position.x < 0) {
      position.x += width;
    }

    if (position.y < 0) {
      position.y += height;
    }
  }

  std::optional<std::int64_t> quadrant() const noexcept
  {
    // 0 1
    // 2 3
    assert(position.x < width && position.y < height);

    constexpr auto mid_x = width / 2;
    constexpr auto mid_y = width / 2;

    if (position.x == mid_x || position.y == mid_y) {
      return std::nullopt;
    }

    auto quad = std::int64_t {0};

    quad += (position.x < mid_x) ? 0 : 1;
    quad += (position.y < mid_y) ? 0 : 2;

    return quad;
  }
};

std::int64_t part_1(std::vector<robot> robots)
{
  auto scores = std::unordered_map<std::int64_t, std::int64_t> {};

  for (auto& r : robots) {
    r.step(100);

    if (auto quad = r.quadrant()) {
      scores[*quad] += 1;
    }
  }

  return scores[0] * scores[1] * scores[2] * scores[3];
}

bool score_comp(
    std::pair<std::int64_t, std::int64_t> const& a,
    std::pair<std::int64_t, std::int64_t> const& b)
{
  return a.second < b.second;
}

utils::point most_in_col_and_row(std::vector<robot> const& in)
{
  auto col_scores = std::unordered_map<std::int64_t, std::int64_t> {};
  auto row_scores = std::unordered_map<std::int64_t, std::int64_t> {};

  for (auto const& r : in) {
    col_scores[r.position.x]++;
    row_scores[r.position.y]++;
  }

  auto max = [](auto const& scores) {
    return std::ranges::max_element(scores, score_comp)->second;
  };

  return {max(col_scores), max(row_scores)};
}

utils::point best_frame(std::vector<robot> in)
{
  constexpr auto bound = std::max(width, height);

  auto best_col_score = std::numeric_limits<std::int64_t>::min();
  auto best_col       = std::numeric_limits<std::int64_t>::min();
  auto best_row_score = std::numeric_limits<std::int64_t>::min();
  auto best_row       = std::numeric_limits<std::int64_t>::min();

  for (auto frame = 0; frame <= bound; ++frame) {
    auto [col_score, row_score] = most_in_col_and_row(in);

    if (col_score > best_col_score) {
      best_col_score = col_score;
      best_col       = frame;
    }

    if (row_score > best_row_score) {
      best_row_score = row_score;
      best_row       = frame;
    }

    for (auto& r : in) {
      r.step(1);
    }
  }

  return {best_col, best_row};
}

std::int64_t part_2(std::vector<robot> const& in)
{
  auto [best_x, best_y] = best_frame(in);
  return utils::solve_crt(best_x, width, best_y, height);
}

int main()
{
  auto in = utils::construct_lines<robot>();
  fmt::print("{}\n", part_1(in));
  fmt::print("{}\n", part_2(in));
}
