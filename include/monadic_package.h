#ifndef LIBMONADIC_MONADIC_PACKAGE_H
#define LIBMONADIC_MONADIC_PACKAGE_H

#include <memory>
#include <type_traits>

namespace monadic
{

template<typename It, typename EndIt, bool is_range>
class package
{
private:
  It m_begin_it;
  EndIt m_end_it;

public:
  using value_type = std::remove_reference_t<decltype(*m_begin_it)>;

  package(It begin_it, EndIt end_it) : m_begin_it{begin_it}, m_end_it{end_it} {}

  template<typename T>
  package(T& obj) : m_begin_it{std::addressof(obj)}, m_end_it{std::addressof(obj)+1} {}

  package() : m_begin_it{nullptr}, m_end_it{nullptr} {}

  auto begin() { return m_begin_it; }
  auto end() { return m_end_it; }
  auto cbegin() const { return m_begin_it; }
  auto cend() const { return m_end_it; }
  //std::size_t size() const { return m_end_it - m_begin_it; }
  
  template<typename U, typename Cont>
  void insert(Cont& cont, U&& val)
  {
    if constexpr(is_range)
    {
      cont.insert(cont.end(), std::forward<U>(val));
    }
    else
    {
      cont = std::forward<U>(val);
    }
  }
};

template<typename It, typename EndIt>
package(It, EndIt) -> package<It, EndIt, true>;

template<typename T>
package(T&) -> package<T*, T*, false>;

package() -> package<char*, char*, false>;

} //namespace monadic

#endif
