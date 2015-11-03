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
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <random>
#include <iostream>
#include <future>
#include <algorithm>
#include <tools/atomic.hpp>
#include <sort/util/token_block.hpp>
#include <sort/util/token_merge.hpp>
#include <sort/util/dispatcher.hpp>

#include <sort/util/full_merge.hpp>
#include <vector>
#include <algorithm>

using namespace _sort::util ;
using namespace tools;



template <class iter_t >
std::ostream & operator << ( std::ostream & out , range<iter_t>  R)
{	out<<"[ "<<(R.last- R.first)<<"] ";
	if ( not R.valid() ) return out ;
	while ( R.first != R.last)
		out << (*(R.first++)) <<" ";
	out<<std::endl ;
	return out ;
};

void prueba1 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator 	iter_t ;
    typedef std::less<uint64_t>                     	compare ;
    typedef uint64_t*   								iterb_t ;

	typedef token_sort<iter_t,iterb_t, compare>			token_sort_t ;
	typedef token_move<iter_t,iterb_t, compare>			token_move_t ;
	typedef token_range<iter_t,iterb_t, compare>		token_range_t ;

    const uint32_t NMAX = 1<<20 ;
    const uint32_t NMAX2 = NMAX >>1 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;
    std::atomic<uint32_t> N ;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < (NMAX+200) ; ++i)
        V1.push_back(my_rand() );

    range<iter_t> R ( V1.begin(), V1.end());
    range <uint64_t *> Raux (&Aux[0], &Aux[BLK_SIZE] );

    std::sort ( V1.begin() , V1.begin() + NMAX2);
    std::sort ( V1.begin()+ NMAX2 , V1.end());

    range<iter_t> R1 ( V1.begin() , V1.begin() + NMAX2);
    range<iter_t> R2 ( V1.begin()+ NMAX2 , V1.end());


    block_merge_internal <1024, iter_t, compare> BM( R1, R2, comp );

    token_sort_t RS1( &N, R1,comp );
    token_sort_t RS2( &N, R2,comp );
    token_move_t TM ( &N, BM , 1);
    token_range_t TR ( &N, BM , 2);

};

void prueba2 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef uint64_t* 									iter_t ;
    typedef std::less<uint64_t>                     	compare ;
    typedef uint64_t*   								iterb_t ;


    const uint32_t NMAX = 1<<20 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1;
    V1.reserve ( NMAX+ 200);
    for ( uint32_t i = 0 ; i < (NMAX+200) ; ++i)
        V1.push_back(my_rand() );

    range<iter_t> R ( &V1[0] , &V1[V1.size()]);
    range <iterb_t> Raux (&Aux[0], &Aux[BLK_SIZE] );

    sort::util::dispatcher<iter_t, iterb_t, compare> DSP (1);
	DSP.split_range (&(DSP.N), R, comp,2 );
	DSP.exec(Raux);


    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =1 ; i < V1.size() ; ++i)
    {	if ( V1[i]< V1[i-1])
    	{	std::cout<<"Error in the fusion "<< i<<" - ";
    		std::cout<<V1[i-1]<<" , "<<V1[i]<<std::endl ;
    		return ;
    	};
    };

};

void prueba5 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator 	iter_t ;
    typedef std::less<uint64_t>                     	compare ;
    typedef uint64_t*   								iterb_t ;
    //typedef token<iterb_t> 								token_t ;
	//typedef std::unique_ptr<token_t>    				ptoken_t ;
	typedef token_merge<iter_t,iterb_t, compare>		token_merge_t ;
	typedef token_sort<iter_t,iterb_t, compare>			token_sort_t ;

    const uint32_t NMAX = 1<<20 ;
    const uint32_t NMAX2 = NMAX >>1 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1, V2;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < (NMAX+200) ; ++i)
        V1.push_back(my_rand() );


    std::sort ( V1.begin() , V1.begin() + NMAX2);
    std::sort ( V1.begin()+ NMAX2 , V1.end());
    V2 = V1 ;

    range<iter_t> R1 ( V2.begin() , V2.begin() + NMAX2);
    range<iter_t> R2 ( V2.begin()+ NMAX2 , V2.end());
    range <uint64_t *> Raux (&Aux[0], &Aux[BLK_SIZE] );

    block_merge_internal <1024, iter_t, compare> BM( R1, R2, comp );
    BM.fill_Vb (Raux) ;
    BM.move_blocks (Raux) ;
    BM.create_ranges ( 64,Raux);
    BM.merge_ranges(Raux) ;

    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =1 ; i < V1.size() ; ++i)
    {	if ( V2[i]< V2[i-1])
    	{	std::cout<<"Error in the fusion "<< i<<" - ";
    		std::cout<<V2[i-1]<<" , "<<V2[i]<<std::endl ;
    		return ;
    	};
    };
    //std::cout<<"Fin block_merge_internal \n";

    //------------------------------------------------------------------------
    //                  V1
    //------------------------------------------------------------------------
    R1 = range<iter_t> ( V1.begin() , V1.begin() + NMAX2);
    R2 = range<iter_t>( V1.begin()+ NMAX2 , V1.end());
    Raux = range <uint64_t *>(&Aux[0], &Aux[BLK_SIZE] );


    range<iter_t> R ( V1.begin() , V1.end());

    sort::util::dispatcher<iter_t, uint64_t*, compare> DSP (1);
	DSP.split_range (&(DSP.N), R, comp,2 );
	//DSP.exec(Raux);


	auto E = std::async ( std::launch::async,
			              &sort::util::dispatcher<iter_t, uint64_t*, compare>::exec, &DSP, Raux);
	E.get() ;
    //dq_exec<iterb_t> (N,dq, Raux);

    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =1 ; i < V1.size() ; ++i)
    {	if ( V1[i]< V1[i-1])
    	{	std::cout<<"Error in the fusion "<< i<<" - ";
    		std::cout<<V1[i-1]<<" , "<<V1[i]<<std::endl ;
    		return ;
    	};
    };
    //------------------------------------------------------------------------
    //            Verification Comparison
    //------------------------------------------------------------------------
    assert ( V1.size() == V2.size()) ;
    for ( uint32_t i = 0 ; i < V1.size() ; ++i)
    {   if  ( V1[i] != V2[i] )
    	{	std::cout<<"Error in the position "<< i<<" - ";
    		std::cout<<V1[i]<<" , "<<V2[i]<<std::endl ;
    		return ;
    	};
    };

};

