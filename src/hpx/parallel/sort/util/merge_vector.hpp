//----------------------------------------------------------------------------
/// @file merge_vector.hpp
/// @brief
///
/// @author Copyright (c) 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __SORT_UTIL_MERGE_VECTOR_HPP
#define __SORT_UTIL_MERGE_VECTOR_HPP

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <hpx/parallel/sort/tools/util_iterator.hpp>
#include <hpx/parallel/sort/tools/algorithm.hpp>
#include <hpx/parallel/sort/util/merge_four.hpp>

namespace hpx
{
namespace parallel
{
namespace _sort
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
//  function : merge_level4
/// @brief merge the ranges in the vector Vin using full_merge4. The Vout 
///        vector is used as auxiliary memory in the internal process
///        The final results is in the dest range.
///        All the ranges of Vout are inside the range dest
/// @param [in] dest : range where move the elements merged
/// @param [in] Vin : vector of ranges to merge
/// @param [in] Vout : vector of ranges obtained
/// @param [in] comp : comparison object
/// @return range with all the elements moved
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class compare >
void merge_level4 ( range<iter1_t> dest,
                    std::vector<range<iter2_t> > &Vin,
                    std::vector<range<iter1_t> >& Vout ,
                    compare comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
	//------------------------------- begin ----------------------------------
    Vout.clear() ;
    if ( Vin.size() == 0) return ;
    if ( Vin.size() == 1 )
    {   Vout.emplace_back ( move ( dest, Vin[0]) );
        return ;
    };

    uint32_t Nrange = Vin.size() ;
    uint32_t PosIni = 0;
    while ( PosIni < Vin.size())
    {   uint32_t Nmerge =(Nrange + 3) >> 2 ;
        uint32_t Nelem =  ( Nrange + Nmerge -1) / Nmerge ;
        range1_t Rz = full_merge4 ( dest, & Vin[PosIni], Nelem , comp);
        Vout.emplace_back ( Rz );
        dest.first = Rz.last ;
        PosIni += Nelem ;
        Nrange -= Nelem ;
    };
    return  ;
};
//
//-----------------------------------------------------------------------------
//  function : uninit_merge_level4
/// @brief merge the ranges over uninitialized memory,in the vector Vin using  
///        full_merge4. The Voutvector is used as auxiliary memory in the 
///        internal process. The final results is in the dest range.
///        All the ranges of Vout are inside the range dest
/// @param [in] dest : range where move the elements merged
/// @param [in] Vin : vector of ranges to merge
/// @param [in] Vout : vector of ranges obtained
/// @param [in] comp : comparison object
/// @return range with all the elements moved
//-----------------------------------------------------------------------------
template <class iter1_t, class iter2_t, class compare >
void uninit_merge_level4 ( 	range<iter1_t> dest,
                    		std::vector<range<iter2_t> > &Vin,
							std::vector<range<iter1_t> >& Vout ,
							compare comp )
{   //---------------------------- begin ------------------------------------
	typedef range<iter1_t>         				range1_t;
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");
	//------------------------------- begin ----------------------------------
    Vout.clear() ;
    if ( Vin.size() == 0) return ;
    if ( Vin.size() == 1 )
    {   Vout.emplace_back ( uninit_move ( dest, Vin[0]) );
        return ;
    };

    uint32_t Nrange = Vin.size() ;
    uint32_t PosIni = 0;
    while ( PosIni < Vin.size())
    {   uint32_t Nmerge =(Nrange + 3) >> 2 ;
        uint32_t Nelem =  ( Nrange + Nmerge -1) / Nmerge ;
        range1_t Rz = uninit_full_merge4 ( dest, & Vin[PosIni], Nelem , comp);
        Vout.emplace_back ( Rz );
        dest.first = Rz.last ;
        PosIni += Nelem ;
        Nrange -= Nelem ;
    };
    return  ;
};
//
//-----------------------------------------------------------------------------
//  function : merge_vector4
/// @brief This function merge the ranges in Vin and put the final range in Vout
/// @tparam iter1_t : iterator to the input buffers
/// @tparam iter2_t : iterator to the output buffers
/// @tparam compare : object to compate the elements pointed by iter1_t
/// @param [in] first1 iterator to the first position of the Vin ranges
/// @param [in] first2 iterator to the first position of the Vout ranges
/// @param [in] Vin : vector of input ranges
/// @param [in] Vout : vector of output ranges
/// @param [in] comp : comparison object
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//  function : merge_vector4
/// @brief merge the ranges in the vector Vin using merge_level4. The Vout 
///        vector is used as auxiliary memory in the internal process
///        The final results is in the Rout range.
///        All the ranges of Vout are inside the range Rout
///        All the ranges of Vin are inside the range Rin
/// @param [in] Rin : range including all the ranges of Vin
/// @param [in]Rout : range including all the elements of Vout
/// @param [in] Vin : vector of ranges to merge
/// @param [in] Vout : vector of ranges obtained
/// @param [in] comp : comparison object
/// @return range with all the elements moved
//-----------------------------------------------------------------------------
template <class iter1_t , class iter2_t , class compare >
range<iter2_t> merge_vector4 ( range<iter1_t> Rin,
		                       range<iter2_t> Rout,
							   std::vector<range<iter1_t> > &Vin,
							   std::vector<range<iter2_t> > &Vout,
							   compare comp)
{   //---------------------------- begin ------------------------------------

	typedef range<iter2_t>         				range2_t;	
	typedef typename range<iter1_t>::value_t 	type1 ;
    typedef typename range<iter2_t>::value_t 	type2 ;    
           
    static_assert ( std::is_same<type1, type2>::value,
                   "Incompatible iterators\n");

    //--------------------- code -------------------------------------------
    Vout.clear() ;
    if ( Vin.size() == 0) return range2_t (Rout.first, Rout.first);
    if ( Vin.size() == 1) return move (Rout, Vin[0]);

    bool SW = false ;
    uint32_t Nrange = Vin.size() ;

    while ( Nrange >1)
    {   if ( SW )
        {   merge_level4 ( Rin, Vout, Vin, comp );
            SW = false ;
            Nrange = Vin.size() ;
        }
        else
        {   merge_level4 ( Rout , Vin, Vout, comp);
            SW = true ;
            Nrange = Vout.size() ;
        };
    };
	return ( SW) ?Vout[0]:move ( Rout, Vin[0]);
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
