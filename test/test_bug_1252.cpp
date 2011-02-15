/* Copyright (C) 2011 Kwan Ting Chan
 * Based from bug report submitted by Xiaohan Wang
 * 
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

// Test of bug #3349 (https://svn.boost.org/trac/boost/ticket/3349)

#include <boost/pool/pool.hpp>

struct limited_allocator_new_delete
{
  typedef std::size_t size_type; 
  typedef std::ptrdiff_t difference_type; 

  static char * malloc BOOST_PREVENT_MACRO_SUBSTITUTION(const size_type bytes)
  { 
     if(bytes > 2000)
        return 0;
     return new (std::nothrow) char[bytes];
  }
  static void free BOOST_PREVENT_MACRO_SUBSTITUTION(char * const block)
  { 
     delete [] block;
  }
};


int main() 
{
  boost::pool<limited_allocator_new_delete> po(1501);
  void* p = po.malloc();
  return p == 0 ? 1 : 0;
}
