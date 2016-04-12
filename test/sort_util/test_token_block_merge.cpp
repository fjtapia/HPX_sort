//----------------------------------------------------------------------------
/// @file test_block_merge_parallel.cpp
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
#include <iostream>
#include <tools/atomic.hpp>
#include <sort/util/token_block_merge.hpp>
#include <vector>
#include <algorithm>

using HPX_INLINE_NAMESPACE(v2) { namespace sort::util ;
using namespace tools;


int main( int, char*[] )
{	//------------------------------------------------------------------------
	//                     DEFINITIONS
	//------------------------------------------------------------------------
	typedef typename std::vector<uint64_t>::iterator 	iter_t ;
	typedef uint64_t *                               	iterb_t ;
	//typedef token<iterb_t>            					token_t ;
	typedef std::less<uint64_t>							compare ;

	//------------------------------------------------------------------------
	//                     VARIABLES
	//------------------------------------------------------------------------
	std::atomic<uint32_t> N (7);
	std::vector<uint64_t> V ( 100 , 0 );
	dq_cnc_token<iterb_t> DQ ;
	token<iterb_t> *Aux ;
	compare comp ;

	//--------------------- token_join -------------------------------------
	//token_join<iterb_t> TJ (&N, 2);
	Aux = new token_join<iterb_t> (&N, 2);
	DQ.emplace_back( Aux);


	//----------------------- token_sort ----------------------------------
	range<iter_t>  R1 ( V.begin() , V.end());
	Aux = new token_sort<iter_t, iterb_t,compare> ( &N,R1,comp);
	DQ.emplace_back( Aux);

	//------------------------ token_move ----------------------------------
	range<iter_t>  RA ( V.begin() ,V.begin() +50), RB(V.begin() +50, V.end());
	block_merge_internal<1024, iter_t, compare> BM (RA,RB,comp );
	Aux = new token_move<iter_t, iterb_t,compare>  ( &N,BM,1);
	DQ.emplace_back( Aux);

	//--------------------------token_range ---------------------------------
	Aux = new token_range<iter_t, iterb_t,compare>  ( &N,BM,1);
	DQ.emplace_back( Aux);

	//--------------------------- token_merge -------------------------------
	Aux =  new token_move<iter_t, iterb_t,compare>  ( &N,BM,1) ;
	DQ.emplace_back( Aux);

	Aux = new token_merge<iter_t, iterb_t,compare> (&N, RA,RB,comp,DQ );

	DQ.emplace_back( Aux);



    return 0 ;
};



