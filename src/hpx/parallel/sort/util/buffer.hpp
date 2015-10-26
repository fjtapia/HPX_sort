//----------------------------------------------------------------------------
/// @file buffer.hpp
/// @brief
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __SORT_UTIL_BUFFER_HPP
#define __SORT_UTIL_BUFFER_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <tools/util_iterator.hpp>
#include <tools/algorithm.hpp>

namespace hpx
{
namespace parallel
{
namespace sort
{
namespace util
{
using tools::iter_value ;
//
//-----------------------------------------------------------------------------
//  function : construct_object
/// @brief create an object in the memory specified by Ptr
/// @tparam value_t : class of the object to create
/// @tparam Args : parameters for the constructor
/// @param [in] Ptr : pointer to the memory where to create the object
/// @param [in] args : arguments to the constructor
//-----------------------------------------------------------------------------
template <class value_t ,class ... Args>
inline void construct_object ( value_t *Ptr, Args && ... args)
{   (::new (static_cast<void*> (Ptr)) value_t (std::forward<Args> ( args)...));
};
//
//-----------------------------------------------------------------------------
//  function : move_buf
/// @brief Move objets
/// @tparam iter_t : iterator to the elements
/// @tparam value_t : class of the object to create
/// @param [in] itdest : iterator to the final place of the objects
/// @param [in] R : range to move
//-----------------------------------------------------------------------------
template <class iter1_t , class iter2_t >
inline iter2_t move_buf ( iter2_t it_dest, iter1_t first, iter1_t last)
{   //----------------- begin ---------------------------
    while ( first != last )   *(it_dest++) = std::move (*(first++));
    return it_dest ;
};
//
//-----------------------------------------------------------------------------
//  function : uninit_move_buf
/// @brief Move objets to uninitialized memory
/// @tparam iter_t : iterator to the elements
/// @tparam value_t : class of the object to construct
/// @param [in] Ptr : pointer to the memory where to create the object
/// @param [in] R : range to move
//-----------------------------------------------------------------------------
template <class iter_t ,
          class value_t = typename iter_value<iter_t>::type >
inline value_t * uninit_move_buf ( value_t *Ptr, iter_t first, iter_t last )
{   //----------------- begin ---------------------------
    while ( first != last )
        ::new (static_cast<void*> (Ptr++)) value_t (std::move ( *(first++)) );
    return Ptr ;
};
//
//-----------------------------------------------------------------------------
//  function : destroy_buf
/// @brief Move objets to uninitialized memory
/// @tparam iter_t : iterator to the elements
/// @tparam value_t : class of the object to construct
/// @param [in] Ptr : pointer to the memory where to construct the object
/// @param [in] R : range to move
//-----------------------------------------------------------------------------
template <class iter_t >
inline void destroy_buf ( iter_t first, iter_t last )
{   //----------------- begin ---------------------------
    typedef typename iter_value<iter_t>::type value_t ;
    while (first != last) (&(*(first++)))->~value_t () ;
};
//
//****************************************************************************
};//    End namespace util
};//    End namespace sort
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
//
#endif
