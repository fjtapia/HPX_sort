//----------------------------------------------------------------------------
/// @file test_low_level.cpp
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


#include <hpx/parallel/sort/util/low_level.hpp>
#include <vector>
#include <cassert>

#include <algorithm>

using namespace hpx::parallel::sort::util::lwl ;


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
//  TEST MOVE, CONSTRUCT, UNINITIALIZED_MOVE, DESTROY
//****************************************************************************
void prueba1 ( void )
{   //-------------------- test of move --------------------------------------
    std::vector<uint64_t> A , B;

    A.resize ( 10, 0);
    B.resize ( 10, 0);
    for ( uint32_t i =0 ; i < 10 ; ++i) A[i] = i ;

    init_move( &B[0],A.begin() , A.end() );
    for ( uint32_t i =0 ; i < 10 ; ++i)
        assert ( B[i] == i) ;

    //------------- test of construct , destroy and uninitialized_move -------
    struct forensic
    {   int64_t N ;
        forensic ( uint64_t K = 0 ){ N = (int64_t)K ;};
        ~forensic () { N = -1 ;};
    };
    char K[80];
    forensic *PAux = reinterpret_cast <forensic*> ( &K[0]);

    for ( uint32_t i =0 ; i < 10 ; ++i)
        construct( PAux+i, i);
    for ( uint32_t i = 0 ; i < 10 ; ++i)
    {   assert ( PAux[i].N == i);
    };
    destroy ( PAux , PAux+10);

    //----------------test of uninitialized_move---------------------------
    std::vector <forensic> V ;
    for ( uint32_t i =0 ; i < 10 ; ++i) V.emplace_back ( i);

    uninit_move (PAux, V.begin() , V.end());
    for ( uint32_t i =0 ; i < 10 ; ++i)
        assert ( PAux[i].N == i) ;

    destroy ( PAux , PAux+10);

};

//
//****************************************************************************
//      TEST OF FULL_MERGE
//****************************************************************************
void prueba2 ()
{   //---------------------------- begin -------------------------------------
    typedef std::less<uint64_t> compare ;
    std::vector<uint64_t> A , B;
    //uint64_t A[20],B[20];
    size_t NA =0 ;

    A.clear() ;
    B.clear() ;
    B.assign ( 20 , 0 );
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);

    full_merge ( &A[0], &A[10], &A[10], &A[20], &B[0], compare() )  ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( B[i]== i);

    //NA =0 ;
    for ( uint32_t i =0 ; i < 20 ; ++i)B[i] = 100 ;
    for ( uint32_t i =0  ; i < 20 ; i++) A[i] = i;

    full_merge ( &A[0], &A[10], &A[10], &A[20], &B[0], compare() )  ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( B[i] == i) ;

    NA  = 0 ;
    for ( uint32_t i =0 ; i < 20 ; ++i)B[i] = 100 ;
    for ( uint32_t i =0  ; i < 10 ; i++) A[NA++] = 10+i;
    for ( uint32_t i =0  ; i < 10 ; i++) A[NA++] = i;
    full_merge ( &A[0], &A[10], &A[10], &A[20], &B[0], compare() )  ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( B[i]== i);

};
//
//****************************************************************************
//      TEST OF HALF_MERGE
//****************************************************************************
void prueba3 ()
{   //---------------------------- begin -------------------------------------
    typedef std::less<uint64_t> compare ;
    std::vector<uint64_t> A , B;
    //uint64_t A[20],B[20];

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 20 ; i+=2) B.push_back( i);
    A.resize (10,0);
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);
    half_merge (&B[0] , &B[10], &A[10] , &A[20], &A[0], compare());
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( A[i] == i) ;

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 10 ; ++i)B.push_back(i );
    A.resize (10,0);
    for ( uint32_t i =10  ; i < 20 ; i++) A.push_back( i);
    half_merge (&B[0] , &B[10], &A[10] , &A[20], &A[0] , compare());
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( A[i]== i);

    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back( 10+i);
    A.resize (10,0);
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    half_merge (&B[0] , &B[10], &A[10] , &A[20], &A[0], compare()) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
        assert ( A[i]== i) ;

};

