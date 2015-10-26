//----------------------------------------------------------------------------
/// @file merge_four.hpp
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
#ifndef __SORT_UTIL_MERGE_FOUR_HPP
#define __SORT_UTIL_MERGE_FOUR_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <hpx/parallel/sort/tools/util_iterator.hpp>
#include <hpx/parallel/sort/tools/algorithm.hpp>
#include <hpx/parallel/sort/util/full_merge.hpp>

namespace hpx
{
namespace parallel
{
namespace sort
{
namespace util
{

//
//############################################################################
//                                                                          ##
//                       F U S I O N     O F                                ##
//                                                                          ##
//              F O U R     E L E M E N T S    R A N G E                    ##
//                                                                          ##
//############################################################################
//

//-----------------------------------------------------------------------------
//  function : less_range
/// @brief Compare the elements pointed by it1 and it2, and if they
///        are equals, compare their position
/// @tparam compare : object to compate the elements pointed by iter1_t
/// @param [in] it1 : iterator to the first element
/// @param [in] pos1 : position of the object pointed by it1
/// @param [in] it2 : iterator to the second element
/// @param [in] pos2 : position of the element ointed by it2
/// @param [in] comp : comparison object
/// @return result of the comparison
//-----------------------------------------------------------------------------
template < class iter_t,
           class compare = std::less< typename iter_value<iter_t>::type> >
inline bool less_range ( iter_t it1, uint32_t pos1 ,
                         iter_t it2, uint32_t pos2 ,compare comp =compare() )
{   //----------------------------- begin ------------------------------------
    if ( comp (*it1, *it2 )) return true ;
    if ( pos2 < pos1) return false ;
    return not ( comp (*it2, *it1 )) ;
};

//-----------------------------------------------------------------------------
//  function : full_merge4
/// @brief Merge four ranges
/// @param [in] dest: range where move the elements merged. Their size must be 
///                   greater or equal than the sum of the sizes of the ranges
///                   in the array R
/// @param [in] R : array of ranges to merge
/// @param [in] NR : number of ranges in R
/// @param [in] comp : comparison object
/// @return range with all the elements move with the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class compare >
range<iter1_t> full_merge4 ( range<iter1_t> dest, range<iter2_t> R[4],
                             uint32_t NR ,compare comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	//typedef range<iter2_t>         				range2_t;
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
	//----------------------------- begin ------------------------------------
	size_t Ndest = 	0 ;
	uint32_t i = 0 ;
	while (i < NR )
	{ 	if (R[i].size() != 0 ) Ndest += R[i++].size() ;
		else
		{	for ( uint32_t k = i+1 ; k < NR ; ++k)
				R[k-1] = R[k] ;
			--NR ;
		};
	};
#if __DEBUG_SORT != 0
	assert ( dest.size() >= Ndest ) ;
#endif
	if ( NR == 0 ) return range1_t ( dest.first, dest.first) ;
    if ( NR == 1 ) return move ( dest, R[0]);
    if ( NR == 2 ) return full_merge ( dest, R[0],R[1], comp);

    //------------------------------------------------------------------------
    // Initial sort
    //------------------------------------------------------------------------
    uint32_t Pos[4]={0,1,2,3}, NPos =NR  ;

    if (less_range (R[Pos[1]].first, Pos[1], R[Pos[0]].first,Pos[0],comp))
        std::swap (Pos[0], Pos[1]);
    if (less_range (R[Pos[2]].first, Pos[2], R[Pos[1]].first,Pos[1],comp))
        std::swap (Pos[1], Pos[2]);
    if ( NPos == 4 and
        less_range (R[Pos[3]].first, Pos[3],R[Pos[2]].first, Pos[2], comp) )
        std::swap ( Pos[3], Pos[2]);

    if (less_range (R[Pos[1]].first , Pos[1],R[Pos[0]].first, Pos[0], comp))
        std::swap (Pos[0], Pos[1]);
    if (NPos==4 and
        less_range (R[Pos[2]].first, Pos[2], R[Pos[1]].first,Pos[1],comp))
        std::swap(Pos[1],Pos[2]);

    if (NPos == 4 and
        less_range (R [Pos[1]].first, Pos[1],R[Pos[0]].first, Pos[0], comp))
        std::swap ( Pos[0], Pos[1]);
	
	iter1_t it_dest = dest.first ;
    while ( NPos > 2)
    {   *(it_dest++) = std::move ( *(R[Pos[0]].first++));
        if (R[Pos[0]].size() == 0   )
        {   Pos[0] = Pos[1];
            Pos[1] = Pos[2];
            Pos[2] = Pos[3];
            --NPos ;
        }
        else
        {   if (less_range(R[Pos[1]].first,Pos[1],R[Pos[0]].first,Pos[0],comp))
            {   std::swap ( Pos[0], Pos[1]);
                if (less_range ( R[Pos[2]].first, Pos[2],
                                 R[Pos[1]].first,Pos[1], comp ))
                {   std::swap ( Pos[1], Pos[2]);
                    if (NPos == 4 and
                        less_range (R[Pos[3]].first,Pos[3],
                                    R[Pos[2]].first, Pos[2], comp))
                    {   std::swap ( Pos[2], Pos[3]);
                    };
                };
            };
        };
    };
   
	range1_t Rx1 ( dest.first, it_dest), Rx2 ( it_dest, dest.last);
    if ( Pos[0]< Pos[1])
        return concat (Rx1,full_merge ( Rx2, R[Pos[0]],R[Pos[1]], comp));
    else
        return concat(Rx1,full_merge ( Rx2, R[Pos[1]],R[Pos[0]], comp));
};
//-----------------------------------------------------------------------------
//  function : uninit_full_merge4
/// @brief Merge four ranges and put the result in uninitialized memory
/// @param [in] dest: range where create and move the elements merged. Their  
///                   size must be greater or equal than the sum of the sizes 
///                   of the ranges in the array R
/// @param [in] R : array of ranges to merge
/// @param [in] NR : number of ranges in R
/// @param [in] comp : comparison object
/// @return range with all the elements move with the size adjusted
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class compare >
range<iter1_t> uninit_full_merge4 ( range<iter1_t> dest,  range<iter2_t> R[4],
                                    uint32_t NR ,compare comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	//typedef range<iter2_t>         				range2_t;
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
	//----------------------------- begin ------------------------------------
	size_t Ndest = 	0 ;
	uint32_t i = 0 ;
	while (i < NR )
	{ 	if (R[i].size() != 0 ) Ndest += R[i++].size() ;
		else
		{	for ( uint32_t k = i+1 ; k < NR ; ++k)
				R[k-1] = R[k] ;
			--NR ;
		};
	};
#if __DEBUG_SORT != 0
	assert ( dest.size() >= Ndest ) ;
#endif
    if ( NR == 0 ) return range1_t (dest.first, dest.first) ;
    if ( NR == 1 ) return uninit_move ( dest, R[0]);
    if ( NR == 2 ) return uninit_full_merge ( dest, R[0],R[1], comp);

    //------------------------------------------------------------------------
    // Initial sort
    //------------------------------------------------------------------------
    uint32_t Pos[4]={0,1,2,3}, NPos =NR  ;

    if (less_range (R[Pos[1]].first, Pos[1],R[Pos[0]].first,Pos[0],comp))
        std::swap (Pos[0], Pos[1]);
    if (less_range (R[Pos[2]].first,Pos[2],R[Pos[1]].first,Pos[1],comp))
        std::swap (Pos[1], Pos[2]);
    if (NPos == 4 and
        less_range (R[Pos[3]].first, Pos[3],R[Pos[2]].first,Pos[2],comp))
        std::swap ( Pos[3], Pos[2]);

    if (less_range (R[Pos[1]].first,Pos[1],R[Pos[0]].first,Pos[0],comp))
        std::swap (Pos[0], Pos[1]);
    if (NPos==4 and
        less_range (R[Pos[2]].first, Pos[2], R[Pos[1]].first,Pos[1],comp))
        std::swap(Pos[1],Pos[2]);

    if (NPos == 4 and
        less_range (R [Pos[1]].first, Pos[1],R[Pos[0]].first, Pos[0], comp))
        std::swap ( Pos[0], Pos[1]);
        
	iter1_t it_dest = dest.first ;
    while ( NPos > 2)
    {   construct_object ( &(* (it_dest++)),std::move ( *(R[Pos[0]].first++)) );
        if (R[Pos[0]].size() == 0   )
        {   Pos[0] = Pos[1];
            Pos[1] = Pos[2];
            Pos[2] = Pos[3];
            --NPos ;
        }
        else
        {   if ( less_range (R[Pos[1]].first, Pos[1],
                             R[Pos[0]].first,Pos[0],comp))
            {   std::swap ( Pos[0], Pos[1]);
                if ( less_range (R[Pos[2]].first, Pos[2],
                                 R[Pos[1]].first, Pos[1], comp ))
                {   std::swap ( Pos[1], Pos[2]);
                    if (NPos == 4 and less_range (R[Pos[3]].first, Pos[3],
                                                  R[Pos[2]].first,Pos[2], comp))
                    {   std::swap ( Pos[2], Pos[3]);
                    };
                };
            };
        };
    };
       
	range1_t Rx1 ( dest.first, it_dest), Rx2 ( it_dest, dest.last);
    if ( Pos[0]< Pos[1])
        return concat (Rx1,uninit_full_merge ( Rx2, R[Pos[0]],R[Pos[1]], comp));
    else
        return concat(Rx1,uninit_full_merge ( Rx2, R[Pos[1]],R[Pos[0]], comp));
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
