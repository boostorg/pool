// Copyright (C) 2000, 2001 Stephen Cleary
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_OBJECT_POOL_HPP
#define BOOST_OBJECT_POOL_HPP
/*!
\file
\brief  provides a template type that can be used for fast and efficient memory allocation.
It also provides automatic destruction of non-deallocated objects.\n
For information on other pool-based interfaces, see the other pool interfaces.

UserAllocator\n
Defines the allocator that the underlying Pool will use to allocate memory from the system.
See User Allocators for details.

\details

(&t)->~ObjectPool() Destructs the ObjectPool.\n
~ElementType() is called for each allocated ElementType that has not been deallocated. O(N).\n\n

Extensions to Public Interface\n

Whenever an object of type ObjectPool needs memory from the system,
it will request it from its UserAllocator template parameter.
The amount requested is determined using a doubling algorithm;
that is, each time more system memory is allocated,
the amount of system memory requested is doubled.
Users may control the doubling algorithm by using the following extensions.\n

Additional constructor parameter\n

Users may pass an additional constructor parameter to ObjectPool.
This parameter is of type size_type, and is the number of chunks
to request from the system the first time that
object needs to allocate system memory.
The default is 32. This parameter may not be 0.\n

*/

#include <boost/pool/poolfwd.hpp>

// boost::pool
#include <boost/pool/pool.hpp>

// The following code will be put into Boost.Config in a later revision
#if defined(BOOST_MSVC) || defined(__KCC)
# define BOOST_NO_TEMPLATE_CV_REF_OVERLOADS
#endif

// The following code might be put into some Boost.Config header in a later revision
#ifdef __BORLANDC__
# pragma option push -w-inl
#endif

// There are a few places in this file where the expression "this->m" is used.
// This expression is used to force instantiation-time name lookup, which I am
//   informed is required for strict Standard compliance.  It's only necessary
//   if "m" is a member of a base class that is dependent on a template
//   parameter.
// Thanks to Jens Maurer for pointing this out!

namespace boost {

/*! \tparam T type of object to allocate/deallocate.
    \pre T must have a non-throwing destructor.
*/
template <typename T, typename UserAllocator>
class object_pool: protected pool<UserAllocator>
{ //!
  public:
    typedef T element_type; //!< ElementType
    typedef UserAllocator user_allocator; //!<
    typedef typename pool<UserAllocator>::size_type size_type; //!<   pool<UserAllocator>::size_type
    typedef typename pool<UserAllocator>::difference_type difference_type; //!< pool<UserAllocator>::difference_type

  protected:
    pool<UserAllocator> & store()
    { //! \return *this
      return *this;
    }
    const pool<UserAllocator> & store() const
    { //! \return *this
      return *this;
    }

    // for the sake of code readability :)
    static void * & nextof(void * const ptr)
    { //! \returns dereferenced ptr (for the sake of code readability :)
      return *(static_cast<void **>(ptr));
    }

  public:
    explicit object_pool(const size_type next_size = 32, const size_type max_size = 0)
    :
    pool<UserAllocator>(sizeof(T), next_size, max_size)
    { //! Constructs a new (empty by default) ObjectPool.
      //! \param next_size number of chunks to request from the system the next time that object needs to allocate system memory (default 32).
      //! \pre next_size != 0.
      //! \param max_size maximum size of block.
    }

    ~object_pool();

    // Returns 0 if out-of-memory.
    element_type * malloc BOOST_PREVENT_MACRO_SUBSTITUTION()
    { //! Allocates memory that can hold one object of type ElementType.
      //! If out of memory, returns 0. Amortized O(1).
      return static_cast<element_type *>(store().ordered_malloc());
    }
    void free BOOST_PREVENT_MACRO_SUBSTITUTION(element_type * const chunk)
    { //! De-Allocates memory that holds a chunk of type ElementType.
      //!  Note that p may not be 0.\n
      //! Note that the destructor for p is not called. O(N).
      store().ordered_free(chunk);
    }
    bool is_from(element_type * const chunk) const
    { /*! \returns true  if p was allocated from u or
      may be returned as the result of a future allocation from u.\n
      Returns false if p was allocated from some other pool or
      may be returned as the result of a future allocation from some other pool.
      Otherwise, the return value is meaningless.
      \note This function may NOT be used to reliably test random pointer values!
    */
      return store().is_from(chunk);
    }

    element_type * construct()
    { //! Constructs a new
      element_type * const ret = (malloc)();
      if (ret == 0)
        return ret;
      try { new (ret) element_type(); }
      catch (...) { (free)(ret); throw; }
      return ret;
    }

// Include automatically-generated file for family of template construct() functions.
// Copy .inc renamed .ipp to conform to Doxygen include filename expectations, PAB 12 Jan 11.
// But still get Doxygen warning:
// I:/boost-sandbox/guild/pool/boost/pool/object_pool.hpp:82:
// Warning: include file boost/pool/detail/pool_construct.ipp
// not found, perhaps you forgot to add its directory to INCLUDE_PATH?
// But the file IS found and referenced OK, but cannot view code.
// This seems because not at the head of the file
// But if moved this up, Doxygen is happy, but of course it won't compile,
// because the many constructors *must* go here.

#ifndef BOOST_NO_TEMPLATE_CV_REF_OVERLOADS
#   include <boost/pool/detail/pool_construct.ipp>
#else
#   include <boost/pool/detail/pool_construct_simple.ipp>
#endif

    void destroy(element_type * const chunk)
    { //! destroy a chunk. (== p->~ElementType(); t.free(p);)
      //! \pre p must have been previously allocated from t.
      chunk->~T();
      (free)(chunk);
    }

    size_type get_next_size() const
    { //! \returns next_size.
      return store().get_next_size();
    }
    void set_next_size(const size_type x)
    { //! Set new next_size.
      //! \param x wanted next_size (!= 0).
      store().set_next_size(x);
    }
};

template <typename T, typename UserAllocator>
object_pool<T, UserAllocator>::~object_pool()
{
  // handle trivial case of invalid list.
  if (!this->list.valid())
    return;

  details::PODptr<size_type> iter = this->list;
  details::PODptr<size_type> next = iter;

  // Start 'freed_iter' at beginning of free list
  void * freed_iter = this->first;

  const size_type partition_size = this->alloc_size();

  do
  {
    // increment next
    next = next.next();

    // delete all contained objects that aren't freed.

    // Iterate 'i' through all chunks in the memory block.
    for (char * i = iter.begin(); i != iter.end(); i += partition_size)
    {
      // If this chunk is free,
      if (i == freed_iter)
      {
        // Increment freed_iter to point to next in free list.
        freed_iter = nextof(freed_iter);

        // Continue searching chunks in the memory block.
        continue;
      }

      // This chunk is not free (allocated), so call its destructor,
      static_cast<T *>(static_cast<void *>(i))->~T();
      // and continue searching chunks in the memory block.
    }

    // free storage.
    (UserAllocator::free)(iter.begin());

    // increment iter.
    iter = next;
  } while (iter.valid());

  // Make the block list empty so that the inherited destructor doesn't try to
  // free it again.
  this->list.invalidate();
}

} // namespace boost

// The following code might be put into some Boost.Config header in a later revision
#ifdef __BORLANDC__
# pragma option pop
#endif

#endif
