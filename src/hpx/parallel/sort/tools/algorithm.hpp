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
#ifndef __HPX_PARALLEL_SORT_TOOLS_ALGORITHM_HPP
#define __HPX_PARALLEL_SORT_TOOLS_ALGORITHM_HPP

#include <hpx/parallel/sort/tools/metaprog.hpp>
#include <hpx/parallel/sort/tools/definition.hpp>
#include <hpx/parallel/sort/tools/config.hpp>

#include <cassert>
#include <climits>
namespace hpx
{
namespace parallel
{
namespace _sort
{
namespace tools
{

//
//###########################################################################
//                                                                         ##
//    ################################################################     ##
//    #                                                              #     ##
//    #            S T R U C T      C N F < 4 >                      #     ##
//    #                                                              #     ##
//    ################################################################     ##
//                                                                         ##
//###########################################################################
//
template <unsigned NByte>
struct cnf
{
//
//##########################################################################
//                                                                        ##
//         M E T A P R O G R A M M I N G     F U N C T I O N S            ##
//                                                                        ##
//##########################################################################
//
//---------------------------------------------------------------------------
//  function : MS1B
/// @brief Obtain the position of the most significant one in N
/// @param [in] N : Number to examine
/// @exception none
/// @return Position of the first one
/// @remarks
//---------------------------------------------------------------------------
CONSTEXPR static inline int  MS1B  ( uint32_t N )
{   return mtprg::MS1B_32<16> (N,0);
};
//
//##########################################################################
//                                                                        ##
//                I N L I N E      F U N C T I O N S                      ##
//                                                                        ##
//##########################################################################
//
//---------------------------------------------------------------------------
//  function :bitScanForward
/// @author Martin Läuter(1997),Charles E.Leiserson,Harald Prokop,Keith H.Randall
/// @brief return the least significant one bit
/// @param [in]V1 bitboard to scan
/// @return index (0..31) of least significant one bit
/// @remarks "Using de Bruijn Sequences to Index a 1 in a Computer Word"
//---------------------------------------------------------------------------
static inline uint32_t BitScanForward (uint32_t V) NOEXCEPT
{   //------------------------- begin --------------------------------
    return mtprg::DeBruijn_32[((uint32_t)((V & ((~V)+1)) * 0x077CB531U)) >> 27];
};
//
//---------------------------------------------------------------------------
//  function : NBits
/// @brief Obtain the number of bits equal or greater than N
/// @param [in] N : Number to examine
/// @exception none
/// @return Number of bits
//---------------------------------------------------------------------------
static inline uint32_t NBits ( uint32_t N) NOEXCEPT
{   //----------------------- begin -------------------------------------
    int Pos = (N & 0xffff0000U)?16:0 ;
    if ((N>>Pos) & 0xff00U) Pos += 8 ;
    return (  mtprg::TMSB [ N >> Pos ] + Pos );
}

static inline uint32_t  BitScanReverse  ( uint32_t N )
{   //--------------------------- begin ----------------------------
    int NB = NBits ( N);
    return  ( NB == 0 )? INT_MAX : NB-1;
};

}; // end template <unsigned NByte> cnf

//
//###########################################################################
//                                                                         ##
//    ################################################################     ##
//    #                                                              #     ##
//    #            S T R U C T      C N F < 8 >                      #     ##
//    #                                                              #     ##
//    ################################################################     ##
//                                                                         ##
//###########################################################################
//
template < >
struct cnf<8>
{
//##########################################################################
//                                                                        ##
//         M E T A P R O G R A M M I N G     F U N C T I O N S            ##
//                                                                        ##
//##########################################################################

//---------------------------------------------------------------------------
//  function : MS1B_64
/// @brief Obtain the position of the most significant one in N
/// @param [in] N : Number to examine
/// @exception none
/// @return Position of the first one
//---------------------------------------------------------------------------
CONSTEXPR static inline int  MS1B  ( uint64_t N )
{   return mtprg::MS1B_64<32> (N,0);
};
//
//##########################################################################
//                                                                        ##
//                I N L I N E      F U N C T I O N S                      ##
//                                                                        ##
//##########################################################################
//
//---------------------------------------------------------------------------
//  function :bitScanForward64
/// @author Martin Läuter(1997),Charles E.Leiserson,Harald Prokop,Keith H.Randall
/// @brief return the least significant one bit
/// @param [in] bb bitboard to scan
/// @return index (0..63) of least significant one bit
/// @remarks "Using de Bruijn Sequences to Index a 1 in a Computer Word"
//---------------------------------------------------------------------------
static inline uint32_t BitScanForward(uint64_t bb) NOEXCEPT
{   //------------------------- begin --------------------------------
    return (mtprg::DeBruijn_64[((bb & ((~bb)+1)) * 0x07EDD5E59A4E28C2ULL) >> 58]);
};
//
//---------------------------------------------------------------------------
//  function : NBits
/// @brief Obtain the number of bits equal or greater than N
/// @param [in] N : Number to examine
/// @exception none
/// @return Number of bits
//---------------------------------------------------------------------------
static inline uint32_t NBits ( uint64_t N)
{   //----------------------- begin -------------------------------------
    uint32_t  Pos = ( N & 0xffffffff00000000ULL)?32:0 ;
    if ( (N>>Pos) & 0xffff0000ULL ) Pos +=16  ;
    if ( (N>>Pos) & 0xff00ULL     ) Pos += 8 ;
    return ( mtprg::TMSB [ N >> Pos ] + Pos );
}

static inline uint32_t  BitScanReverse  ( uint64_t N )
{   //------------------ begin ---------------------------
    int NB = NBits ( N);
    return  ( NB == 0)?INT_MAX : NB-1;
};


}; // end template < > cnf<8>

//##########################################################################
//                                                                        ##
//   A L I A S    M E T A P R O G R A M M I N G    F U N C T I O N S      ##
//                                                                        ##
//##########################################################################

CONSTEXPR auto MS1B = cnf<NByte>::MS1B ;
CONSTEXPR auto MS1B_32 = cnf<4>::MS1B ;
CONSTEXPR auto MS1B_64 = cnf<8>::MS1B ;

//##########################################################################
//                                                                        ##
//            A L I A S   I N L I N E    F U N C T I O N S                ##
//                                                                        ##
//##########################################################################

CONSTEXPR auto LS1B       = cnf<NByte>::BitScanForward ;
CONSTEXPR auto LS1B_32    = cnf<4>::BitScanForward ;
CONSTEXPR auto LS1B_64    = cnf<8>::BitScanForward ;


auto BitScanReverse32 = cnf<4>::BitScanReverse ;
auto BitScanReverse64 = cnf<8>::BitScanReverse ;
auto BitScanReverse = cnf<NByte>::BitScanReverse ;

auto NBits = cnf<NByte>::NBits ;
auto NBits32 = cnf<4>::NBits ;
auto NBits64 = cnf<8>::NBits ;


//****************************************************************************
};//    End namespace tools
};//    End namespace _sort
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
#endif
