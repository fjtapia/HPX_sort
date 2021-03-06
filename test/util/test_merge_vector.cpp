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
#include <vector>
#include <algorithm>
#include <utility>
#include <type_traits>

#include <hpx/parallel/sort/util/merge_vector.hpp>


using namespace hpx::parallel::sort::util  ;

template <class iter_t >
std::ostream & operator << ( std::ostream & out , range<iter_t>  R)
{	out<<"[ "<<(R.last- R.first)<<"] ";
	if ( not R.valid() ) return out ;
	while ( R.first != R.last)
		out << (*(R.first++)) <<" ";
	out<<std::endl ;
	return out ;
}
struct xk
{   unsigned tail : 3 ;
    unsigned num  :24 ;
    xk ( unsigned N=0, unsigned T=0):tail(T),num (N){};
    bool operator < ( xk A) const
    {   return ( num < A.num);
    };
};
std::ostream & operator << ( std::ostream & out , xk x)
{	out<<"["<< x.num <<"-"<<x.tail <<"] ";
	return out ;
};

//
//****************************************************************************
//               TEST OF  MERGE_LEVEL4
//****************************************************************************
void prueba1 ( void)
{   //------------------------------- begin-----------------------------------
    uint64_t X[10][10], Y[100];
    range<uint64_t*> RY ( &Y[0], &Y[100]);

    for ( uint32_t i =0 ; i < 4 ; ++i)
    {   for ( uint32_t k =0 ; k < 10 ; ++k)
            X[i][k] = i+ k*4 ;
    };
    for ( uint32_t i =0 ; i < 3 ; ++i)
    {   for ( uint32_t k =0 ; k < 10 ; ++k)
            X[i+4][k] = i+ 40+k*3 ;
    };
    for ( uint32_t i =0 ; i < 3 ; ++i)
    {   for ( uint32_t k =0 ; k < 10 ; ++k)
            X[i+7][k] = i+ 70+k*3 ;
    };

    for ( uint32_t i =0 ; i < 100 ; ++i) Y[i] = 1000 ;
    std::vector< range <uint64_t*> > V , Z;
    for ( uint32_t i  =0 ; i < 10 ; ++i)
        V.emplace_back( &X[i][0], &X[i][10]);
    merge_level4 ( RY, V, Z , std::less<uint64_t> ());

    for ( uint32_t i = 0 ; i < 100 ; ++i)
        assert ( Y[i] == i);
    assert ( Z.size() == 3 ) ;
} ;
void prueba2 ( void)
{	//---------------------------------- begin -----------------------------
	typedef typename std::vector <xk>::iterator iter_t ;
	typedef range<iter_t >                      rng ;
	typedef std::less<xk>                       compare ;

	std::vector<xk> VA, VB ;
	VB.resize ( 90);
	rng RB ( VB.begin() , VB.end());

	for ( uint32_t i =0 ; i < 30 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( k , i /10);
	};
	for ( uint32_t i =0 ; i < 30 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( k +10 , i /10);
	};
	for ( uint32_t i =0 ; i < 30 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( k+20 , i /10);
	};

	std::vector <rng > Vin, Vout ;
	for ( uint32_t i =0 ; i < 9 ; ++i)
	{	Vin.emplace_back (VA.begin() + (i*10), VA.begin() + ( (i+1)*10) );
	};
	compare comp  ;
	merge_level4 (RB, Vin, Vout, comp  );

	for ( uint32_t i =0 ; i < 90 ; ++i)
	{	assert (VB[i].num == i/3 and VB[i].tail == i %3);
	};
};

//
// ****************************************************************************
//               TEST OF  UNINIT_MERGE_LEVEL4
// ****************************************************************************
void prueba3 ( void)
{   //------------------------------- begin-----------------------------------
    uint64_t X[10][10], Y[100];
    range<uint64_t*> RY ( &Y[0], &Y[100]);

    for ( uint32_t i =0 ; i < 4 ; ++i)
    {   for ( uint32_t k =0 ; k < 10 ; ++k)
            X[i][k] = i+ k*4 ;
    };
    for ( uint32_t i =0 ; i < 3 ; ++i)
    {   for ( uint32_t k =0 ; k < 10 ; ++k)
            X[i+4][k] = i+ 40+k*3 ;
    };
    for ( uint32_t i =0 ; i < 3 ; ++i)
    {   for ( uint32_t k =0 ; k < 10 ; ++k)
            X[i+7][k] = i+ 70+k*3 ;
    };

    for ( uint32_t i =0 ; i < 100 ; ++i) Y[i] = 1000 ;
    std::vector< range <uint64_t*> > V , Z;
    for ( uint32_t i  =0 ; i < 10 ; ++i)
        V.emplace_back( &X[i][0], &X[i][10]);
    uninit_merge_level4 ( RY, V, Z , std::less<uint64_t> ());

    for ( uint32_t i = 0 ; i < 100 ; ++i)
        assert ( Y[i] == i);
    assert ( Z.size() == 3 ) ;
} ;

