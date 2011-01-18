// Copyright (C) 2000 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_POOL_CT_GCD_LCM_HPP
#define BOOST_POOL_CT_GCD_LCM_HPP
/*!
  \file
  \brief Compile-Time GCD and LCM.
  \details Provides two compile-time algorithms: greatest common divisor and least common multiple.

  Selected Quotation from the C++ Standard

  5.19/1: Expressions: Constant Expressions:
  ". . . An integral constant expression can involve only literals (2.13),
  enumerators, const variables or static data members of integral or
  enumeration types initialized with constant expressions (8.5),
  non-type template parameters of integral or enumeration types,
  and sizeof expressions.
  Floating literals (2.13.3) can appear only if they are cast to integral or enumeration types.
  Only type conversions to integral or enumeration types can be used.
  In particular, except in sizeof expressions, functions, class objects, pointers, or references
  shall not be used, and assignment, increment, decrement, function-call,
  or comma operators shall not be used."
*/

#include <boost/static_assert.hpp>
#include <boost/type_traits/ice.hpp>

namespace boost {

namespace details {
namespace pool {

// Compile-time calculation of greatest common divisor and least common multiple

// ct_gcd is a compile-time algorithm that calculates the greatest common
//  divisor of two unsigned integers, using Euclid's algorithm.
//
// assumes: A != 0 && B != 0

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace details
{
	template <unsigned A, unsigned B, bool Bis0>
	struct ct_gcd_helper;
	template <unsigned A, unsigned B>
	struct ct_gcd_helper<A, B, false>
	{ //! Helper function.
		//! \tparam A compile-time unsigned integer constant.
		//! \tparam B compile-time unsigned integer constant.
		//! \tparam Bis0 parameter B == 0, or not.

		BOOST_STATIC_CONSTANT(unsigned, A_mod_B_ = A % B);
		BOOST_STATIC_CONSTANT(unsigned, value =
				(::boost::details::pool::details::ct_gcd_helper<
					B, static_cast<unsigned>(A_mod_B_),
					::boost::type_traits::ice_eq<A_mod_B_, 0>::value
					>::value) );
	};
	template <unsigned A, unsigned B>
	struct ct_gcd_helper<A, B, true>
	{
		BOOST_STATIC_CONSTANT(unsigned, value = A);
	};
} // namespace details

template <unsigned A, unsigned B>
struct ct_gcd
{ //! The greatest common divisor of A and B.
  //! \pre A != 0 && B != 0
  BOOST_STATIC_ASSERT(A != 0 && B != 0);
  BOOST_STATIC_CONSTANT(unsigned, value =
      (::boost::details::pool::details::ct_gcd_helper<A, B, false>::value) );
};

#else
//  BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION == true
// Thanks to Peter Dimov for providing this workaround!

namespace details {
template<unsigned A> struct ct_gcd2
{ //! workaround for NO_TEMPLATE_PARTIAL_SPECIALIZATION (Thanks to Peter Dimov).
  template<unsigned B>
  struct helper
  {
    BOOST_STATIC_CONSTANT(unsigned, value = ct_gcd2<B>::helper<A % B>::value);
  };
  template<>
  struct helper<0>
  {
    BOOST_STATIC_CONSTANT(unsigned, value = A);
  };
};
} // namespace details

template<unsigned A, unsigned B> struct ct_gcd
{ //! Compile-time algorithm that calculates the greatest common divisor of A and B.
  //! \pre A != 0 && B != 0.
  BOOST_STATIC_ASSERT(A != 0 && B != 0);
  enum { value = details::ct_gcd2<A>::helper<B>::value };
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template <unsigned A, unsigned B>
struct ct_lcm
{ //! Compile-time algorithm that calculates the least common multiple
  //!  of two unsigned integers.

  //! \pre A != 0 && B != 0.
  BOOST_STATIC_CONSTANT(unsigned, value =
      (A / ::boost::details::pool::ct_gcd<A, B>::value * B) );
};

} // namespace pool
} // namespace details

} // namespace boost

#endif
