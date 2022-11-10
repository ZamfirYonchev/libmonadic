#include <vector>
#include <list>
#include <string_view>
#include <complex>
#include <iostream>
#include "../include/monadic_package.h"
#include "../include/monadic_elevate.h"

template<typename T>
void print(T& pack)
{
  for(auto it = pack.begin(); it != pack.end(); ++it)
    std::cout << *it << ", ";

  std::cout << '\n';
}

template<typename T>
void mutate_and_print(T& pack)
{
  for(auto it = pack.begin(); it != pack.end(); ++it)
  {
    *it *= 2;
    std::cout << *it << ", ";
  }

  std::cout << '\n';
}

struct S {};

int integer {42};
auto pack(S&) { return monadic::package(integer); }

int main()
{
  //monadic m0;
  //monadic m1 {};

  std::vector vec {1, 2, 3};
  std::list lst {4, 5, 6, 7, 8};
  std::string_view sv {"Hello world"};
  int x = 42;
  std::complex c {1.23, 4.56};
  const std::vector<int> cvec {1, 2, 3};
  const std::list<int> clst {4, 5, 6, 7, 8};
  const std::string_view csv {"Hello world"};
  const int cx = 42;
  const std::complex<double> cc {1.23, 4.56};

  monadic::package p0;
  monadic::package p1 {vec.begin(), vec.end() };
  monadic::package p2 {lst.begin(), lst.end() };
  monadic::package p3 {sv.begin(), sv.end()};
  monadic::package p4 {sv};
  monadic::package p5 {x};
  monadic::package p6 {c};
  monadic::package p7;
  monadic::package p8 {cvec.begin(), cvec.end()};
  monadic::package p9 {clst.begin(), clst.end()};
  monadic::package p10 {csv.begin(), csv.end()};
  monadic::package p11 {csv};
  monadic::package p12 {cx};
  monadic::package p13 {cc};

  //print(p0);
  print(p1);
  print(p2);
  print(p3);
  print(p4);
  print(p5);
  print(p6);
  //print(p7);
  print(p8);
  print(p9);
  print(p10);
  print(p11);
  print(p12);
  print(p13);
  //mutate_and_print(p0);
  mutate_and_print(p1);
  mutate_and_print(p2);
  //mutate_and_print(p3);
  //mutate_and_print(p4);
  mutate_and_print(p5);
  mutate_and_print(p6);
  //mutate_and_print(p7);
  //mutate_and_print(p8);
  //mutate_and_print(p9);
  //mutate_and_print(p10);
  //mutate_and_print(p11);
  //mutate_and_print(p12);
  //mutate_and_print(p13);

  S s;

  monadic::elevate m0 {vec};
  monadic::elevate m1 {s};

  m0 | [](int i){ return i+0.1; }
     | [](const auto& el){ std::cout << el << ", "; return el*2; }
     | [](const auto& el){ std::cout << el << ", "; }
  ;
  
  std::cout << '\n';
}
