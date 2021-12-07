#include <utils/utils.h>

#include <numeric>

long score_1(std::vector<long> const& in, long pos)
{
  return std::accumulate(in.begin(), in.end(), 0L, [pos](auto acc, auto i) {
    return acc + std::abs(pos - i);
  });
}

long score_2(std::vector<long> const& in, long pos)
{
  return std::accumulate(in.begin(), in.end(), 0L, [pos](auto acc, auto i) {
    auto dist    = std::abs(pos - i);
    auto sq_dist = (dist * (dist + 1)) / 2;
    return acc + sq_dist;
  });
}

template <typename F>
long compute(std::vector<long> in, F&& score)
{
  std::sort(in.begin(), in.end());

  long best_score = std::numeric_limits<long>::max();

  for (auto i = 0; i < in.size(); ++i) {
    auto s = score(in, i);

    if (s < best_score) {
      best_score = s;
    } else {
      return best_score;
    }
  }

  return best_score;
}

int main()
{
  auto data  = utils::get_lines()[0];
  auto state = std::vector<long> {};
  for (auto s : utils::split(data, ",")) {
    state.push_back(utils::stoi(s));
  }

  std::cout << compute(state, score_1) << '\n';
  std::cout << compute(state, score_2) << '\n';

  return 0;
}
