//----------------------------------------------------------------------------
/// @file test_smart_merge_sort.cpp
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
#include <iostream>
#include <tools/time_measure.hpp>
#include <sort/util/block_merge.hpp>
#include <sort/util/full_merge.hpp>
#include <vector>
#include <algorithm>

using namespace sort::util ;
using namespace tools;

template <class iter_t >
std::ostream & operator << ( std::ostream & out , range<iter_t>  R)
{	out<<"[ "<<(R.last- R.first)<<"] ";
	if ( not R.valid() ) return out ;
	while ( R.first != R.last)
		out << (*(R.first++)) <<" ";
	out<<std::endl ;
	return out ;
}


void prueba1 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937_64 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef std::less<uint64_t>                     compare ;
    typedef range <iter_t > range_t ;
    const uint32_t NMAX = 128 ;
    //uint64_t Aux[4];
    compare comp ;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1, V2;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < NMAX ; i+=2)
        V1.push_back(i );
    for ( uint32_t i = 1 ; i < NMAX ; i+=2)
        V1.push_back(i );
    V2.resize ( 4 , 0 );
    range_t R1 ( V1.begin() , V1.begin() + 64), R2(V1.begin() + 64, V1.end() );
    range_t RB ( V2.begin() , V2.end() );


    block_merge_internal< 4, iter_t, compare > BM( R1, R2, comp);
    BM.fill_Vb (RB) ;
    BM.move_blocks (RB) ;
    BM.create_ranges ( 4,RB);
    BM.merge_ranges(RB) ;
};
void prueba2 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937_64 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef std::less<uint64_t>                     compare ;
    typedef range <iter_t > range_t ;
    const uint32_t NMAX = 128 ;
    //uint64_t Aux[4];
    compare comp ;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1, V2;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < NMAX ; i+=2)
        V1.push_back(i );
    for ( uint32_t i =0 ; i < 16 ; ++i)
    	V1.push_back ( 256 + i);

    for ( uint32_t i = 1 ; i < NMAX ; i+=2)
        V1.push_back(i );

    V1.push_back ( 128);
    V1.push_back ( 129);
    V2.resize ( 4 , 0 );
    range_t R1 ( V1.begin() , V1.begin() + 80), R2(V1.begin() + 80, V1.end() );
    range_t RB ( V2.begin() , V2.end() );


    block_merge_internal< 4, iter_t, compare > BM( R1, R2, comp);
    BM.fill_Vb (RB) ;
    BM.move_blocks (RB) ;
    BM.create_ranges ( 4,RB);
    BM.merge_ranges(RB) ;
};


