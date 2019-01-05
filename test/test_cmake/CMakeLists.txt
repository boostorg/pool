# Copyright 2018 Mike Dev
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt
#
# NOTE: This does NOT run the unit tests for Boost.Pool.
#       It only tests, if the CMakeLists.txt file in it's root works as expected

cmake_minimum_required( VERSION 3.5 )

project( BoostPool_TestCMake LANGUAGES CXX )

set( dependencies
        assert;
        config;
        core;
        integer;
        throw_exception;
        type_traits;
        static_assert;
        predef;
        winapi
)

foreach( lib IN LISTS dependencies )
    add_subdirectory( ../../../${lib} ${CMAKE_CURRENT_BINARY_DIR}/libs/${lib} )
endforeach()

add_subdirectory( ../.. ${CMAKE_CURRENT_BINARY_DIR}/libs/boost_pool )

add_executable( boost_pool_test_cmake main.cpp )
target_link_libraries( boost_pool_test_cmake Boost::pool )