/*
void prueba6 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator 	iter_t ;
    typedef std::less<uint64_t>                     	compare ;
    typedef uint64_t*   								iterb_t ;
    //typedef token<iterb_t> 								token_t ;
	//typedef std::unique_ptr<token_t>    				ptoken_t ;
	//typedef token_merge<iter_t,iterb_t, compare>		token_merge_t ;
	//typedef token_sort<iter_t,iterb_t, compare>			token_sort_t ;

    const uint32_t NMAX = 1<<20 ;
    //const uint32_t NMAX2 = NMAX >>1 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1, V2;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < (NMAX+200) ; ++i)
        V1.push_back(my_rand() );

    range<iter_t> R1 ( V1.begin() , V1.end());
    range <uint64_t*> Raux (&Aux[0], &Aux[BLK_SIZE] );
	std::atomic<uint32_t>  N (1);
	dq_cnc_token<iterb_t>  dq ;

	sort::util::split_range<iter_t, uint64_t*,compare> (&N, R1,2,comp,dq);


	//auto E = std::async (std::launch::async, sort::util::dq_exec<iterb_t>, std::ref(N), std::ref(dq), Raux);
	//E.get() ;
    dq_exec<iterb_t> (N,dq, Raux);

    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =1 ; i < V1.size() ; ++i)
    {	if ( V1[i]< V1[i-1])
    	{	std::cout<<"Error in the fusion "<< i<<" - ";
    		std::cout<<V1[i-1]<<" , "<<V1[i]<<std::endl ;
    		return ;
    	};
    };
    //------------------------------------------------------------------------
    //            Verification Comparison
    //------------------------------------------------------------------------
    assert ( V1.size() == V2.size()) ;
    for ( uint32_t i = 0 ; i < V1.size() ; ++i)
    {   if  ( V1[i] != V2[i] )
    	{	std::cout<<"Error in the position "<< i<<" - ";
    		std::cout<<V1[i]<<" , "<<V2[i]<<std::endl ;
    		return ;
    	};
    };

};
*/
/*
void prueba6 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator 	iter_t ;
    typedef std::less<uint64_t>                     	compare ;
    typedef uint64_t*   								iterb_t ;


    const uint32_t NMAX = 1<<20 ;
    //const uint32_t NMAX2 = NMAX >>1 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1, V2;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < (NMAX+200) ; ++i)
        V1.push_back(my_rand() );

    range<iter_t> R1 ( V1.begin() , V1.end());
    range <uint64_t*> Raux (&Aux[0], &Aux[BLK_SIZE] );

    dispatcher<iter_t, uint64_t*, compare> DSP ( R1,comp);


	sort::util::split_range<iter_t, uint64_t*,compare> (&N, R1,2,comp,DSP.dqt, DSP.sqm);


	//auto E = std::async (std::launch::async, sort::util::dq_exec<iterb_t>, std::ref(N), std::ref(dq), Raux);
	//E.get() ;
    dq_exec<iterb_t> (N,dq, Raux);

    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =1 ; i < V1.size() ; ++i)
    {	if ( V1[i]< V1[i-1])
    	{	std::cout<<"Error in the fusion "<< i<<" - ";
    		std::cout<<V1[i-1]<<" , "<<V1[i]<<std::endl ;
    		return ;
    	};
    };
    //------------------------------------------------------------------------
    //            Verification Comparison
    //------------------------------------------------------------------------
    assert ( V1.size() == V2.size()) ;
    for ( uint32_t i = 0 ; i < V1.size() ; ++i)
    {   if  ( V1[i] != V2[i] )
    	{	std::cout<<"Error in the position "<< i<<" - ";
    		std::cout<<V1[i]<<" , "<<V2[i]<<std::endl ;
    		return ;
    	};
    };


};
*/
int main( int, char*[] )
{
    prueba1() ;
    prueba2() ;
    prueba5() ;
    //prueba6() ;
    return 0 ;
};



