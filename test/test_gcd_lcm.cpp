/* Copyright (C) 2011 Kwan Ting Chan
 * 
 * Use, modification and distribution is subject to the 
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/pool/detail/gcd_lcm.hpp>
#include <boost/pool/detail/ct_gcd_lcm.hpp>
#include <boost/limits.hpp>
#include <boost/math/common_factor.hpp>

#include <boost/detail/lightweight_test.hpp>

#include <climits>
#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(static_cast<unsigned>(std::time(0)));

    {
        BOOST_TEST_EQ(boost::details::pool::gcd<int>(1, 1), 1);
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(1, 1), 1);
    }

    {
        int r;
        while((r = std::rand()) < 2) {}

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
        int r1, r2;
        while((r1 = std::rand()) < 2 || (r2 = std::rand()) < 2) {}
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
        int r1, r2;
        while((r1 = std::rand()) < 2) {}
        do { r2 = std::rand(); } while((m/r1) < r2);

        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r1, r2),
                      boost::details::pool::lcm<int>(r2, r1));
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r1, r2),
                      boost::math::lcm(r1, r2));
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(r2, r1),
                      boost::math::lcm(r2, r1));

        BOOST_TEST_EQ(boost::details::pool::lcm<int>(46340, 46341), 2147441940);
        BOOST_TEST_EQ(boost::details::pool::lcm<int>(46341, 46340), 2147441940);
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

        r = boost::details::pool::ct_lcm<46340U, 46341U>::value;
        BOOST_TEST_EQ(r, 2147441940U);
        r = boost::details::pool::ct_lcm<46341U, 46340U>::value;
        BOOST_TEST_EQ(r, 2147441940U);
    }
}
