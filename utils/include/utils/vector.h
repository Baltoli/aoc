#pragma once

#include <array>
#include <cstdlib>
#include <tuple>
#include <type_traits>

#include <fmt/format.h>

namespace utils {

template <typename T, std::size_t N>
  requires(std::is_arithmetic_v<T>)
class vec_t {
public:
  template <typename... Args>
  constexpr vec_t(Args... args) noexcept
      : elements_ {T(args)...}
  {
  }

  template <std::size_t I>
  constexpr auto get() const noexcept
  {
    using std::get;
    return get<I>(elements_);
  }

  constexpr auto x() const noexcept { return get<0>(); }
  constexpr auto y() const noexcept { return get<1>(); }
  constexpr auto z() const noexcept { return get<2>(); }

  constexpr auto magnitude_sq() const noexcept
  {
    return std::apply(
        [](auto... args) { return (args + ... + 0); },
        (*this * *this).elements_);
  }

  auto magnitude() const noexcept { return T(std::sqrt(magnitude_sq())); }

private:
  std::array<T, N> elements_;
};

template <typename T, typename... Args>
constexpr auto vec(Args&&... args)
{
  return vec_t<T, sizeof...(Args)>(args...);
}

template <typename T, std::size_t N, typename Func>
constexpr auto
zip_with(vec_t<T, N> const& a, vec_t<T, N> const& b, Func&& f) noexcept(
    noexcept(std::forward<Func>(f)(std::declval<T>(), std::declval<T>())))
{
  auto pack = std::make_index_sequence<N>();
  return [&a, &b, &f]<std::size_t... Inds>(std::index_sequence<Inds...>) {
    return vec_t<T, N>(std::forward<Func>(f)(
        a.template get<Inds>(), b.template get<Inds>())...);
  }(pack);
}

template <typename T, std::size_t N, typename Func>
constexpr void for_each(vec_t<T, N> const& v, Func&& f) noexcept(
    noexcept(std::forward<Func>(f)(std::declval<T>())))
{
  [&v, &f]<std::size_t... Inds>(std::index_sequence<Inds...>) {
    (std::forward<Func>(f)(v.template get<Inds>()), ...);
  }(std::make_index_sequence<N>());
}

template <typename T, std::size_t N>
constexpr vec_t<T, N>
operator+(vec_t<T, N> const& a, vec_t<T, N> const& b) noexcept
{
  return zip_with(a, b, std::plus<T> {});
}

template <typename T, std::size_t N>
constexpr vec_t<T, N>& operator+=(vec_t<T, N>& a, vec_t<T, N> const& b) noexcept
{
  a = a + b;
  return a;
}

template <typename T, std::size_t N>
constexpr vec_t<T, N>
operator*(vec_t<T, N> const& a, vec_t<T, N> const& b) noexcept
{
  return zip_with(a, b, std::multiplies<T> {});
}

template <typename T, std::size_t N, typename Int>
constexpr vec_t<T, N> operator*(vec_t<T, N> const& a, Int s) noexcept
{
  auto pack = std::make_index_sequence<N>();
  return [&a, s]<std::size_t... Inds>(std::index_sequence<Inds...>) {
    return vec_t<T, N>((a.template get<Inds>() * s)...);
  }(pack);
}

template <typename T, std::size_t N>
constexpr vec_t<T, N>& operator*=(vec_t<T, N>& a, vec_t<T, N> const& b) noexcept
{
  a = a * b;
  return a;
}

template <typename T, std::size_t N, typename Int>
constexpr vec_t<T, N>& operator*=(vec_t<T, N>& a, Int s) noexcept
{
  a = a * s;
  return a;
}

template <typename T, std::size_t N>
constexpr vec_t<T, N>
operator-(vec_t<T, N> const& a, vec_t<T, N> const& b) noexcept
{
  return zip_with(a, b, std::minus<T> {});
}

template <typename T, std::size_t N>
constexpr vec_t<T, N>& operator-=(vec_t<T, N>& a, vec_t<T, N> const& b) noexcept
{
  a = a - b;
  return a;
}

} // namespace utils

template <typename T, std::size_t N>
struct fmt::formatter<::utils::vec_t<T, N>> {
  constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const ::utils::vec_t<T, N>& vec, FormatContext& ctx)
  {
    auto comma       = "";
    auto format_item = [&] { };

    fmt::format_to(ctx.out(), "[");

    for_each(vec, [&](auto elt) {
      fmt::format_to(ctx.out(), "{}{}", comma, elt);
      comma = ", ";
    });

    return fmt::format_to(ctx.out(), "]");
  }
};

template <typename T, std::size_t N>
struct std::tuple_size<::utils::vec_t<T, N>>
    : public std::integral_constant<std::size_t, N> { };

template <std::size_t I, typename T, std::size_t N>
struct std::tuple_element<I, ::utils::vec_t<T, N>> {
  using type = T;
};
