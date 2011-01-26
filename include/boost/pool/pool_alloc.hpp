// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_POOL_ALLOC_HPP
#define BOOST_POOL_ALLOC_HPP

/*!
  \file
  \brief Standard Pool allocators.
  \details  provides two template types that can be used for fast and efficient memory allocation.
  These types both satisfy the Standard Allocator requirements [20.1.5]
  and the additional requirements in [20.1.5/4],
  so they can be used with Standard or user-supplied containers.\n
  For information on other pool-based interfaces, see the other pool interfaces.

  \n\n
  Both of the pool allocators above satisfy all Standard Allocator requirements,
  as laid out in the Standard [20.1.5].
  They also both satisfy the additional requirements found in [20.1.5/4];
  this permits their usage with any Standard-compliant container.

  In addition, the fast_pool_allocator also provides an additional allocation
  and an additional deallocation function:

  Symbol Table\n
  Symbol	Meaning\n
  PoolAlloc	fast_pool_allocator<T, UserAllocator>\n
  p	value of type T *\n
  \n
  Additional allocation/deallocation functions (fast_pool_allocator only)\n
  Expression	Return Type	Semantic Equivalence\n
  PoolAlloc::allocate()	T *	PoolAlloc::allocate(1)\n
  PoolAlloc::deallocate(p)	void	PoolAlloc::deallocate(p, 1)\n

The typedef user_allocator publishes the value of the UserAllocator template parameter.

Notes\n

If the allocation functions run out of memory, they will throw std::bad_alloc.

The underlying Pool type used by the allocators is accessible through the Singleton Pool Interface.
The identifying tag used for pool_allocator is pool_allocator_tag,
and the tag used for fast_pool_allocator is fast_pool_allocator_tag.
All template parameters of the allocators (including implementation-specific ones)
determine the type of the underlying Pool,
with the exception of the first parameter T, whose size is used instead.

Since the size of T is used to determine the type of the underlying Pool,
each allocator for different types of the same size will share the same underlying pool.
The tag class prevents pools from being shared between pool_allocator and fast_pool_allocator.
For example, on a system where
sizeof(int) == sizeof(void *), pool_allocator<int> and pool_allocator<void *>
will both allocate/deallocate from/to the same pool.

If there is only one thread running before main() starts and after main() ends,
then both allocators are completely thread-safe.\n

Compiler and STL Notes\n

A number of common STL libraries contain bugs in their using of allocators.
Specifically, they pass null pointers to the deallocate function,
which is explicitly forbidden by the Standard [20.1.5 Table 32].
PoolAlloc will work around these libraries if it detects them;
currently, workarounds are in place for:\n

Borland C++ (Builder and command-line compiler) with default (RogueWave) library, ver. 5 and earlier\n
STLport (with any compiler), ver. 4.0 and earlier\n

*/

// std::numeric_limits
#include <boost/limits.hpp>
// new, std::bad_alloc
#include <new>

#include <boost/throw_exception.hpp>
#include <boost/pool/poolfwd.hpp>

// boost::singleton_pool
#include <boost/pool/singleton_pool.hpp>

#include <boost/detail/workaround.hpp>

// The following code will be put into Boost.Config in a later revision
#if defined(_RWSTD_VER) || defined(__SGI_STL_PORT) || \
    BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x582))
 #define BOOST_NO_PROPER_STL_DEALLOCATE
#endif

namespace boost {

 //! Tag to identify pool_allocator when used as template parameter.
 struct pool_allocator_tag
{
};

template <typename T, //! tparam T type of object to allocate/deallocate.
    typename UserAllocator, //!< Defines the method that the underlying Pool will use to allocate memory from the system. See User Allocators for details.
    typename Mutex, //!< Allows the user to determine the type of synchronization to be used on the underlying singleton pool. See the extensions to the public interface of singleton pool for more information.
    unsigned NextSize, //!< The value of this parameter is passed to the underlying Pool when it is created.
    unsigned MaxSize> //!< Limit on the maximum size used.
class pool_allocator
{ //! Allocate a pool of memory.
  public:
    typedef T value_type;
    typedef UserAllocator user_allocator;
    typedef Mutex mutex; //!< typedef mutex publishs the value of the template parameter Mutex.
    //!> BOOST_STATIC_CONSTANT static const value next_size publishes the values of the template parameter NextSize.
     BOOST_STATIC_CONSTANT(unsigned, next_size = NextSize);

    typedef value_type * pointer;
    typedef const value_type * const_pointer;
    typedef value_type & reference;
    typedef const value_type & const_reference;
    typedef typename pool<UserAllocator>::size_type size_type;
    typedef typename pool<UserAllocator>::difference_type difference_type;

