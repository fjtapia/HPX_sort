//----------------------------------------------------------------------------
/// @file test_range.cpp
/// @brief
///
/// @author Copyright (c) 2010 2015 Francisco Jos� Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <sort/util/in_place_merge.hpp>
#include <vector>
#include <algorithm>

using namespace _sort::util  ;

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
//                    TEST OF in_place_MERGE
//****************************************************************************
void prueba1 ()
{   //---------------------------- begin -------------------------------------
    typedef std::less<uint64_t>  compare ;
    typedef range<uint64_t *> rng ;

    std::vector<uint64_t> A , B;
    compare comp ;

    A.clear() ;
    B.clear() ;
    B.assign ( 20 , 0 );
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);

    rng A1 (& A[0], & A[10]), A2 (&A[10], & A[20] );
    rng B1 ( & B[0], & B[20]);
    rng C1 ( A1);

    C1 = in_place_merge (  A1, A2,B1, comp )  ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)   assert ( A[i] == i);

    A.clear() ;
    B.clear() ;
    B.assign ( 20 , 100 );
    for ( uint32_t i =0  ; i < 20 ; i++) A.push_back (i);

    in_place_merge ( rng( &A[0],&A[10]) ,rng(&A[10],&A[20]),B1,comp)  ;
    for ( uint32_t i = 0  ; i < 20 ; ++i) assert ( A[i] == i);

    A.clear() ;
    B.clear() ;
    B.assign ( 20 , 100 );

    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back (10+i);
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back(i);

    in_place_merge ( rng( &A[0],&A[10]),rng(&A[10],&A[20]),B1,comp );
    for ( uint32_t i = 0  ; i < 20 ; ++i)    assert ( A[i] == i) ;

};

//
//****************************************************************************
//                          TEST OF in_place_MERGE
//****************************************************************************
void prueba2 ()
{   //-------------------------- begin ---------------------------------------
	typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<uint64_t>  compare ;

    compare   comp ;
    std::vector<uint64_t> A , B;
    rng RA1 (A.begin(), A.end() ),RA2 (A.begin(), A.end() );
    rng RB ( B.begin() ,B.end());
    rng Rz (RB);

    //------------------------------------------------------------------------
    // src1 empty
    //------------------------------------------------------------------------
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.clear() ;
    B.resize ( 20 , 100);

    RB = rng ( B.begin() , B.end());
    RA1 = rng ( A.begin() , A.begin());
    RA2 = rng ( A.begin() , A.end());

    Rz = in_place_merge (  RA1,RA2,RB, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;


    //------------------------------------------------------------------------
    // src2 empty
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.resize (10 , 0);

    RA1 = rng ( A.begin() , A.end());
    RA2 = rng ( A.end(), A.end() );
    RB = rng ( B.begin() , B.end());

    Rz = in_place_merge (  RA1,RA2,RB, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;

    //------------------------------------------------------------------------
    // merged even , odd numbers
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    assert ( A.size() == 10 );
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);
    assert ( A.size() == 20 );
    B.resize (20,0);

    RA1 = rng( A.begin() ,A.begin()+10);
    RA2 = rng(A.begin()+10, A.end() );
    RB  = rng ( B.begin() , B.end());

    Rz = in_place_merge (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)  assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src1 0-10 in src2 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 20 ; ++i)A.push_back(i );
    B.resize (20,0);

    RA1 = rng( A.begin() ,A.begin()+10);
    RA2 = rng(A.begin()+10, A.end() );
    RB  = rng ( B.begin() , B.end());

    Rz = in_place_merge (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)   assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src2 0-10 in src1 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =10 ; i < 20 ; ++i)A.push_back(i );
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.resize (20,0);

    RA1 = rng( A.begin() ,A.begin()+10);
    RA2 = rng(A.begin()+10, A.end() );
    RB  = rng ( B.begin() , B.end());

    Rz = in_place_merge (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)    assert ( A[i] == i) ;

};
//
//****************************************************************************
//                          TEST OF in_place_MERGE_BIG
//****************************************************************************

