#ifndef LIBMONADIC_MONADIC_ELEVATE_H
#define LIBMONADIC_MONADIC_ELEVATE_H

#include <type_traits>
#include <vector>
#include "monadic_package.h"

namespace monadic
{

auto pack(int& i) { return package{i}; }
auto pack(const int& i) { return package{i}; }

template<typename T>
auto pack(const std::vector<T>& vec)
{
  return package{vec.cbegin(), vec.cend()};
}

template<typename T>
auto pack(std::vector<T>& vec)
{
  return package{vec.begin(), vec.end()};
}


template<typename From, typename To>
struct convert_wrapper { using type = To; };

template<typename From, typename To, template<typename> typename C>
struct convert_wrapper< C<From>, To > { using type = C<To>; };

template<typename T>
class elevate
{
private:
  T obj; //T=std::vector<type> or std::optional<type> or int, etc.

public:
  elevate() = default;

  template<typename U>
  elevate(U&& u) : obj{std::forward<U>(u)} {}

  template<typename F>
  auto apply(F&& f) const -> decltype(auto)
  {
    auto p = pack(obj);
    for(auto it = p.begin(); it != p.end(); ++it)
      f(*it);

    return *this;
  }

  template<typename F>
  auto apply(F&& f) -> decltype(auto)
  {
    auto p = pack(obj);
    using value_type = typename decltype(p)::value_type;
    using f_return_t = std::result_of_t<F(value_type&)>;
    constexpr bool is_value_type_ref = std::is_reference_v<value_type>;
    constexpr bool is_return_type_ref = std::is_reference_v<f_return_t>;

    if constexpr(std::is_same_v<f_return_t, void> or (is_value_type_ref and is_return_type_ref and std::is_same_v<f_return_t, value_type>))
    {
      for(auto it = p.begin(); it != p.end(); ++it)
        f(*it);

      return *this;
    }
    else
    {
      using return_type = typename convert_wrapper<T, f_return_t>::type;

      return_type result;
      auto result_p = pack(result);

      for(auto it = p.begin(); it != p.end(); ++it)
        result_p.insert(result, f(*it));

      return elevate<decltype(result)>{result};
    }
  }

  template<typename F>
  auto operator|(F&& f) const -> decltype(auto)
  {
    return apply(std::forward<F>(f));
  }

  template<typename F>
  auto operator|(F&& f) -> decltype(auto)
  {
    return apply(std::forward<F>(f));
  }
};

elevate() -> elevate<char*>;

template<typename U>
elevate(U&&) -> elevate<std::remove_reference_t<U>>;


} //namespace monadic

#endif
