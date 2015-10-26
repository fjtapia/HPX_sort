//----------------------------------------------------------------------------
/// @file half_merge.hpp
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
#ifndef __SORT_UTIL_HALF_MERGE_HPP
#define __SORT_UTIL_HALF_MERGE_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <hpx/parallel/sort/tools/util_iterator.hpp>
#include <hpx/parallel/sort/tools/algorithm.hpp>
#include <hpx/parallel/sort/util/range.hpp>

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
//---------------------------------------------------------------------------
//  function : half_merge
/// @brief : Merge two buffers. The first buffer is in a separate memory
/// @param [in] dest : range where finish the two buffers merged
/// @param [in] src1 : first range to merge in a separate memory
/// @param [in] src2 : second range to merge, in the final part of the 
///                    range where deposit the final results
/// @param [in] comp : object for compare two elements of the type pointed
///                    by the iter1_t and iter2_t
/// @return : range with the two buffers merged
//---------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class compare >
inline range<iter2_t> half_merge ( 	range<iter2_t> dest, 
									range<iter1_t> src1, 
									range<iter2_t> src2, compare  comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef range<iter2_t>         				range2_t;	
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");

    //--------------------- code -------------------------------------------
#if __DEBUG_SORT != 0
    assert (( src2.first - dest.first) >= 0 and
    		 size_t ( src2.first - dest.first) == src1.size() ) ;
    assert ( dest.size() >= (src1.size() + src2.size() ) ) ;
#endif
    if ( src1.size() == 0 ) return src2 ;
    if ( src2.size() == 0 ) return move ( dest, src1) ;
    if ( not comp (*src2.first, *(src1.last-1) ) )
		return concat ( move (dest,src1), src2);

    iter1_t it1 = src1.first ;
    iter2_t it2 = src2.first;
    iter2_t itx = dest.first ;
    while ((it1 != src1.last) and (it2 != src2.last)  )
    {   *(itx++)=(not comp(*it2,*it1))?std::move(*(it1++)):std::move (*(it2++));
    };
    range2_t Rx1 ( dest.first, itx), Rx2 ( itx, dest.last) ;
    range1_t R1 ( it1, src1.last) ;
    range2_t R2 ( it2, src2.last ) ;
    
    return ( (R2.size() == 0)? concat(Rx1,move(Rx2,R1)): concat(Rx1,R2));
};
//
//---------------------------------------------------------------------------
//  function : half_merge_big
/// @brief : Merge two buffers. The first buffer is in a separate memory.
///          This is designed for big buffers, where binary search of the 
///          beginning point can save comparisons
/// @param [in] dest : range where finish the two buffers merged
/// @param [in] src1 : first range to merge in a separate memory
/// @param [in] src2 : second range to merge, in the final part of the 
///                    range where deposit the final results
/// @param [in] comp : object for compare two elements of the type pointed
///                    by the iter1_t and iter2_t
/// @return : range with the two buffers merged
//---------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class compare >
inline range<iter2_t> half_merge_big ( 	range<iter2_t> dest, 
									    range<iter1_t> src1,
									    range<iter2_t> src2, compare  comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef range<iter2_t>         				range2_t;	
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");

    //--------------------- code -------------------------------------------
#if __DEBUG_SORT != 0
    assert ( (src2.first - dest.first)  >= 0 and
             size_t (src2.first - dest.first) == src1.size() ) ;
    assert ( dest.size() >= (src1.size() + src2.size() ) ) ;
#endif
    if ( src1.size() == 0 ) return src2 ;
    if ( src2.size() == 0 ) return move ( dest, src1) ;
    if ( not comp (*src2.first, *(src1.last-1) ) )
		return ( concat ( move (dest,src1), src2));
		
    range2_t Rx1 ( dest), Rx2(dest) ;

    if ( not comp ( *src2.first, *src1.first))// src1 < src2
	{	iter1_t itx = upper_bound ( src1.first, src1.last, *src2.first, comp);
		range1_t RA1 ( src1.first, itx), RA2 ( itx, src1.last) ;
		Rx1 = move ( dest, RA1)  ;
		Rx2 = range2_t(Rx1.last, dest.last);
		Rx2 = half_merge (Rx2, RA2, src2, comp );
	}    
	else
	{	iter2_t itx = lower_bound ( src2.first, src2.last, *src1.first, comp);
		range2_t RB1 ( src2.first, itx), RB2 ( itx, src2.last);
		Rx1 = move (dest, RB1)  ;
		Rx2 = range2_t (Rx1.last, dest.last);
		Rx2 = half_merge (Rx2, src1, RB2, comp) ;
	};
	return ( concat ( Rx1,Rx2)) ;
};
/*
//---------------------------------------------------------------------------
//  function : half_merge_back
/// @brief : Merge two buffers. The first buffer is in a separate memory
/// @tparam iter1_t : iterator to the first buffer
/// @tparam iter2_t : iterator to the second buffer and the output buffer
/// @tparam compare : object to compate the elements pointed by the iterators
/// @param [in]
/// @param [in]
/// @param [in]
/// @param [in]
/// @param [in]
/// @param [in]

/// @exception
/// @return
/// @remarks
//---------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class compare >
inline iter1_t half_merge_back ( iter1_t buf1   , iter1_t end_buf1 ,
                                 iter2_t buf2   , iter2_t end_buf2 ,
                                 iter1_t buf_out, compare  comp      )
{   //---------------------------- begin ------------------------------------
    typedef typename iter_value<iter1_t>::type value1_t ;
    typedef typename iter_value<iter2_t>::type value2_t ;
    static_assert ( std::is_same<value1_t, value2_t>::value,
                   "Incompatible iterators\n");
    //--------------------- code -------------------------------------------
    size_t N1 = end_buf1 - buf1 ;
    size_t N2 = end_buf2 - buf2 ;
    iter1_t it1 = buf1 + ( N1 -1), it1_end = buf1 - 1 ;
    iter2_t it2 = buf2 + ( N2 -1), it2_end = buf2 - 1 ;

    while ((it1 != it1_end)and(it2 != it2_end) )
    {   *(buf_out--)=(comp(*it2,*it1))?std::move(*(it1--)):std::move(*(it2--));
    };
    if (it1 == it1_end)
    {   while ( it2 != it2_end)
            *(buf_out--)= std::move ( *(it2-- ));
    };
    return buf_out ;
};
*/
//
//****************************************************************************
};//    End namespace util
};//    End namespace sort
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
//
#endif