void prueba3 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef std::less<uint64_t>                     compare ;
    const uint32_t NMAX = 1<<28 ;
    const uint32_t NMAX2 = NMAX >>1 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;
	double duracion ;
	time_point start, finish;

    //------------------------------------------------------------------------
    std::vector< uint64_t> V1;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0; i < (NMAX+200) ; ++i) V1.push_back(i);


    std::sort ( V1.begin() , V1.begin() + NMAX2);
    std::sort ( V1.begin()+ NMAX2 , V1.end());

    range<iter_t> R1 ( V1.begin() , V1.begin() + NMAX2);
    range<iter_t> R2 ( V1.begin()+ NMAX2 , V1.end());
    range <uint64_t *> Raux (&Aux[0], &Aux[BLK_SIZE] );

    std::cout<<"block_merge (sorted blocks)  : ";
    start= now() ;


    block_merge_internal< BLK_SIZE, iter_t,compare > BM( R1, R2,comp);
    BM.fill_Vb (Raux) ;
    BM.move_blocks (Raux) ;
    BM.create_ranges ( 64,Raux);
    BM.merge_ranges(Raux) ;
    finish = now() ;
    duracion = subtract_time(finish ,start) ;
    std::cout<<duracion<<" secs\n";


    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =0 ; i < NMAX+ 200 ; ++i)
    {	if ( V1[i] != i )
    	{	std::cout<<"Error in the fusion "<< i<<std::endl ;
    		return ;
    	};
    }

};
void prueba4 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef std::less<uint64_t>                     compare ;
    const uint32_t NMAX = 1<<28 ;
    const uint32_t NMAX2 = NMAX >>1 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;
	double duracion ;
	time_point start, finish;
    //------------------------------------------------------------------------
    std::vector< uint64_t> V1, V2, V3;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < (NMAX2) ; ++i) V1.push_back(i + NMAX2+200 );
    for ( uint32_t i = 0 ; i < (NMAX2+200) ; ++i)
        V1.push_back(i );

    range<iter_t> R1 ( V1.begin() , V1.begin() + NMAX2);
    range<iter_t> R2 ( V1.begin()+ NMAX2 , V1.end());
    range <uint64_t *> Raux (&Aux[0], &Aux[BLK_SIZE] );

    std::cout<<"block_merge (reverse sorted blocks)       : ";
    start= now() ;


    block_merge_internal< BLK_SIZE, iter_t,compare > BM( R1, R2, comp);
    BM.fill_Vb (Raux) ;
    BM.move_blocks (Raux) ;
    BM.create_ranges ( 64,Raux);
    BM.merge_ranges(Raux) ;
    finish = now() ;
    duracion = subtract_time(finish ,start) ;
    std::cout<<duracion<<" secs\n";

    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =0 ; i < NMAX+ 200 ; ++i)
    {	if ( V1[i] != i )
    	{	std::cout<<"Error in the fusion "<< i<<std::endl ;
    		return ;
    	};
    }

};
void prueba5 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937 my_rand(0);
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef std::less<uint64_t>                     compare ;
    const uint32_t NMAX = 1<<20 ;
    const uint32_t NMAX2 = NMAX >>1 ;
    const uint32_t BLK_SIZE = 1024;
    uint64_t Aux[BLK_SIZE];
    compare comp ;
	double duracion ;
	time_point start, finish;
    //------------------------------------------------------------------------
    std::vector< uint64_t> V1, V2, V3;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < (NMAX+200) ; ++i)
        V1.push_back(my_rand() );


    std::sort ( V1.begin() , V1.begin() + NMAX2);
    std::sort ( V1.begin()+ NMAX2 , V1.end());
    V2 = V1 ;
    range<iter_t> R1 ( V1.begin() , V1.begin() + NMAX2);
    range<iter_t> R2 ( V1.begin()+ NMAX2 , V1.end());
    range <uint64_t *> Raux (&Aux[0], &Aux[BLK_SIZE] );

    std::cout<<"block_merge ( random blocks)       : ";
    start= now() ;


    block_merge_internal< BLK_SIZE,iter_t,compare > BM(R1,R2,comp);
    BM.fill_Vb (Raux) ;
    BM.move_blocks (Raux) ;
    BM.create_ranges (64,Raux);
    BM.merge_ranges(Raux) ;
    finish = now() ;
    duracion = subtract_time(finish ,start) ;
    std::cout<<duracion<<" secs\n";

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

    //MP.check_ranges() ;
    V3.resize( V2.size(),0);
    range<iter_t> R3 ( V3.begin() , V3.end());
    range<iter_t> RB1 ( V2.begin() , V2.begin() + NMAX2);
    range<iter_t> RB2 ( V2.begin()+ NMAX2 , V2.end());
    std::cout<<"full_merge        : ";
    start= now() ;
    full_merge ( R3 ,RB1, RB2,  comp);

    finish = now() ;
    duracion = subtract_time(finish ,start) ;
    std::cout<<duracion<<" secs\n";
    V2 = V3 ;
    //------------------------------------------------------------------------
    //            Verification
    //------------------------------------------------------------------------
    for ( uint32_t i =1 ; i < V2.size() ; ++i)
    {	if ( V2[i]< V2[i-1])
    	{	std::cout<<"Error in the fusion "<< i<<" - ";
    		std::cout<<V1[i-1]<<" , "<<V1[i]<<std::endl ;
    		return ;
    	};
    }
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
int main( int, char*[] )
{   //-------------- begin------------
    prueba1() ;
    prueba2() ;
    prueba3() ;
    prueba4() ;
    prueba5() ;
    return 0 ;
};