    template <typename U> //!\tparam U ???
    //! TODO explanation of use needed.
    struct rebind
    { //
      typedef pool_allocator<U, UserAllocator, Mutex, NextSize, MaxSize> other;
    };

  public:
    pool_allocator()
    { //! Construction of default singleton_pool IFF an
      //! instance of this allocator is constructed during global initialization.
      // Required to ensure construction of singleton_pool IFF an
      // instance of this allocator is constructed during global
      // initialization. See ticket #2359 for a complete explanation
      // ( http://svn.boost.org/trac/boost/ticket/2359 )
      singleton_pool<pool_allocator_tag, sizeof(T), UserAllocator, Mutex,
                     NextSize, MaxSize>::is_from(0);
    }

    // default copy constructor.

    // default assignment operator.

    // not explicit, mimicking std::allocator [20.4.1]
    template <typename U>
    pool_allocator(const pool_allocator<U, UserAllocator, Mutex, NextSize, MaxSize> &)
    { //! Construction of singleton_pool using template U.
      // Required to ensure construction of singleton_pool IFF an
      // instance of this allocator is constructed during global
      // initialization. See ticket #2359 for a complete explaination
      // ( http://svn.boost.org/trac/boost/ticket/2359 )
      singleton_pool<pool_allocator_tag, sizeof(T), UserAllocator, Mutex,
                     NextSize, MaxSize>::is_from(0);
    }

    // default destructor

    static pointer address(reference r)
    { return &r; }
    static const_pointer address(const_reference s)
    { return &s; }
    static size_type max_size()
    { return (std::numeric_limits<size_type>::max)(); }
    static void construct(const pointer ptr, const value_type & t)
    { new (ptr) T(t); }
    static void destroy(const pointer ptr)
    {
      ptr->~T();
      (void) ptr; // avoid unused variable warning.
    }

    bool operator==(const pool_allocator &) const
    { return true; }
    bool operator!=(const pool_allocator &) const
    { return false; }

    static pointer allocate(const size_type n)
    {
      const pointer ret = static_cast<pointer>(
          singleton_pool<pool_allocator_tag, sizeof(T), UserAllocator, Mutex,
              NextSize, MaxSize>::ordered_malloc(n) );
      if (ret == 0)
        boost::throw_exception(std::bad_alloc());
      return ret;
    }
    static pointer allocate(const size_type n, const void * const)
    { return allocate(n); }
    static void deallocate(const pointer ptr, const size_type n)
    {
#ifdef BOOST_NO_PROPER_STL_DEALLOCATE
      if (ptr == 0 || n == 0)
        return;
#endif
      singleton_pool<pool_allocator_tag, sizeof(T), UserAllocator, Mutex,
          NextSize, MaxSize>::ordered_free(ptr, n);
    }
};

//! pool_allocator Pool memory allocator.
template<
    typename UserAllocator,
    typename Mutex,
    unsigned NextSize,
    unsigned MaxSize>
class pool_allocator<void, UserAllocator, Mutex, NextSize, MaxSize>
{
public:
    typedef void*       pointer;
    typedef const void* const_pointer;
    typedef void        value_type;
    //! Need explanation of rebind. TODO.
    template <class U> struct rebind
    {  //! Rebind.
       typedef pool_allocator<U, UserAllocator, Mutex, NextSize, MaxSize> other;
    };
};

//! Tag to identify pool_allocator when used as template parameter.
struct fast_pool_allocator_tag
{
};

 /*! Fast Pool memory allocator.

  pool_allocator is a more general-purpose solution, geared towards
  efficiently servicing requests for any number of contiguous chunks.
  fast_pool_allocator is also a general-purpose solution,
  but is geared towards efficiently servicing requests for one chunk at a time;
  it will work for contiguous chunks, but not as well as pool_allocator.
  If you are seriously concerned about performance,
  use fast_pool_allocator when dealing with containers such as std::list,
  and use pool_allocator when dealing with containers such as std::vector.
*/
template <typename T,
    typename UserAllocator,
    typename Mutex,
    unsigned NextSize,
    unsigned MaxSize>
class fast_pool_allocator
{
  public:
    typedef T value_type;
    typedef UserAllocator user_allocator;
    typedef Mutex mutex;
    BOOST_STATIC_CONSTANT(unsigned, next_size = NextSize);

    typedef value_type * pointer;
    typedef const value_type * const_pointer;
    typedef value_type & reference;
    typedef const value_type & const_reference;
    typedef typename pool<UserAllocator>::size_type size_type;
    typedef typename pool<UserAllocator>::difference_type difference_type;

