//----------------------------------------------------------------------------
/// @file algorithm.hpp
/// @brief This file contains the description of several low level algorithms
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __HPX_PARALLEL_SORT_DETAIL_UTIL_NBITS_HPP
#define __HPX_PARALLEL_SORT_DETAIL_UTIL_NBITS_HPP

#include <cstdint>

namespace hpx		{
namespace parallel	{ 
namespace tr1		{
namespace detail	{
namespace util		{
//
//##########################################################################
//                                                                        ##
//                    G L O B A L     V A R I B L E S                     ##
//                                                                        ##
//##########################################################################
//
static constexpr const uint32_t tmsb [256] =
{   0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};
//
//##########################################################################
//                                                                        ##
//                I N L I N E      F U N C T I O N S                      ##
//                                                                        ##
//##########################################################################
//
//---------------------------------------------------------------------------
//  function : nbits32
/// @brief Obtain the number of bits equal or greater than num
/// @param [in] num : Number to examine
/// @exception none
/// @return Number of bits
//---------------------------------------------------------------------------
static inline uint32_t nbits32 ( uint32_t num) noexcept
{   //----------------------- begin -------------------------------------
    int Pos = (num & 0xffff0000U)?16:0 ;
    if ((num>>Pos) & 0xff00U) Pos += 8 ;
    return (  tmsb [ num >> Pos ] + Pos );
}
//
//---------------------------------------------------------------------------
//  function : nbits64
/// @brief Obtain the number of bits equal or greater than num
/// @param [in] num : Number to examine
/// @exception none
/// @return Number of bits
//---------------------------------------------------------------------------
static inline uint32_t nbits64 ( uint64_t num)
{   //----------------------- begin -------------------------------------
    uint32_t  Pos = ( num & 0xffffffff00000000ULL)?32:0 ;
    if ( (num>>Pos) & 0xffff0000ULL ) Pos +=16  ;
    if ( (num>>Pos) & 0xff00ULL     ) Pos += 8 ;
    return ( tmsb [ num >> Pos ] + Pos );
}

//
//***************************************************************************
};// end namespace util
};// end namespace detail
};// end namespace tr1
};// end namespace parallel
};// end namespace hpx
//***************************************************************************
//
#endif