//
//****************************************************************************
//      TEST OF UNINITIALIZED_FULL_MERGE
//****************************************************************************
void prueba5 ()
{   //---------------------------- begin -------------------------------------
    struct forensic
    {   int64_t N ;
        forensic ( uint64_t K = 0 ){ N = (int64_t)K ;};
        ~forensic () { N = -1 ;};
        bool operator < ( const forensic & f)const{ return (N < f.N); };
    };
    char K[1600];
    forensic *PAux = reinterpret_cast <forensic*> ( &K[0]);

    typedef std::less<forensic > compare ;
    std::vector<forensic> A ;

    A.clear() ;
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.emplace_back( i);
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.emplace_back( i);

    uninit_full_merge ( &A[0], &A[10], &A[10], &A[20],
                                        PAux , compare() );
    for ( uint32_t i = 0  ; i < 20 ; ++i)
    {   assert ( PAux[i].N == i);
    };
    destroy ( PAux, PAux + 20);

    //NA =0 ;
    for ( uint32_t i =0  ; i < 20 ; i++) A[i] = i;

    uninit_full_merge ( &A[0], &A[10], &A[10], &A[20],
                                        PAux  , compare() ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)
    {   assert ( PAux[i]. N == i);
    };
    destroy ( PAux, PAux + 20);

    for ( uint32_t i =0  ; i < 10 ; i++) A[i] = 10+i;
    for ( uint32_t i =0  ; i < 10 ; i++) A[10+i] = i;
    uninit_full_merge ( &A[0], &A[10], &A[10],&A[20],
                                        PAux  , compare() );
    for ( uint32_t i = 0  ; i < 20 ; ++i)
    {   assert ( PAux[i]. N == i );
    };
    destroy ( PAux, PAux + 20);
};
//
//****************************************************************************
//                    TEST OF in_place_MERGE
//****************************************************************************
void prueba6 ()
{   //---------------------------- begin -------------------------------------
    typedef std::less<uint64_t>  compare ;
    std::vector<uint64_t> A , B;
    compare comp ;

    A.clear() ;
    B.clear() ;
    B.assign ( 20 , 0 );
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    for ( uint32_t i =1  ; i < 20 ; i+=2) A.push_back( i);


    bool SW = in_place_merge ( & A[0], & A[10],  & A[20],& B[0], comp )  ;
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)   assert ( A[i] == i);

    A.clear() ;
    B.clear() ;
    B.assign ( 20 , 100 );
    for ( uint32_t i =0  ; i < 20 ; i++) A.push_back (i);

    in_place_merge ( &A[0],&A[10] ,&A[20],& B[0],comp)  ;
    for ( uint32_t i = 0  ; i < 20 ; ++i) assert ( A[i] == i);

    A.clear() ;
    B.clear() ;
    B.assign ( 20 , 100 );

    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back (10+i);
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back(i);

    in_place_merge (&A[0],&A[10],&A[20],& B[0],comp );
    for ( uint32_t i = 0  ; i < 20 ; ++i)    assert ( A[i] == i) ;

};

//
//****************************************************************************
//                          TEST OF in_place_MERGE
//****************************************************************************
void prueba7 ()
{   //-------------------------- begin ---------------------------------------
	typedef std::less<uint64_t>  compare ;
    compare   comp ;
    std::vector<uint64_t> A , B;

    //------------------------------------------------------------------------
    // src1 empty
    //------------------------------------------------------------------------
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.clear() ;
    B.resize ( 20 , 100);

    in_place_merge (A.begin() , A.begin(), A.end(), B.begin(), comp);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( A[i] == i ) ;

    //------------------------------------------------------------------------
    // src2 empty
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.resize (10 , 0);

    in_place_merge ( A.begin() , A.end() , A.end(),B.begin(), comp);
    for ( uint32_t i = 0  ; i < 10 ; ++i)
        assert ( A[i] == i );

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

    in_place_merge (A.begin() ,A.begin()+10, A.end(),B.begin() ,comp);
    for ( uint32_t i = 0  ; i < 20 ; ++i)  assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src1 0-10 in src2 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 20 ; ++i)A.push_back(i );
    B.resize (20,0);

    in_place_merge (A.begin() ,A.begin()+10, A.end(),B.begin() ,comp);
    assert ( A.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)   assert ( A[i] == i) ;

    //------------------------------------------------------------------------
    // in src2 0-10 in src1 10-20
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =10 ; i < 20 ; ++i)A.push_back(i );
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    B.resize (20,0);

    in_place_merge (A.begin() ,A.begin()+10, A.end(),B.begin() ,comp);
    assert ( A.size() == 20 ) ;
    for ( uint32_t i = 0  ; i < 20 ; ++i)    assert ( A[i] == i) ;

};
//
//****************************************************************************
//                          TEST OF STABILITY
//****************************************************************************

