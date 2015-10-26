//----------------------------------------------------------------------------
/// @file test_range.cpp
/// @brief
///
/// @author Copyright (c) 2010 2015 Francisco Jos� Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sort/util/range.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

using sort::util::range ;
namespace bs_util = sort::util ;

template <class iter_t >
std::ostream & operator << ( std::ostream & out , range<iter_t>  R)
{	out<<"[ "<<(R.last- R.first)<<"] ";
	if ( not R.valid() ) return out ;
	while ( R.first != R.last)
		out << (*(R.first++)) <<" ";
	out<<std::endl ;
	return out ;
}
//
//****************************************************************************
//          TEST MOVE, UNINITIALIZED_MOVE, DESTROY
//****************************************************************************
void prueba1 ( void )
{   //-------------------------------------- begin --------------------------
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef range<iter_t>  range_t ;

    std::vector<uint64_t> A , B;

    A.resize ( 10, 0);
    B.resize ( 10, 0);
    for ( uint32_t i =0 ; i < 10 ; ++i) A[i] = i ;
    range_t RA (A.begin() , A.end()), RB ( B.begin() , B.end());
    std::cout<<RA<<std::endl;
	//------------------------------------------------------------------------
	// test copy copy constructor
	//------------------------------------------------------------------------
	range_t RC ( RA);
	assert ( RC.size() == RA.size());
    std::cout<<RC<<std::endl;
    RC.first = RC.last ;
    RC = RA ;
    std::cout<<RC<<std::endl;
	//-----------------------------------------------------------------------
    //              test of move
    //------------------------------------------------------------------------

    RB = bs_util::move( RB, RA);
    for ( uint32_t i =0 ; i < 10 ; ++i)
        assert ( B[i] == i) ;
    std::cout<<RB<<std::endl;
    //------------------------------------------------------------------------
    //           test of uninitialized_move , destroy
    //------------------------------------------------------------------------
    struct forensic
    {   int64_t N ;
        forensic ( uint64_t K = 0 ){ N = (int64_t)K ;};
        ~forensic () { N = -1 ;};
    };
    typedef typename std::vector<forensic>::iterator fiter_t ;
    typedef bs_util::range<fiter_t>  frange_t ;

    char K[160];
    forensic *PAux = reinterpret_cast <forensic*> ( &K[0]);
    range <forensic *> F1 (PAux , PAux + 20 );
    std::vector <forensic> V ;
    for ( uint32_t i =0 ; i < 10 ; ++i) V.emplace_back ( i);

    F1 = bs_util::uninit_move ( F1, frange_t(V.begin(),V.end()));
    for ( uint32_t i =0 ; i < 10 ; ++i)
        assert ( PAux[i].N == i) ;

    bs_util::destroy <forensic *>( F1);
    //for ( uint32_t i =0 ; i < 10 ; ++i)
    //{   if ( PAux[i].N != -1)
    //        std::cout<<"Error in the destroy function \n";
    //};
};
void prueba2 ( )
{	//---------------------------------- begin -----------------------------
	typedef typename std::vector <uint64_t >::iterator iter_t ;
	std::vector <uint64_t > V1 ;
	V1.resize (100, 0);
	range <iter_t> R1 ( V1.begin() , V1.end());
	uint64_t K = 999 ;
	range <iter_t> R2 = init ( R1, K);
	while ( R2.first != R2.last)
		assert ( * (R2.first ++) == 999);
};


int main( int, char*[] )
{   //-------------- begin------------
    prueba1() ;
    prueba2 () ;
    return 0 ;
};
