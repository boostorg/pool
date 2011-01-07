/* Copyright (C) 2011 Kwan Ting Chan
 * 
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/pool/detail/gcd_lcm.hpp>
#include <boost/pool/detail/ct_gcd_lcm.hpp>
#include <boost/cstdint.hpp>
#include <boost/limits.hpp>
#include <boost/math/common_factor.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/detail/lightweight_test.hpp>

#include <climits>
#include <ctime>

boost::mt19937 gen;

int main()
{
    gen.seed(static_cast<boost::uint32_t>(std::time(0)));
    boost::uniform_int<> dist(2, 32767);
    boost::variate_generator<boost::mt19937&,
        boost::uniform_int<> > die(gen, dist);

    {
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(1, 1), 1);
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(1, 1), 1);
    }

    {
        int r = die();

        BOOST_TEST_EQ(boost::details::pool::gcd<int>(1, r), 1);
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(r, 1), 1);
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(r, r), r);

        BOOST_TEST_EQ(boost::details::pool::lcm<int>(1, r), r);
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r, 1), r);
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r, r), r);
    }

    {
        int m = (std::numeric_limits<int>::max)();

        BOOST_TEST_EQ(boost::details::pool::gcd<int>(1, m), 1);
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(m, 1), 1);
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(m, m), m);

        BOOST_TEST_EQ(boost::details::pool::lcm<int>(1, m), m);
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(m, 1), m);
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(m, m), m);
    }

    {
        int r1 = die(), r2 = die();
        int m = (std::numeric_limits<int>::max)();

        BOOST_TEST_EQ(boost::details::pool::gcd<int>(r1, r2),
                      boost::details::pool::gcd<int>(r2, r1));
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(r1, r2),
                      boost::math::gcd(r1, r2));
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(r2, r1),
                      boost::math::gcd(r2, r1));
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(r1, m),
                      boost::math::gcd(r1, m));
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(m, r1),
                      boost::math::gcd(m, r1));
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(r2, m),
                      boost::math::gcd(r1, m));
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(m, r2),
                      boost::math::gcd(m, r1));
    }

    {
        int m = (std::numeric_limits<int>::max)();
        int r2, r1 = die();
        do { r2 = die(); } while((m/r1) < r2);

        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r1, r2),
                      boost::details::pool::lcm<int>(r2, r1));
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r1, r2),
                      boost::math::lcm(r1, r2));
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r2, r1),
                      boost::math::lcm(r2, r1));

        BOOST_TEST_EQ(boost::details::pool::lcm<boost::int32_t>(46340L, 46341L),
            2147441940L);
        BOOST_TEST_EQ(boost::details::pool::lcm<boost::int32_t>(46341L, 46340L),
            2147441940L);
    }

    {
        unsigned r;
        r = boost::details::pool::ct_gcd<1U, 1U>::value;
        BOOST_TEST_EQ(r, 1U);
        r = boost::details::pool::ct_lcm<1U, 1U>::value;
        BOOST_TEST_EQ(r, 1U);

        r = boost::details::pool::ct_gcd<1U, UINT_MAX>::value;
        BOOST_TEST_EQ(r, 1U);
        r = boost::details::pool::ct_gcd<UINT_MAX, 1U>::value;
        BOOST_TEST_EQ(r, 1U);
        r = boost::details::pool::ct_gcd<UINT_MAX, UINT_MAX>::value;
        BOOST_TEST_EQ(r, UINT_MAX);

        r = boost::details::pool::ct_lcm<1U, UINT_MAX>::value;
        BOOST_TEST_EQ(r, UINT_MAX);
        r = boost::details::pool::ct_lcm<UINT_MAX, 1U>::value;
        BOOST_TEST_EQ(r, UINT_MAX);
        r = boost::details::pool::ct_lcm<UINT_MAX, UINT_MAX>::value;
        BOOST_TEST_EQ(r, UINT_MAX);

        r = boost::details::pool::ct_gcd<3458U, 6916U>::value;
        BOOST_TEST_EQ(r, 3458U);
        r = boost::details::pool::ct_gcd<6916U, 3458U>::value;
        BOOST_TEST_EQ(r, 3458U);

        if(sizeof(unsigned) >= 4)
        {
            r = boost::details::pool::ct_lcm<46340U, 46341U>::value;
            BOOST_TEST_EQ(r, 2147441940U);
            r = boost::details::pool::ct_lcm<46341U, 46340U>::value;
            BOOST_TEST_EQ(r, 2147441940U);
        }
    }
}