void prueba8()
{	//---------------------------------- begin ------------------------------
	typedef std::less<xk>  compare ;
    compare   comp ;
    std::vector<xk> A , B;

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) A.emplace_back( 100+ i*2,1);
    B.resize ( 200, 0 );

    in_place_merge (   A.begin() ,A.begin()+100, A.end(),B.begin(),comp);
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

    in_place_merge ( A.begin() ,A.begin()+100, A.end(),  B.begin(),comp);

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
void prueba9 ()
{   //---------------------------- begin -------------------------------------
    typedef std::less<uint64_t>  compare ;
    bool SW ;

    std::vector<uint64_t> A , B,C;
    compare comp ;

    A.clear() ;
    B.clear() ;
    C.clear() ;
    C.assign ( 10 , 0 );
    for ( uint32_t i =0  ; i < 20 ; i+=2) A.push_back( i);
    for ( uint32_t i =1  ; i < 20 ; i+=2) B.push_back( i);

    SW = in_place_merge_uncontiguous (&A[0],&A[10],& B[0],& B[10],&C[0],comp);
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)   assert ( A[i] == i);
    for ( uint32_t i = 10  ; i < 20 ; ++i)   assert ( B[i-10] == i);

    A.clear() ;
    B.clear() ;
    C.clear() ;
    C.assign ( 10 , 0 );
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back (i);
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back (i + 10);

    SW = in_place_merge_uncontiguous (&A[0],&A[10] ,&B[0],&B[10],& C[0],comp)  ;
    assert ( SW == true );
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( A[i] == i);
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( B[i] == i+10);

    A.clear() ;
    B.clear() ;
    C.clear() ;
    C.assign ( 10 , 0 );
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back (10+i);
    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back(i);

    SW = in_place_merge_uncontiguous (&A[0],&A[10] ,&B[0],&B[10],& C[0],comp)  ;
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( A[i] == i);
    for ( uint32_t i = 0  ; i < 10 ; ++i) assert ( B[i] == i+10);

};
//
//****************************************************************************
//                          TEST OF in_place_MERGE
//****************************************************************************
void prueba10 ()
{   //-------------------------- begin ---------------------------------------
	typedef std::less<uint64_t>  compare ;
    compare   comp ;
    std::vector<uint64_t> A , B, C;
    bool SW ;

    //------------------------------------------------------------------------
    // src1 empty
    //------------------------------------------------------------------------
    A.clear();
    B.clear() ;
    C.clear() ;

    for ( uint32_t i =0  ; i < 10 ; i++) B.push_back( i);
    C.resize ( 10 , 0);
    SW = in_place_merge_uncontiguous ( A.begin() , A.end(),
    		                           B.begin() , B.end(),C.begin(), comp);
    assert ( SW == true ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)   assert ( B[i]== i) ;

    //------------------------------------------------------------------------
    // src2 empty
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    C.resize ( 10 , 0);
    for ( uint32_t i =0  ; i < 10 ; i++) A.push_back( i);
    SW = in_place_merge_uncontiguous ( A.begin() , A.end(),
    		                           B.begin() , B.end(),C.begin(), comp);
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
    SW = in_place_merge_uncontiguous ( A.begin() , A.end(),
    		                           B.begin() , B.end(),C.begin(), comp);
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
    SW = in_place_merge_uncontiguous ( A.begin() , A.end(),
    		                           B.begin() , B.end(),C.begin(), comp);
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
    SW = in_place_merge_uncontiguous ( A.begin() , A.end(),
    		                           B.begin() , B.end(),C.begin(), comp);
    assert ( SW == false ) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( A[i] == i) ;
    for ( uint32_t i = 0  ; i < 10 ; ++i)  assert ( B[i] == i+10) ;
};
//
//****************************************************************************
//                          TEST OF STABILITY
//****************************************************************************
void prueba11()
{	//---------------------------------- begin ------------------------------
	typedef std::less<xk>  compare ;
    compare   comp ;
    std::vector<xk> A , B , C;
    bool SW ;

    //------------------------------------------------------------------------
    // the smallest elements at the beginning of src1
    //------------------------------------------------------------------------
    A.clear() ;
    B.clear() ;
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(i, 0 );
    for ( uint32_t i =0 ; i < 50 ; ++i)A.emplace_back(100+ i*2, 0 );
    for ( uint32_t i =0  ; i < 100 ; i++) B.emplace_back( 100+ i*2,1);
    C.resize ( 100, 0 );
    SW = in_place_merge_uncontiguous ( A.begin() ,A.end(),
    		                           B.begin() ,B.end(),C.begin(),comp);
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

    SW = in_place_merge_uncontiguous (A.begin(),A.end(),
    		                          B.begin(),B.end(),C.begin(),comp);
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

void prueba12()
{	//--------------------------- begin ----------------------------------
	std::vector<uint32_t> A ={2,3,4,6,10,11,12,13};
	std::vector<uint32_t> B = {5,7,8,9}, C ( 8 , 0);

    bool SW = in_place_merge_uncontiguous (A.begin(),A.end(),B.begin(),B.end(),
    		                               C.begin() ,std::less<uint32_t> ());
    assert ( SW == false) ;
    for ( uint32_t i = 0 ; i < A.size() ; ++i)
    	assert ( A[i] == i +2) ;
    for ( uint32_t i = 0 ; i < B.size() ; ++i)
    	assert ( B[i] == i +10);
};
int main( int, char*[] )
{   //-------------- begin------------
    prueba1() ;
    prueba2() ;
    prueba3() ;
    prueba5() ;
    prueba6() ;
    prueba7() ;
    prueba8() ;
    prueba9() ;
    prueba10() ;
    prueba11() ;
    prueba12() ;
    return 0 ;

};