    //! ???  TODO rebind description needed.
    template <typename U>
    struct rebind
    {
      typedef fast_pool_allocator<U, UserAllocator, Mutex, NextSize, MaxSize> other;
    };

  public:
    fast_pool_allocator()
    {
      // Required to ensure construction of singleton_pool IFF an
      // instace of this allocator is constructed during global
      // initialization. See ticket #2359 for a complete explaination
      // ( http://svn.boost.org/trac/boost/ticket/2359 )
      singleton_pool<fast_pool_allocator_tag, sizeof(T),
                     UserAllocator, Mutex, NextSize, MaxSize>::is_from(0);
    }

    // Default copy constructor used.

    // Default assignment operator used.

    // Not explicit, mimicking std::allocator [20.4.1]
    template <typename U>
    fast_pool_allocator(
        const fast_pool_allocator<U, UserAllocator, Mutex, NextSize, MaxSize> &)
    {
      // Required to ensure construction of singleton_pool IFF an
      // instance of this allocator is constructed during global
      // initialization. See ticket #2359 for a complete explaination
      // ( http://svn.boost.org/trac/boost/ticket/2359 )
      singleton_pool<fast_pool_allocator_tag, sizeof(T),
                     UserAllocator, Mutex, NextSize, MaxSize>::is_from(0);
    }

    // Default destructor used.

    static pointer address(reference r)
    { return &r; }
    static const_pointer address(const_reference s)
    { return &s; }
    static size_type max_size()
    { return (std::numeric_limits<size_type>::max)(); }
    void construct(const pointer ptr, const value_type & t)
    { new (ptr) T(t); }
    void destroy(const pointer ptr)
    {
      ptr->~T();
      (void) ptr; // Avoid unused variable warning.
    }

    bool operator==(const fast_pool_allocator &) const
    { return true; }
    bool operator!=(const fast_pool_allocator &) const
    { return false; }

    static pointer allocate(const size_type n)
    {
      const pointer ret = (n == 1) ?
          static_cast<pointer>(
              (singleton_pool<fast_pool_allocator_tag, sizeof(T),
                  UserAllocator, Mutex, NextSize, MaxSize>::malloc)() ) :
          static_cast<pointer>(
              singleton_pool<fast_pool_allocator_tag, sizeof(T),
                  UserAllocator, Mutex, NextSize, MaxSize>::ordered_malloc(n) );
      if (ret == 0)
        boost::throw_exception(std::bad_alloc());
      return ret;
    }
    static pointer allocate(const size_type n, const void * const)
    { return allocate(n); }
    static pointer allocate()
    {
      const pointer ret = static_cast<pointer>(
          (singleton_pool<fast_pool_allocator_tag, sizeof(T),
              UserAllocator, Mutex, NextSize, MaxSize>::malloc)() );
      if (ret == 0)
        boost::throw_exception(std::bad_alloc());
      return ret;
    }
    static void deallocate(const pointer ptr, const size_type n)
    {
#ifdef BOOST_NO_PROPER_STL_DEALLOCATE
      if (ptr == 0 || n == 0)
        return;
#endif
      if (n == 1)
        (singleton_pool<fast_pool_allocator_tag, sizeof(T),
            UserAllocator, Mutex, NextSize, MaxSize>::free)(ptr);
      else
        (singleton_pool<fast_pool_allocator_tag, sizeof(T),
            UserAllocator, Mutex, NextSize, MaxSize>::free)(ptr, n);
    }
    static void deallocate(const pointer ptr)
    {
      (singleton_pool<fast_pool_allocator_tag, sizeof(T),
          UserAllocator, Mutex, NextSize, MaxSize>::free)(ptr);
    }
};

//!  Fast pool memory allocator.
template<
    typename UserAllocator, //!< Defines the method that the underlying Pool will use to allocate memory from the system. See User Allocators for details.
    typename Mutex, //!< Allows the user to determine the type of synchronization to be used on the underlying singleton pool. See the extensions to the public interface of singleton pool for more information.
    unsigned NextSize,  //!< The value of this parameter is passed to the underlying Pool when it is created.
    unsigned MaxSize> //!< Limit on the maximum size used.
class fast_pool_allocator<void, UserAllocator, Mutex, NextSize, MaxSize>
{
public:
    typedef void*       pointer;
    typedef const void* const_pointer;
    typedef void        value_type;
    //! need explanation of rebind TODO.
    template <class U> struct rebind {
        typedef fast_pool_allocator<U, UserAllocator, Mutex, NextSize, MaxSize> other;
    };
};

} // namespace boost

#endif
