// Copyright (C) 2000 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_POOL_GCD_LCM_HPP
#define BOOST_POOL_GCD_LCM_HPP

/*!
  \file
  \brief GCD and LCM two generic integer algorithms: greatest common divisor and least common multiple..
*/

namespace boost {

namespace details {
namespace pool {

// Greatest common divisor and least common multiple.

// gcd is an algorithm that calculates the greatest common divisor of two
//  integers, using Euclid's algorithm.
//
// Pre: A > 0 && B > 0
// Recommended: A > B
template <typename Integer>
Integer gcd(Integer A, Integer B)
{ //! algorithm that calculates the greatest common divisor of two
  //!  integers, using Euclid's algorithm.
  //! For faster results, ensure A > B.
  //! \pre A != 0 && B != 0.
  do
  {
    const Integer tmp(B);
    B = A % B;
    A = tmp;
  } while (B != 0);

  return A;
}

//
// lcm is an algorithm that calculates the least common multiple of two integers.
//
// Pre: A > 0 && B > 0
// Recommended: A > B
template <typename Integer>
Integer lcm(const Integer & A, const Integer & B)
{ //! Algorithm that calculates the least common multiple of two integers.
  //! For faster results, ensure A > B.
  //! \pre A != 0 && B != 0.
  Integer ret = A;
  ret /= gcd(A, B);
  ret *= B;
  return ret;
}

} // namespace pool
} // namespace details

} // namespace boost

#endif
