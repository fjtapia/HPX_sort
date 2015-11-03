//----------------------------------------------------------------------------
/// @file test_heap_sort.cpp
/// @brief Test program of the heap sort algorithm
///
/// @author Copyright (c) 2010 2015 Francisco Jos� Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//#include <iostream>
#include <hpx/parallel/sort/algorithm/heap_sort.hpp>
#include <vector>

namespace bs_algo = hpx::parallel::_sort::algorithm ;
using namespace std ;

void prueba2 ()
{   //---------------------------- Inicio --------------------------------
    unsigned A[]={7,4,23,15,17,2,24,13,8,3,11,16, 6, 14,21,5,1,12,19,22,25,8};
	//cout<<"Heap Sort  Unordered not repeated ----------------------------\n";
	bs_algo::heap_sort ( &A[0] , &A[22] );
	for ( unsigned i = 0 ; i < 21 ; i ++ )
	{	assert ( A[i] <= A[i+1] ) ;
	};

    unsigned B[]={1,2,3,5,6,7,8,9,10,11,12,13,14,15,17,18,19,20,21,23,24,25};
	//cout<<"Heap Sort Sorted, not repeated ----------------------------\n";
	bs_algo::heap_sort ( &B[0] , &B[22] );
	for ( unsigned i = 0 ; i < 21 ; i ++ )
	{	assert ( B[i] <= B[i+1] ) ;
	};

    unsigned C [] ={27,26,25,23,22,21,19,18,17,16,15,14,13,11,10,9,8,7,6,5,3,2};
	//cout<<"Heap Sort reverse sorted, not repeated ------------------------\n";
	bs_algo::heap_sort (&C[0] , &C[22] );
	for ( unsigned i = 0 ; i < 21 ; i ++ )
	{	assert ( C[i] <= C[i+1] )  ;
	};

    unsigned D [] ={4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4};
	//cout<<"Heap Sort equals ----------------------------\n";
	bs_algo::heap_sort ( &D[0] , &D[22]);
	for ( unsigned i = 0 ; i < 21 ; i ++ )
	{	assert ( D[i] >= D[i+1] ) ;
	};

	//cout<<"Heap Sort 100 random numbers ----------------------------\n";
	unsigned F[100];
	for ( unsigned i = 0 ; i < 100 ; i ++) F[i] = rand()%1000 ;
	bs_algo::heap_sort ( &F[0] , &F[100] );
	for ( unsigned i = 0 ; i < 99 ; i ++ )
	{	assert ( F[i] <= F[i+1] )  ;
	};

	const unsigned NG= 10000;
	//cout<<"Heap Sort "<<NG<<" random numbers ----------------------------\n";
	unsigned G[NG];
	for ( unsigned i = 0 ; i < NG ; i ++) G[i] = rand()%1000 ;
	bs_algo::heap_sort ( &G[0] , &G[NG] );
	for ( unsigned i = 0 ; i < NG-1 ; i ++ )
	{	assert ( G[i] <= G[i+1] ) ;
	};
};

void prueba3 ( void)
{   //---------------------- begin ------------------------------------
    const uint32_t NELEM = 100000 ;
    std::vector <uint32_t>  A ;
    A.reserve ( NELEM) ;

    for ( unsigned i = 0  ; i < NELEM ; i ++ )
		A.push_back(rand() )  ;

    //cout<<"--------------------- heap_sort ----------------------------\n";
	bs_algo::heap_sort (&A[0] , &A[NELEM]  );
	for ( unsigned i = 0 ; i < (NELEM -1) ; i ++ )
	{	assert ( A[i] <= A[i+1] ) ;
	};
}

int main(int argc, char* argv[])
{	return hpx::init(argc, argv);
}
int hpx_main(boost::program_options::variables_map&)
{	
    {
    	prueba2() ;
    	prueba3() ;
    }

    // Initiate shutdown of the runtime systems on all localities.
    return hpx::finalize();
}