void prueba3 ()
{   //-------------------------- begin ---------------------------------------
	typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<uint64_t>  compare ;

    compare   comp ;
    std::vector<uint64_t> A , B;
    rng RA1 (A.begin(), A.end() ),RA2 (A.begin(), A.end() );
    rng RB ( B.begin() ,B.end());
    rng Rz (RB);

    //------------------------------------------------------------------------
    // src1 empty
    //------------------------------------------------------------------------
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.clear() ;
    B.resize ( 20 , 100);

    RB = rng ( B.begin() , B.end());
    RA1 = rng ( A.begin() , A.begin());
    RA2 = rng ( A.begin() , A.end());

    Rz = in_place_merge_big (  RA1,RA2,RB, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;

    //------------------------------------------------------------------------
    // src2 empty
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.resize (10 , 0);

    RA1 = rng ( A.begin() , A.end());
    RA2 = rng ( A.end(), A.end() );
    RB = rng ( B.begin() , B.end());

    Rz = in_place_merge_big (  RA1,RA2,RB, comp);
    assert ( Rz.size() == 10);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( * (Rz.first + i) == i) ;

    //------------------------------------------------------------------------
    // merged even , odd numbers
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    assert ( A.size() == 10 );
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);
    assert ( A.size() == 20 );
    B.resize (20,0);

    RA1 = rng( A.begin() ,A.begin()+10);
    RA2 = rng(A.begin()+10, A.end() );
    RB  = rng ( B.begin() , B.end());

    Rz = in_place_merge_big (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i) assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src1 0-10 in src2 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 20 ; ++i)A.push_back(i );
    B.resize (20,0);

    RA1 = rng( A.begin() ,A.begin()+10);
    RA2 = rng(A.begin()+10, A.end() );
    RB  = rng ( B.begin() , B.end());

    Rz = in_place_merge_big (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)  assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src2 0-10 in src1 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =10 ; i < 20 ; ++i)A.push_back(i );
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.resize (20,0);

    RA1 = rng( A.begin() ,A.begin()+10);
    RA2 = rng(A.begin()+10, A.end() );
    RB  = rng ( B.begin() , B.end());

    Rz = in_place_merge_big (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)  assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)A.push_back(i );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.push_back(100+ i*2 );

    for ( uint32_t i =0  ; i < 100 ; i++) A.push_back( 101+ i*2);
    B.resize ( 200, 0 );

    RA1 = rng( A.begin() ,A.begin()+100);
    RA2 = rng(A.begin()+100, A.end() );

    RB = rng ( B.begin(), B.end() );
    Rz = in_place_merge_big (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 200 ) ;

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src2
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)A.push_back(i );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.push_back(100+ i*2 );

    for ( uint32_t i =0  ; i < 100 ; i++) A.push_back( 101+ i*2);
    B.resize ( 200, 0 );

    RA1 = rng( A.begin() ,A.begin()+100);
    RA2 = rng(A.begin()+100, A.end() );

    RB = rng ( B.begin(), B.end() );
    Rz = in_place_merge_big (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 200 ) ;
};

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
    rng RA1 (A.begin(), A.end() ), RA2 (A.begin(), A.end() );
    rng RB ( B.begin() ,B.end());
    rng Rz (RB);

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,1);
    B.resize ( 200, 0 );

    RA1 = rng( A.begin() ,A.begin()+100);
    RA2 = rng(A.begin()+100, A.end() );
    RB = rng ( B.begin() , B.end());

    Rz = in_place_merge (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 200 ) ;

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
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,0);
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i,1 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2,1 );
    B.resize ( 200, 0 );

    RA1 = rng( A.begin() ,A.begin()+100);
    RA2 = rng(A.begin()+100, A.end() );
    RB = rng ( B.begin() , B.end());

    Rz = in_place_merge ( RA1, RA2, RB,comp);
    assert ( Rz.size() == 200 ) ;

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
void prueba5()
{	//---------------------------------- begin ------------------------------
	typedef typename std::vector<xk>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<xk>  compare ;
    compare   comp ;
    std::vector<xk> A , B;
    rng RA1 (A.begin(), A.end() ), RA2 (A.begin(), A.end() );
    rng RB ( B.begin() ,B.end());
    rng Rz (RB);

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,1);
    B.resize ( 200, 0 );

    RA1 = rng( A.begin() ,A.begin()+100);
    RA2 = rng(A.begin()+100, A.end() );
    RB = rng ( B.begin() , B.end());

    Rz = in_place_merge_big (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 200 ) ;

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
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,0);
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i,1 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2,1 );
    B.resize ( 200, 0 );

    RA1 = rng( A.begin() ,A.begin()+100);
    RA2 = rng(A.begin()+100, A.end() );
    RB = rng ( B.begin() , B.end());

    Rz = in_place_merge_big (  RA1, RA2,RB,comp);
    assert ( Rz.size() == 200 ) ;

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
//                    TEST OF IN_PLACE_MERGE_UNCONTIGUOUS
//****************************************************************************
void prueba6 ()
{   //---------------------------- begin -------------------------------------
    typedef std::less<uint64_t>  compare ;
    typedef range<uint64_t *> rng ;
    bool SW ;

    std::vector<uint64_t> A , B,C;
    compare comp ;

    A.clear() ;
    B.clear() ;
    C.clear() ;
    C.assign ( 10 , 0 );
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    for ( uint32_t i =1  ; i < 20 ; i+=2) B.push_back( i);

    rng A1 (& A[0], & A[10]);
    rng B1 ( & B[0], & B[10]);
    rng C1 ( & C[0], & C[10]);
    rng D1 ( A1 );

    SW = in_place_merge_uncontiguous (  A1, B1,C1 ,comp )  ;
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)   assert ( A[i] == i);
    for ( uint32_t i = 10  ; i < 20 ; ++i)   assert ( B[i-10] == i);

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back (i);
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back (i + 10);

    SW = in_place_merge_uncontiguous ( rng( &A[0],&A[10]) ,rng(&B[0],&B[100]),C1,comp)  ;
    assert ( SW == true );
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( A[i] == i);
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( B[i] == i+10);

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back (10+i);
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back(i);

    SW = in_place_merge_uncontiguous ( rng( &A[0],&A[10]) ,rng(&B[0],&B[10]),C1,comp)  ;
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( A[i] == i);
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( B[i] == i+10);

};

