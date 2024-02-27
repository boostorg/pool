/* Copyright (C) 2023 Orgad Shaneh
*
* Use, modification and distribution is subject to the
* Boost Software License, Version 1.0. (See accompanying
* file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/

// Test of bug #54 (https://github.com/boostorg/pool/issues/54)

#include <boost/pool/pool.hpp>
#include <boost/limits.hpp>

int main()
{
  boost::pool<> pool(8, 32, 64);
  // On 32 next_size reaches max_size.
  // One more round to asserts that it remains 64 (max_size)
  for (int i = 0; i <= 33; ++i) {
	size_t expected = (i == 0) ? 32 : 64;
	BOOST_ASSERT(pool.get_next_size() == expected);
	void *ptr = pool.malloc();
	BOOST_ASSERT(ptr);
  }

  return 0;
}
