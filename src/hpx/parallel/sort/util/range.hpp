//----------------------------------------------------------------------------
/// @file range.hpp
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
#ifndef __SORT_UTIL_RANGE_HPP
#define __SORT_UTIL_RANGE_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <cassert>
#include <hpx/parallel/sort/tools/util_iterator.hpp>
#include <hpx/parallel/sort/tools/algorithm.hpp>

#ifndef __DEBUG_SORT
#define __DEBUG_SORT 0
#endif
namespace hpx
{
namespace parallel
{
namespace _sort
{
namespace util
{
///---------------------------------------------------------------------------
/// @struct range
/// @brief this represent a range between two iterators
/// @tparam iter_t type of paramenters of the range
/// @remarks
//----------------------------------------------------------------------------
template <class iter_t>
struct range
{   typedef typename tools::iter_value<iter_t>::type value_t ;
	//------------------------ variables -------------------
    iter_t first , last ;

    //------------------------- functions ------------------
    range ( void){};
    range ( iter_t frs, iter_t lst):first ( frs),last (lst){};

    bool 	empty 		( void ) const { return (first == last );};
    bool 	not_empty	( void ) const { return (first != last );};
    bool   	valid 		( void ) const { return ((last-first)>= 0 ); };
    size_t 	size		( void ) const { return (last-first);}
};
//
//-----------------------------------------------------------------------------
//  function : concat
/// @brief concatebate two contiguous ranges
/// @tparam value_t : class of the object to create
/// @param [in] it1 : first range
/// @param [in] it2 : second range
/// @return  range resulting of the concatenation
//-----------------------------------------------------------------------------
template <class iter_t>
range<iter_t> concat ( range<iter_t> it1 , range <iter_t> it2 )
{	//--------------------------- begin -------------------------------------
#if __DEBUG_SORT != 0
	assert ( it1.last == it2.first ) ;
#endif
	return range<iter_t> ( it1.first , it2.last ) ;
};
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
//  function : destroy_object
/// @brief destroy an object in the memory specified by Ptr
/// @tparam value_t : class of the object to create
/// @param [in] Ptr : pointer to the object to destroy
//-----------------------------------------------------------------------------
template <class value_t >
inline void destroy_object ( value_t *Ptr){   Ptr->~value_t () ;};
//
//-----------------------------------------------------------------------------
//  function : move
/// @brief Move objets from the range src to dest
/// @tparam iter1_t : iterator to the value_t elements
/// @tparam iter2_t : iterator to the value_t elements
/// @param [in] dest : range where move the objects
/// @param [in] src : range from where move the objects
/// @return range with the objects moved and the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t , class iter2_t >
inline range<iter2_t> move ( range<iter2_t> dest, range<iter1_t> src)
{   //------------- static checking ------------------------------------------
	typedef typename range<iter1_t>::value_t type1 ;
    typedef typename range<iter2_t>::value_t type2 ;
    static_assert ( std::is_same<type1, type2>::value,
                    "Incompatible iterators\n");

    //------------------------------- begin ----------------------------------
    if ( src.size() == 0 ) return range<iter2_t>(dest.first, dest.first);
#if __DEBUG_SORT != 0
    assert ( dest.size() >= src.size() ) ;
#endif
    iter1_t it1  = src.first;
    iter2_t it2  = dest.first ;
    while ( it1 != src.last )   *(it2++) = std::move (*(it1++));
    return range<iter2_t>(dest.first, it2) ;
};
//-----------------------------------------------------------------------------
//  function : uninit_move
/// @brief Move objets from the range src creatinf them in  dest
/// @tparam iter1_t : iterator to the value_t elements
/// @tparam iter2_t : iterator to the value_t elements
/// @param [in] dest : range where move and create the objects
/// @param [in] src : range from where move the objects
/// @return range with the objects moved and the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t ,class iter2_t >
inline range<iter2_t> uninit_move ( range<iter2_t> dest, range<iter1_t> src )
{   //------------- static checking ------------------------------------------
	typedef typename range<iter1_t>::value_t type1 ;
    typedef typename range<iter2_t>::value_t type2 ;
    static_assert ( std::is_same<type1, type2>::value,
                    "Incompatible iterators\n");

    //------------------------------- begin ----------------------------------
    if ( src.size() == 0 ) return range<iter2_t>(dest.first, dest.first);
#if __DEBUG_SORT != 0
    assert ( dest.size() >= src.size() ) ;
#endif
    iter1_t it1  = src.first;
    iter2_t it2  = dest.first ;
    while ( it1 != src.last )
		construct_object ( &(*(it2++)), std::move ( * (it1++)));
    return range<iter2_t>(dest.first, it2) ;
};
//
//-----------------------------------------------------------------------------
//  function : destroy
/// @brief destroy a range of objects
/// @param [in] R : range to destroy
//-----------------------------------------------------------------------------
template <class iter_t >
inline void destroy ( range<iter_t> r)
{   //----------------- begin ---------------------------
    while (r.first != r.last)
    	destroy_object(&(*(r.first++))) ;
};
//
//-----------------------------------------------------------------------------
//  function : init
/// @brief initialize a range of objects with the object val moving across them

/// @param [in] r : range of elements not initialized
/// @param [in] val : object used for the initialization
/// @return range initialized
//-----------------------------------------------------------------------------
template <class iter_t >
inline range<iter_t> init ( range<iter_t> r,
				            typename tools::iter_value< iter_t>::type & val)
{   //----------------- begin ---------------------------
	if ( r.size() == 0 ) return r ;
	construct_object ( &(*r.first), std::move ( val) );
	iter_t it1 = r.first, it2 = r.first+1 ;
	while ( it2 != r.last)
		construct_object (&(*( it2++)), std::move ( * (it1++)));
	val = std::move ( * (r.last -1));
	return r ;
};
//
//****************************************************************************
};//    End namespace util
};//    End namespace _sort
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
//
#endif