//
//****************************************************************************
//                          TEST OF in_place_MERGE
//****************************************************************************
void prueba7 ()
{   //-------------------------- begin ---------------------------------------
	typedef typename std::vector<uint64_t>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<uint64_t>  compare ;

    compare   comp ;
    std::vector<uint64_t> A , B, C;
    bool SW ;

    rng RA ( A.begin() , A.end() );
    rng RB ( B.begin() , B.end() );
    rng RC ( C.begin() , C.end() );

    //------------------------------------------------------------------------
    // src1 empty
    //------------------------------------------------------------------------
    A.clear();
    B.clear() ;
    C.clear() ;

    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back( i);
    C.resize ( 10 , 0);

    RA = rng ( A.begin() , A.end() );
    RB = rng ( B.begin() , B.end() );
    RC = rng ( C.begin() , C.end() );

    SW = in_place_merge_uncontiguous ( RA,RB,RC, comp);
    assert ( SW == true ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)   assert ( B[i]== i) ;


    //------------------------------------------------------------------------
    // src2 empty
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    C.resize ( 10 , 0);
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);

    RA = rng ( A.begin() , A.end() );
    RB = rng ( B.begin() , B.end() );
    RC = rng ( C.begin() , C.end() );

    SW = in_place_merge_uncontiguous ( RA,RB,RC, comp);
    assert ( SW == true ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // merged even , odd numbers
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    assert ( A.size() == 10 );
    for ( uint32_t i =1  ; i < 20 ; i+=2) B.push_back( i);
    assert ( B.size() == 10 );

    RA = rng ( A.begin() , A.end() );
    RB = rng ( B.begin() , B.end() );
    RC = rng ( C.begin() , C.end() );

    SW = in_place_merge_uncontiguous ( RA,RB,RC, comp);
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( A[i] == i) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( B[i] == i+10) ;


    //------------------------------------------------------------------------
    // in src1 0-10 in src2 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 10 ; ++i)A.push_back(i );
    for ( uint32_t i =10 ; i < 20 ; ++i)B.push_back(i );

    RA = rng ( A.begin() , A.end() );
    RB = rng ( B.begin() , B.end() );
    RC = rng ( C.begin() , C.end() );

    SW = in_place_merge_uncontiguous ( RA,RB,RC, comp);
    assert ( SW == true ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( A[i] == i) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( B[i] == i+10) ;

    //------------------------------------------------------------------------
    // in src2 0-10 in src1 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 10 ; ++i) B.push_back(i );
    for ( uint32_t i =10  ; i < 20 ; i++) A.push_back( i);
    RA = rng ( A.begin() , A.end() );
    RB = rng ( B.begin() , B.end() );
    RC = rng ( C.begin() , C.end() );
    SW = in_place_merge_uncontiguous ( RA,RB,RC, comp);
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( A[i] == i) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( B[i] == i+10) ;

};

//
//****************************************************************************
//                          TEST OF STABILITY
//****************************************************************************

void prueba8()
{	//---------------------------------- begin ------------------------------
	typedef typename std::vector<xk>::iterator iter_t ;
    typedef range<iter_t> rng ;
    typedef std::less<xk>  compare ;
    compare   comp ;
    std::vector<xk> A , B , C;
    bool SW ;
    rng RA ( A.begin() ,A.end() );
    rng RB ( B.begin() ,B.end() );
    rng RC ( C.begin() ,C.end() );


    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) B.emplace_back( 100+ i*2,1);
    C.resize ( 100, 0 );

    RA = rng ( A.begin() ,A.end() );
    RB = rng ( B.begin() ,B.end() );
    RC = rng ( C.begin() ,C.end() );

    SW = in_place_merge_uncontiguous ( RA, RB,RC,comp);
    assert ( SW == false ) ;

    for( uint32_t i =0 ; i < 50 ; ++i)
    	assert ( A[i].num == i and A[i].tail == 0);
    for ( uint32_t i = 50 ; i < 100 ; ++i)
    {	uint32_t K = i +50 ;
    	uint32_t M = K %2 ;
    	assert ( A[i].num == K -M  and A[i].tail == M);
    };

    for ( uint32_t i = 0 ; i < 50 ; ++i)
    {	uint32_t K = i +150 ;
    	uint32_t M = K %2 ;
    	assert ( B[i].num == K-M  and B[i].tail == M);
    };


    for ( uint32_t i = 50 ; i < 100 ; ++i)
    	assert ( B[i]. num == 2*i+100 and B[i].tail == 1);

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src2
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,0);
    for ( uint32_t i =0 ; i < 50 ; ++i)B.emplace_back(i,1 );
    for ( uint32_t i =0 ; i < 50 ; ++i)B.emplace_back(100+ i*2,1 );

    RA = rng ( A.begin() ,A.end() );
    RB = rng ( B.begin() ,B.end() );
    RC = rng ( C.begin() ,C.end() );

    SW = in_place_merge_uncontiguous ( RA, RB,RC,comp);
    assert ( SW == false ) ;

    for( uint32_t i =0 ; i < 50 ; ++i)
    	assert ( A[i].num == i and A[i].tail == 1);
    for ( uint32_t i = 50 ; i < 100 ; ++i)
    {	uint32_t K = i +50 ;
    	uint32_t M = K %2 ;
    	assert ( A[i].num == K -M  and A[i].tail == M);
    };

    for ( uint32_t i = 0 ; i < 50 ; ++i)
    {	uint32_t K = i +150 ;
    	uint32_t M = K %2 ;
    	assert ( B[i].num == K-M  and B[i].tail == M);
    };


    for ( uint32_t i = 50 ; i < 100 ; ++i)
    	assert ( B[i]. num == 2*i+100 and B[i].tail == 0);

}
void prueba9()
{	//--------------------------- begin ----------------------------------
	typedef typename std::vector<uint32_t>::iterator iter_t ;
	std::vector<uint32_t> A ={2,3,4,6,10,11,12,13};
	std::vector<uint32_t> B = {5,7,8,9}, C ( 8 , 0);
	range<iter_t> RA ( A.begin() , A.end()), RB ( B.begin() , B.end());
	range<iter_t> RC ( C.begin() , C.end()) ;
    bool SW = in_place_merge_uncontiguous ( RA, RB,RC,std::less<uint32_t> ());
    std::cout<<SW<<std::endl ;
    for ( uint32_t i = 0 ; i < A.size() ; ++i)
    	std::cout<<A[i]<<" ";
    for ( uint32_t i = 0 ; i < B.size() ; ++i)
    	std::cout<<B[i]<<" ";

}

int main( int, char*[] )
{   //-------------- begin------------
    prueba1() ;
    prueba2() ;
    prueba3() ;
    prueba4() ;
    prueba5() ;
    prueba6() ;
    prueba7() ;
    prueba8() ;
    prueba9() ;

    return 0 ;
};
