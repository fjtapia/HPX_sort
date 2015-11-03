//----------------------------------------------------------------------------
/// @file full_merge.hpp
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
#ifndef __SORT_UTIL_FULl_MERGE_HPP
#define __SORT_UTIL_FULl_MERGE_HPP

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
namespace _sort
{
namespace util
{
using tools::iter_value ;

//
//-----------------------------------------------------------------------------
//  function : full_merge
/// @brief Merge two contiguous ranges src1 and src2 , and put the result in
///        the range dest, returning the range merged
/// @param [in] dest : range where locate the lements merged. the size of dest
///                    must be  greater or equal than the sum of the sizes of
///                    src1 and src2
/// @param [in] src1 : first range to merge
/// @param [in] src2 : second range to merge
/// @param [in] comp : comparison object
/// @return range with the elements merged and the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class iter3_t, class compare >
inline range<iter3_t> full_merge ( range<iter3_t> dest,
                                   range<iter1_t> src1,
                                   range<iter2_t> src2, compare  comp )
{   //------------------- metaprogramming ------------------------------------
    typedef typename range<iter1_t>::value_t type1 ;
    typedef typename range<iter2_t>::value_t type2 ;
    typedef typename range<iter3_t>::value_t type3 ;
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
    static_assert ( std::is_same<type3, type2>::value,
                   "Incompatible iterators\n");
    //--------------------- code -------------------------------------------
#if __DEBUG_SORT != 0
    assert ( dest.size() >= ( src1.size() + src2.size() ) ) ;
#endif
    if ( src1.size() == 0) return move (dest, src2) ;
    if ( src2.size() == 0) return move (dest, src1) ;
    if ( not comp (*src2.first, *(src1.last-1) ) )
    {	range<iter3_t> Rx1 = move ( dest, src1);
    	dest.first = Rx1.last ;
    	range<iter3_t> Rx2 = move ( dest, src2);
		return concat (Rx1, Rx2 );
    };
    iter1_t it1 = src1.first ;
    iter2_t it2 = src2.first ;
    iter3_t it3 = dest.first ;
    while ((it1 != src1.last) and (it2 != src2.last) )
    {   *(it3++) = (not comp(*it2,*it1))?  std::move(*(it1++))
                                          : std::move (*(it2++)) ;
    };
    range<iter3_t> Rx1 (dest.first, it3 ) , Rx2 ( it3, dest.last ) ;
    range<iter1_t> R1 ( it1, src1.last ) ;
    range<iter2_t> R2 ( it2, src2.last ) ;

    return concat (Rx1,(R1.size()== 0)? move(Rx2, R2):move(Rx2, R1));
};

//-----------------------------------------------------------------------------
//  function : uninit_full_merge
/// @brief Merge two contiguous ranges src1 and src2 , and create and move the
///        result in the range dest, returning the range merged
/// @param [in] dest : range where locate the lements merged. the size of dest
///                    must be  greater or equal than the sum of the sizes of
///                    src1 and src2
/// @param [in] src1 : first range to merge
/// @param [in] src2 : second range to merge
/// @param [in] comp : comparison object
/// @return range with the elements merged and the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class iter3_t, class compare >
inline range<iter3_t> uninit_full_merge ( range<iter3_t> dest,
                                          range<iter1_t> src1,
                                          range<iter2_t> src2, compare  comp )
{   //------------------- metaprogramming ------------------------------------
    typedef typename range<iter1_t>::value_t type1 ;
    typedef typename range<iter2_t>::value_t type2 ;
    typedef typename range<iter3_t>::value_t type3 ;
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
    static_assert ( std::is_same<type3, type2>::value,
                   "Incompatible iterators\n");
    //--------------------- code -------------------------------------------
#if __DEBUG_SORT != 0
    assert ( dest.size() >= ( src1.size() + src2.size() ) ) ;
#endif
    if ( src1.size() == 0) return uninit_move (dest, src2) ;
    if ( src2.size() == 0) return uninit_move (dest, src1) ;
    if ( not comp (*src2.first, *(src1.last-1) ) )
    {	range<iter3_t> Rx1 = uninit_move ( dest, src1);
    	dest.first = Rx1.last ;
    	range<iter3_t> Rx2 = uninit_move ( dest, src2);
		return concat (Rx1, Rx2 );
    };
    iter1_t it1 = src1.first ;
    iter2_t it2 = src2.first ;
    iter3_t it3 = dest.first ;
    while ((it1 != src1.last) and (it2 != src2.last) )
    {	construct_object (&(*(it3++)),(not comp(*it2,*it1)) ?
    		                          std::move(*(it1++)):
                                      std::move (*(it2++))) ;
    };

    range<iter3_t> Rx1 (dest.first, it3 ) , Rx2 ( it3, dest.last ) ;
    range<iter1_t> R1 ( it1, src1.last ) ;
    range<iter2_t> R2 ( it2, src2.last ) ;

    return concat (Rx1,(R1.size()== 0)? uninit_move(Rx2, R2)
    		                           :uninit_move(Rx2, R1));

};
//
//-----------------------------------------------------------------------------
//  function : full_merge_big
/// @brief Merge two contiguous ranges src1 and src2 , and put the result in
///        the range dest, returning the range merged. This is useful for big
///        ranges where the binary search of the positions save comparisons
/// @param [in] dest : range where locate the lements merged. the size of dest
///                    must be  greater or equal than the sum of the sizes of
///                    src1 and src2
/// @param [in] src1 : first range to merge
/// @param [in] src2 : second range to merge
/// @param [in] comp : comparison object
/// @return range with the elements merged and the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class iter3_t, class compare >
inline range<iter3_t> full_merge_big ( 	range<iter3_t> dest,
										range<iter1_t> src1,
										range<iter2_t> src2, compare  comp )
{   //------------------- metaprogramming ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef range<iter2_t>         				range2_t;
	typedef range<iter3_t>         				range3_t;
    typedef typename range<iter1_t>::value_t type1 ;
    typedef typename range<iter2_t>::value_t type2 ;
    typedef typename range<iter3_t>::value_t type3 ;

    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
    static_assert ( std::is_same<type3, type2>::value,
                   "Incompatible iterators\n");
    //--------------------- code -------------------------------------------
#if __DEBUG_SORT != 0
    assert ( dest.size() >= ( src1.size() + src2.size() ) ) ;
#endif
    if ( src1.size() == 0) return move (dest, src2) ;
    if ( src2.size() == 0) return move (dest, src1) ;
    if ( not comp (*src2.first, *(src1.last-1) ) )
    {	range<iter3_t> Rx1 = move ( dest, src1);
    	dest.first = Rx1.last ;
    	range<iter3_t> Rx2 = move ( dest, src2);
		return concat (Rx1, Rx2 );
    };

    range3_t Rx1 ( dest), Rx2(dest) ;
    if ( comp ( *src2.first, *src1.first))
    {	iter2_t itx = lower_bound ( src2.first, src2.last, *src1.first, comp);
		range2_t RB1 ( src2.first, itx), RB2 ( itx, src2.last);
		Rx1 = move (dest, RB1)  ;
		Rx2 = range3_t (Rx1.last, dest.last);
		Rx2 = full_merge (Rx2, src1, RB2, comp) ;
	}
	else
	{	iter1_t itx = upper_bound ( src1.first, src1.last, *src2.first, comp);
		range1_t RA1 ( src1.first, itx), RA2 ( itx, src1.last) ;
		Rx1 = move ( dest, RA1)  ;
		Rx2 = range3_t(Rx1.last, dest.last);
		Rx2 = full_merge (Rx2, RA2, src2, comp );
	};
	return concat ( Rx1,Rx2) ;

};
//-----------------------------------------------------------------------------
//  function : uninit_full_merge_big
/// @brief Merge two contiguous ranges src1 and src2 , and create and move the
///        result in the range dest, returning the range merged. This is useful
///        for big ranges where the binary search of the positions save
///        comparisons
/// @param [in] dest : range where locate the lements merged. the size of dest
///                    must be  greater or equal than the sum of the sizes of
///                    src1 and src2
/// @param [in] src1 : first range to merge
/// @param [in] src2 : second range to merge
/// @param [in] comp : comparison object
/// @return range with the elements merged and the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class iter3_t, class compare >
inline range<iter3_t> uninit_full_merge_big ( 	range<iter3_t> dest,
										range<iter1_t> src1,
										range<iter2_t> src2, compare  comp )
{   //------------------- metaprogramming ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef range<iter2_t>         				range2_t;
	typedef range<iter3_t>         				range3_t;
    typedef typename range<iter1_t>::value_t type1 ;
    typedef typename range<iter2_t>::value_t type2 ;
    typedef typename range<iter3_t>::value_t type3 ;

    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
    static_assert ( std::is_same<type3, type2>::value,
                   "Incompatible iterators\n");
    //--------------------- code -------------------------------------------
#if __DEBUG_SORT != 0
    assert ( dest.size() >= ( src1.size() + src2.size() ) ) ;
#endif
    if ( src1.size() == 0) return move (dest, src2) ;
    if ( src2.size() == 0) return move (dest, src1) ;
    if ( not comp (*src2.first, *(src1.last-1) ) )
    {	range<iter3_t> Rx1 = uninit_move ( dest, src1);
    	dest.first = Rx1.last ;
    	range<iter3_t> Rx2 = uninit_move ( dest, src2);
		return concat (Rx1, Rx2 );
    };

    range3_t Rx1 ( dest), Rx2(dest) ;
    if ( comp ( *src2.first, *src1.first))
    {	iter2_t itx = lower_bound ( src2.first, src2.last, *src1.first, comp);
		range2_t RB1 ( src2.first, itx), RB2 ( itx, src2.last);
		Rx1 = uninit_move (dest, RB1)  ;
		Rx2 = range3_t (Rx1.last, dest.last);
		Rx2 = uninit_full_merge (Rx2, src1, RB2, comp) ;
	}
	else
	{	iter1_t itx = upper_bound ( src1.first, src1.last, *src2.first, comp);
		range1_t RA1 ( src1.first, itx), RA2 ( itx, src1.last) ;
		Rx1 = uninit_move ( dest, RA1)  ;
		Rx2 = range3_t(Rx1.last, dest.last);
		Rx2 = uninit_full_merge (Rx2, RA2, src2, comp );
	};
	return concat ( Rx1,Rx2) ;

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
