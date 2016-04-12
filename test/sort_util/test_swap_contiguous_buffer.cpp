//----------------------------------------------------------------------------
/// @file test_swap_contiguous_buffer.cpp
/// @brief
///
/// @author Copyright (c) 2010 2012 Francisco Jose Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See a ccompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <iostream>
#include <cassert>
#include <sort/util/swap_contiguous_buffer.hpp>

#include <vector>
#include <algorithm>


using std::cout ;
using std::endl;

using HPX_INLINE_NAMESPACE(v2) { namespace sort::util;

typedef typename std::vector<uint32_t>::iterator iter_t ;
std::vector<uint32_t> A ;
uint32_t Aux[4];


int main ()
{   //------------------------- begin -------------------------------------
    //------------------------------------------------------------------------
    //       N1 == N2
    //------------------------------------------------------------------------
    A = {10,11,12,13,14,15,16,17,18,19,0,1,2,3,4,5,6,7,8,9};
    swap_contiguous_buffer<iter_t> (A.begin(),A.begin()+10,A.end(),Aux, 4 );
    for ( uint32_t i =0 ; i < A.size() ; ++i)
    if ( A[i] != i )std::cout <<"Error\n"<<std::endl;

    //------------------------------------------------------------------------
    //  N1 > N2 N1 <= ( N2 + NP) NP = 4
    //------------------------------------------------------------------------
    A = {9,10,11,12,13,14,15,16,17,18,19,0,1,2,3,4,5,6,7,8};
    swap_contiguous_buffer<iter_t> (A.begin(),A.begin()+11,A.end(),Aux, 4 );
    for ( uint32_t i =0 ; i < A.size() ; ++i)
    if ( A[i] != i )std::cout <<"Error\n"<<std::endl;

    //--------------------------------------------------------------------
    // N1 > N2      N1 > ( N2+ NP ) N2 <= NP
    //--------------------------------------------------------------------
    A = {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,0,1,2,3};
    swap_contiguous_buffer<iter_t> (A.begin(),A.begin()+16,A.end(),Aux, 4 );
    for ( uint32_t i =0 ; i < A.size() ; ++i)
    if ( A[i] != i )std::cout <<"Error\n"<<std::endl;

    //--------------------------------------------------------------------
    // N1 > N2      N1 > ( N2+ NP ) N2 > NP
    //--------------------------------------------------------------------
    A = {5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,0,1,2,3,4};
    swap_contiguous_buffer<iter_t> (A.begin(),A.begin()+15,A.end(),Aux, 4 );
    for ( uint32_t i =0 ; i < A.size() ; ++i)
    if ( A[i] != i )std::cout <<"Error\n"<<std::endl;

    //--------------------------------------------------------------------
    // N2 > N1      N2 <= ( N1+ NP )
    //--------------------------------------------------------------------
    A = {11,12,13,14,15,16,17,18,19,0,1,2,3,4,5,6,7,8,9,10};
    swap_contiguous_buffer<iter_t> (A.begin(),A.begin()+9,A.end(),Aux, 4 );
    for ( uint32_t i =0 ; i < A.size() ; ++i)
    if ( A[i] != i )std::cout <<"Error\n"<<std::endl;

    //--------------------------------------------------------------------
    // N2 > N1      N2 > ( N1+ NP ) N1 <= NP
    //--------------------------------------------------------------------
    A = {16,17,18,19,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    swap_contiguous_buffer<iter_t> (A.begin(),A.begin()+4,A.end(),Aux, 4 );
    for ( uint32_t i =0 ; i < A.size() ; ++i)
    if ( A[i] != i )std::cout <<"Error\n"<<std::endl;

    //--------------------------------------------------------------------
    // N2 > N1      N2 > ( N1+ NP ) N1 > NP
    //--------------------------------------------------------------------
    A = {15,16,17,18,19,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
    swap_contiguous_buffer<iter_t> (A.begin(),A.begin()+5,A.end(),Aux, 4 );
    for ( uint32_t i =0 ; i < A.size() ; ++i)
    if ( A[i] != i )std::cout <<"Error\n"<<std::endl;


    return 0;
};




