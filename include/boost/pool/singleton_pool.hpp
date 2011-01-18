// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_SINGLETON_POOL_HPP
#define BOOST_SINGLETON_POOL_HPP

/*!
  \file
  \brief Singleton_pool class allows other pool interfaces
  for types of the same size to share the same pool.

  /details singleton_pool.hpp provides a template class singleton_pool,
  which provides access to a pool as a singleton object.\n
  For information on other pool-based interfaces, see the other pool interfaces.\n

  Notes\n
  The underlying pool p referenced by the static functions
  in singleton_pool is actually declared in a way that it is:\n

  1 Thread-safe if there is only one thread running before main() begins and after main() ends
  -- all of the static functions of singleton_pool synchronize their access to p.

  2 Guaranteed to be constructed before it is used --
  thus, the simple static object in the synopsis above would actually be an incorrect implementation.
  The actual implementation to guarantee this is considerably more complicated.

  3 Note too that a different underlying pool p exists
  for each different set of template parameters,
  including implementation-specific ones.
*/

#include <boost/pool/poolfwd.hpp>

// boost::pool
#include <boost/pool/pool.hpp>
// boost::details::pool::singleton_default
#include <boost/pool/detail/singleton.hpp>
// boost::details::pool::guard
#include <boost/pool/detail/guard.hpp>

namespace boost {

 //! singleton pool class allows other pool interfaces
 //! for types of the same size to share the same pool.
 template <typename Tag, unsigned RequestedSize,
    typename UserAllocator, //!< User allocator, default = default_user_allocator_new_delete
    typename Mutex, //!< The typedef mutex  publish the values of the template parameter Mutex (default details::pool::default_mutex).
    unsigned NextSize, //!< The typedef static const value next_size publish the values of the template parameter NextSize, (default 32).
    unsigned MaxSize> //!< Maximum size.
struct singleton_pool
{
  public:
    typedef Tag tag; /*!< The Tag template parameter allows
			different unbounded sets of singleton pools to exist.
			For example, the pool allocators use two tag classes to ensure that the
			two different allocator types never share the same underlying singleton pool.
			Tag is never actually used by singleton_pool.
		*/
    typedef Mutex mutex;
    typedef UserAllocator user_allocator;
    typedef typename pool<UserAllocator>::size_type size_type;
    typedef typename pool<UserAllocator>::difference_type difference_type;

    BOOST_STATIC_CONSTANT(unsigned, requested_size = RequestedSize);
    BOOST_STATIC_CONSTANT(unsigned, next_size = NextSize);

  private:
    struct pool_type: Mutex
    { /*! Mutex  This class is the type of mutex to use to protect
       simultaneous access to the underlying Pool.
       It is exposed so that users may declare some singleton pools normally
       (i.e., with synchronization),
       but some singleton pools without synchronization
       (by specifying details::pool::null_mutex) for efficiency reasons.
      */
      pool<UserAllocator> p;
      pool_type()
      :
      p(RequestedSize, NextSize)
      { /*! Pool allocation.
       See The pair of functions size_type get_next_size() const; and void set_next_size(size_type);
       that allow users to explicitly read and write the next_size value.
       This value is the number of chunks to request from the system
       the next time that object needs to allocate system memory.

       param RequestedSize value of this parameter is passed to the underlying Pool when it is created.
       param NextSize value of this parameter is passed to the underlying Pool when it is created.

       \pre NextSize value should never be set to 0.
      */
			}
    }; //  struct pool_type: Mutex

    typedef details::pool::singleton_default<pool_type> singleton;

    singleton_pool();

  public:
    static void * malloc BOOST_PREVENT_MACRO_SUBSTITUTION()
    { //! Equivalent to SingletonPool::p.malloc(); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return (p.p.malloc)();
    }
    static void * ordered_malloc()
    {  //! Equivalent to SingletonPool::p.ordered_malloc(); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.ordered_malloc();
    }
    static void * ordered_malloc(const size_type n)
    { //! Equivalent to SingletonPool::p.ordered_malloc(n); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.ordered_malloc(n);
    }
    static bool is_from(void * const ptr)
    { //! Equivalent to SingletonPool::p.is_from(chunk); synchronized.
      //! \returns true if chunk is from SingletonPool::is_from(chunk)
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.is_from(ptr);
    }
    static void free BOOST_PREVENT_MACRO_SUBSTITUTION(void * const ptr)
    { //! Equivalent to SingletonPool::p.free(chunk); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      (p.p.free)(ptr);
    }
    static void ordered_free(void * const ptr)
    { //! Equivalent to SingletonPool::p.ordered_free(chunk); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      p.p.ordered_free(ptr);
    }
    static void free BOOST_PREVENT_MACRO_SUBSTITUTION(void * const ptr, const size_type n)
    { //! Equivalent to SingletonPool::p.free(chunk, n); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      (p.p.free)(ptr, n);
    }
    static void ordered_free(void * const ptr, const size_type n)
    { //! Equivalent to SingletonPool::p.ordered_free(chunk, n); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      p.p.ordered_free(ptr, n);
    }
    static bool release_memory()
    { //! Equivalent to SingletonPool::p.release_memory(); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.release_memory();
    }
    static bool purge_memory()
    { //! Equivalent to SingletonPool::p.purge_memory(); synchronized.
      pool_type & p = singleton::instance();
      details::pool::guard<Mutex> g(p);
      return p.p.purge_memory();
    }
}; // struct singleton_pool

} // namespace boost

#endif
