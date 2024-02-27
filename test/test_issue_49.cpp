/* Copyright (C) 2022 Joel Pelaez Jorge
*
* Use, modification and distribution is subject to the
* Boost Software License, Version 1.0. (See accompanying
* file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

// Test of issue #49 (https://github.com/boostorg/pool/issues/49)

#if defined (_WIN32)
# include <Windows.h>
#endif
#include <boost/pool/pool.hpp>

int main()
{
  boost::pool<> p(1024);
  return 0;
}
