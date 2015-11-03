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
#include <sort/util/half_merge.hpp>
#include <vector>
#include <algorithm>

using namespace _sort::util ;


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
//                          TEST OF HALF_MERGE
//****************************************************************************
void prueba1 ()
{   //-------------------------- begin ---------------------------------------
	typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<uint64_t>  compare ;

    compare   comp ;
    std::vector<uint64_t> A , B;
    rng RA (A.begin(), A.end() ), RB ( B.begin() ,B.end());
    rng Rx ( A.begin(), A.end() );
    rng Rz (Rx);

    //------------------------------------------------------------------------
    // src1 empty
    //------------------------------------------------------------------------
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.clear() ;
    RB = rng ( B.begin() , B.end());
    RA = rng ( A.begin() , A.end());
    Rx = RA ;
    Rz = half_merge ( Rx, RB,RA, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;
    //std::cout<<Rz ;

    //------------------------------------------------------------------------
    // src2 empty
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back( i);
    A.resize (10 , 0);
    Rz = rng ( A.begin() , A.end());
    RB = rng ( B.begin() , B.end());
    RA = rng ( A.end(), A.end());
    Rz = half_merge ( Rx, RB,RA, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;
    //std::cout<<Rz ;

    //------------------------------------------------------------------------
    // merged even , odd numbers
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 20 ; i+=2) B.push_back( i);
    assert ( B.size() == 10 );
    A.resize (10,0);
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);
    assert ( A.size() == 20 ) ;
    RA = rng(A.begin()+10, A.end() );
    RB = rng( B.begin() ,B.begin()+10);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge ( Rx, RB, RA,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src1 0-10 in src2 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 10 ; ++i)B.push_back(i );
    A.resize (10,0);
    for ( uint32_t i =10  ; i < 20 ; i++) A.push_back( i);

    RA = rng(A.begin()+10, A.end() );
    RB = rng( B.begin() ,B.begin()+10);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge ( Rx, RB, RA,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src2 0-10 in src1 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =10 ; i < 20 ; ++i)B.push_back(i );
    A.resize (10,0);
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);

    RA = rng(A.begin()+10, A.end() );
    RB = rng( B.begin() ,B.begin()+10);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge ( Rx, RB, RA,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( A[i] == i) ;

};
//
//****************************************************************************
//                          TEST OF HALF_MERGE_BIG
//****************************************************************************
void prueba2 ()
{   //-------------------------- begin ---------------------------------------
	typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<uint64_t>  compare ;

    compare   comp ;
    std::vector<uint64_t> A , B;
    rng RA (A.begin(), A.end() ), RB ( B.begin() ,B.end());
    rng Rx ( A.begin(), A.end() );
    rng Rz (Rx);

    //------------------------------------------------------------------------
    // src1 empty
    //------------------------------------------------------------------------
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.clear() ;
    RB = rng ( B.begin() , B.end());
    RA = rng ( A.begin() , A.end());
    Rx = RA ;
    Rz = half_merge_big ( Rx, RB,RA, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;
    //std::cout<<Rz ;

    //------------------------------------------------------------------------
    // src2 empty
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back( i);
    A.resize (10 , 0);
    Rz = rng ( A.begin() , A.end());
    RB = rng ( B.begin() , B.end());
    RA = rng ( A.end(), A.end());
    Rz = half_merge_big ( Rx, RB,RA, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;
    //std::cout<<Rz ;

    //------------------------------------------------------------------------
    // merged even , odd numbers
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 200 ; i+=2) B.push_back( i);
    assert ( B.size() == 100 );
    A.resize (100,0);
    for ( uint32_t i =1  ; i < 200 ; i+=2) A.push_back( i);
    assert ( A.size() == 200 ) ;
    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge_big ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    for ( uint32_t i = 0  ; i < 200 ; ++i)
        assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src1 0-10 in src2 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 100 ; ++i)B.push_back(i );
    A.resize (100,0);
    for ( uint32_t i =100  ; i < 200 ; i++) A.push_back( i);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    for ( uint32_t i = 0  ; i < 200 ; ++i)
        assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src2 0-10 in src1 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =100 ; i < 200 ; ++i)B.push_back(i );
    A.resize (100,0);
    for ( uint32_t i =0  ; i < 100 ; i++) A.push_back( i);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    for ( uint32_t i = 0  ; i < 200 ; ++i)
        assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)B.push_back(i );
    for ( uint32_t i =0 ; i < 50 ; ++i)B.push_back(100+ i*2 );
    A.resize ( 100, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) A.push_back( 101+ i*2);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge_big ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    //std::cout<<Rz ;

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src2
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    A.resize ( 100, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.push_back(i );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.push_back(100+ i*2 );

    for ( uint32_t i =0  ; i < 100 ; i++) B.push_back( 101+ i*2);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge_big ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    //std::cout<<Rz ;

};
//
//****************************************************************************
//                          TEST OF STABILITY
//****************************************************************************
void prueba3()
{	//---------------------------------- begin ------------------------------
	typedef typename std::vector<xk>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<xk>  compare ;
    compare   comp ;
    std::vector<xk> A , B;
    rng RA (A.begin(), A.end() ), RB ( B.begin() ,B.end());
    rng Rx ( A.begin(), A.end() );
    rng Rz (Rx);

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)B.emplace_back(i, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)B.emplace_back(100+ i*2, 0 );
    A.resize ( 100, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,1);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    std::cout<<Rz<<std::endl ;

    for( uint32_t i =0 ; i < 50 ; ++i)
    	assert ( A[i].num == i and A[i].tail == 0);
    for ( uint32_t i = 50 ; i < 150 ; ++i)
    {	uint32_t K = i +50 ;
    	uint32_t M = K %2 ;
    	assert ( A[i].num == K -M  and A[i].tail == M);
    };
    for ( uint32_t i = 150 ; i < 200 ; ++i)
    	assert ( A[i]. num == 2*i -100 and A[i].tail == 1);

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src2
    //------------------------------------------------------------------------

    A.clear() ;
    B.clear() ;
    A.resize ( 100, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i,1 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2,1 );

    for ( uint32_t i =0  ; i < 100 ; i++) B.emplace_back( 100+ i*2,0);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    std::cout<<Rz<<std::endl ;

    for( uint32_t i =0 ; i < 50 ; ++i)
    	assert ( A[i].num == i and A[i].tail == 1);
    for ( uint32_t i = 50 ; i < 150 ; ++i)
    {	uint32_t K = i +50 ;
    	uint32_t M = K %2 ;
    	assert ( A[i].num == K -M  and A[i].tail == M);
    };
    for ( uint32_t i = 150 ; i < 200 ; ++i)
    	assert ( A[i]. num == 2*i -100 and A[i].tail == 0);
}
//
//****************************************************************************
//                          TEST OF STABILITY
//****************************************************************************
void prueba4()
{	//---------------------------------- begin ------------------------------
	typedef typename std::vector<xk>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<xk>  compare ;
    compare   comp ;
    std::vector<xk> A , B;
    rng RA (A.begin(), A.end() ), RB ( B.begin() ,B.end());
    rng Rx ( A.begin(), A.end() );
    rng Rz (Rx);

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)B.emplace_back(i, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)B.emplace_back(100+ i*2, 0 );
    A.resize ( 100, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,1);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge_big ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    std::cout<<Rz<<std::endl ;

    for( uint32_t i =0 ; i < 50 ; ++i)
    	assert ( A[i].num == i and A[i].tail == 0);
    for ( uint32_t i = 50 ; i < 150 ; ++i)
    {	uint32_t K = i +50 ;
    	uint32_t M = K %2 ;
    	assert ( A[i].num == K -M  and A[i].tail == M);
    };
    for ( uint32_t i = 150 ; i < 200 ; ++i)
    	assert ( A[i]. num == 2*i -100 and A[i].tail == 1);

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src2
    //------------------------------------------------------------------------

    A.clear() ;
    B.clear() ;
    A.resize ( 100, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i,1 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2,1 );

    for ( uint32_t i =0  ; i < 100 ; i++) B.emplace_back( 100+ i*2,0);

    RA = rng(A.begin()+100, A.end() );
    RB = rng( B.begin() ,B.begin()+100);
    Rx = rng ( A.begin(), A.end() );
    Rz = half_merge_big ( Rx, RB, RA,comp);
    assert ( Rz.size() == 200 ) ;
    std::cout<<Rz<<std::endl ;

    for( uint32_t i =0 ; i < 50 ; ++i)
    	assert ( A[i].num == i and A[i].tail == 1);
    for ( uint32_t i = 50 ; i < 150 ; ++i)
    {	uint32_t K = i +50 ;
    	uint32_t M = K %2 ;
    	assert ( A[i].num == K -M  and A[i].tail == M);
    };
    for ( uint32_t i = 150 ; i < 200 ; ++i)
    	assert ( A[i]. num == 2*i -100 and A[i].tail == 0);
}
//
//****************************************************************************
//      TEST OF HALF_MERGE_BACK
//****************************************************************************
/*
void prueba2 ()
{   //---------------------------- begin -------------------------------------
    typedef std::less<uint64_t> compare ;
    std::vector<uint64_t> A , B;
    A.reserve ( 20);
    B.reserve ( 20);

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 20 ; i+=2) B.push_back( i);
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);
    A.resize (20,0);
    bs_util::half_merge_back (&A[0] , &A[10],&B[0] , &B[10],  &A[19], compare());
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        if ( A[i]!= i) std::cout<<"error "<<i<<"\n";

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 10 ; ++i)B.push_back(i+10 );
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    A.resize (20,0);
    bs_util::half_merge_back ( &A[0] , &A[10],&B[0] , &B[10], &A[19] , compare());
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        if ( A[i]!= i) std::cout<<"error "<<i<<"\n";

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back( i);
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( 10+i);
    A.resize (20,0);
    bs_util::half_merge_back ( &A[0] , &A[10],&B[0] , &B[10], &A[19] , compare());
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        if ( A[i]!= i) std::cout<<"error "<<i<<"\n";

};
*/
int main( int, char*[] )
{   //-------------- begin------------
    prueba1() ;
    prueba2() ;
    prueba3() ;
    prueba4() ;
    return 0 ;
};