void prueba4 ( void)
{	//---------------------------------- begin -----------------------------
	typedef typename std::vector <xk>::iterator iter_t ;
	typedef range<iter_t >                      rng ;
	typedef std::less<xk>                       compare ;

	std::vector<xk> VA ;
	xk VB[ 90];
	range <xk*> RB ( &VB[0] , &VB[89]);

	for ( uint32_t i =0 ; i < 30 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( k , i /10);
	};
	for ( uint32_t i =0 ; i < 30 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( k +10 , i /10);
	};
	for ( uint32_t i =0 ; i < 30 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( k+20 , i /10);
	};

	std::vector <rng > Vin ;
	std::vector<range<xk*> > Vout ;
	for ( uint32_t i =0 ; i < 9 ; ++i)
	{	Vin.emplace_back (VA.begin() + (i*10), VA.begin() + ( (i+1)*10) );
	};
	compare comp  ;
	uninit_merge_level4 (RB, Vin, Vout, comp  );

	for ( uint32_t i =0 ; i < 90 ; ++i)
	{	assert (VB[i].num == i/3 and VB[i].tail == i %3);
	};
};


//
// ****************************************************************************
//                 TEST OF MERGE_VECTOR4
// ****************************************************************************

void prueba5 ( void)
{   //------------------------------- begin-----------------------------------
	typedef typename std::vector<uint64_t>::iterator 	iter_t ;
	typedef range<iter_t > 								rng ;
	typedef std::less<uint64_t>                         compare ;

	std::vector< uint64_t> X , Y ;
	compare comp ;
	Y.resize ( 100 , 0 );

	for ( uint32_t i =0 ; i < 10 ; ++i)
	{	for ( uint32_t k =0 ; k < 10 ; ++k)
		{	X.push_back ( k * 10 + i);
		}
	};
	rng Rin (X.begin() , X.end()  );
	rng Rout ( Y.begin() , Y.end() );

	std::vector <rng > Vin, Vout ;
	for ( uint32_t i =0 ; i < 10 ; ++i)
	{	Vin.emplace_back (X.begin() + (i*10), X.begin() + ( (i+1)*10) );
	};
	rng RX ( merge_vector4 ( Rin, Rout , Vin, Vout , comp) );
	assert ( RX.size() == 100 );

	for ( uint32_t i =0 ; i < Y.size() ; ++i)
	{	assert ( Y[ i] == i  );
	};

};


void prueba6 ( void)
{	//---------------------------------- begin -----------------------------
	typedef typename std::vector <xk>::iterator iter_t ;
	typedef range<iter_t >                      rng ;
	typedef std::less <xk>                      compare ;

	std::vector<xk> VA, VB ;
	VB.resize ( 160);
	compare comp ;
	for ( uint32_t i =0 ; i < 80 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( 2*k+1 , i /10);
	};
	for ( uint32_t i =0 ; i < 80 ; ++i)
	{	uint32_t k = i % 10 ;
		VA.emplace_back ( 2*k , i /10);
	};
	std::vector <rng > Vin, Vout ;
	for ( uint32_t i =0 ; i < 16 ; ++i)
	{	Vin.emplace_back (VA.begin() + (i*10), VA.begin() + ( (i+1)*10) );
	};
	rng RA ( VA.begin() , VA.end() );
	rng RB ( VB.begin() , VB.end() );

	rng RX ( merge_vector4 ( RA, RB , Vin, Vout , comp) );
	assert ( RX.size() == 160 );

	for ( uint32_t i =0 ; i < VB.size() ; ++i)
	{	uint32_t K = i /8;
		assert ( VB[i].num == K and VB[i].tail == (i%8) );
	};
};

int main( int, char*[] )
{   //-------------- begin------------
    prueba1() ;
    prueba2() ;
    prueba3() ;
    prueba4() ;
    prueba5() ;
    prueba6() ;
    return 0 ;
};
