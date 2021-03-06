//----------------------------------------------------------------------------
/// @file test_spin_sort.cpp
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
#define __DEBUG_SORT 1
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <hpx/parallel/sort/detail/spin_sort.hpp>
#include <vector>
#include <algorithm>

namespace hpx_det = hpx::parallel::sort::detail;
using hpx_det::util::range ;
using hpx_det::util::compare_iter ;


template <class iter_t, class compare= compare_iter<iter_t> >
void spin_sort (iter_t first, iter_t last , compare comp= compare()  )
{	hpx_det::spin_sort<iter_t,compare>(first, last,comp);
};


void prueba2 ();
void prueba3 ();
void prueba4 ();
void prueba5 ();
void prueba6 ();
void prueba7 ();


struct xk
{   unsigned tail : 3 ;
    unsigned num  :24 ;
    bool operator < ( xk A) const
    {   return ( num < A.num);
    };
};
//std::ostream & operator <<( std::ostream & salida, const xk &s)
//{   salida<<"["<<s.num<<" : "<<s.tail<<"] ";
//    return salida ;
//};

void prueba2 ()
{   //---------------------------- Inicio --------------------------------
    uint64_t  V1[300] ;
    std::less<uint64_t> comp ;

    for ( uint32_t i =0 ; i < 200 ; ++i) V1[i]= i ;
    //std::cout<<"Smart Merge Sort Sorted, not repeated ---------------\n";
    spin_sort ( &V1[0] , &V1[200], comp );
    for ( unsigned i = 1 ; i < 200 ; i ++ )
    {	assert ( V1[i-1] <= V1[i] ) ;
    };

    for ( uint32_t i =0 ; i < 200 ; ++i) V1[i] = 199-i ;
    //std::cout<<"Smart Merge Sort Reverse sorted , not repeated ---------------\n";
    spin_sort ( &V1[0] , &V1[200], comp );
    for ( unsigned i = 1 ; i < 200 ; i ++ )
    {	assert ( V1[i-1] <= V1[i] ) ;
    };

    for ( uint32_t i =0 ; i < 300 ; ++i) V1[i] = 299-i ;
    //std::cout<<"Smart Merge Sort Reverse sorted , not repeated ---------------\n";
    spin_sort ( &V1[0] , &V1[300], comp );
    for ( unsigned i = 1 ; i < 300 ; i ++ )
    {	assert ( V1[i-1] <= V1[i] )  ;
    };

    for ( uint32_t i =0 ; i < 300 ; ++i) V1[i] = 88 ;
    //std::cout<<"Smart Merge Sort Equals ---------------\n";
    spin_sort( &V1[0] , &V1[300],comp );
    for ( unsigned i = 1 ; i < 300 ; i ++ )
    {	assert ( V1[i-1] <= V1[i] ) ;
    };
};

void prueba3 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937_64 my_rand(0);

    const uint32_t NMAX = 500000 ;

    //------------------------------------------------------------------------
    std::vector< xk> V1, V2, V3;
    V1.reserve ( NMAX);
    for ( uint32_t i = 0 ; i < 8 ; ++i)
    {   for ( uint32_t k =0 ; k < NMAX; ++k)
        {   uint32_t NM = my_rand() ;
            xk G ;
            G.num = NM >> 3 ;
            G.tail = i ;
            V1.push_back(G);
        };
    };
    V3 =V2 = V1 ;
    spin_sort ( V1.begin() , V1.end());
    //indirect_spin_sort ( V3.begin() , V3.end());
    std::stable_sort( V2.begin() , V2.end() );

    //------------------------------------------------------------------------
    //            Comprobation
    //------------------------------------------------------------------------
    assert ( V1.size() == V2.size()) ;
    for ( uint32_t i = 0 ; i < V1.size() ; ++i)
    {   assert ( V1[i].num == V2[i].num and V1[i].tail == V2[i].tail);
    };
/*
    //------------------------------------------------------------------------
    //            Comprobation
    //------------------------------------------------------------------------
    assert ( V3.size() == V2.size()) ;
    for ( uint32_t i = 0 ; i < V3.size() ; ++i)
    {   assert ( V3[i].num == V2[i].num and V3[i].tail == V2[i].tail);
    };
*/
};

void prueba4(void)
{   //----------------------------- begin-------------------------------------
    const uint32_t NElem = 500000 ;
    std::vector<uint64_t>  V1 ;
    std::mt19937_64 my_rand(0);

    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back(my_rand() %NElem) ;
    //std::cout<<"spin_sort unsorted ---------------\n";
    spin_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] <= V1[i] )  ;
    };

    V1.clear() ;
    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back (i );
    //std::cout<<"spin_sort sorted ---------------\n";
    spin_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] <= V1[i] ) ;
    };

    V1.clear() ;
    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back( NElem-i) ;
    //std::cout<<"spin_sort reverse sorted ---------------\n";
    spin_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] <= V1[i] ) ;
    };

    V1.clear() ;
    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back( 1000) ;
    //std::cout<<"spin_sort equals ---------------\n";
    spin_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] == V1[i] ) ;
    };
};

void prueba5 ( void)
{   //---------------------------------------- begin--------------------------
    typedef std::vector<uint64_t >::iterator iter_t ;
    typedef std::less<uint64_t>					compare ;
    const uint32_t KMax = 500000 ;
    typedef std::vector<uint64_t >::iterator iter_t ;
    std::vector<uint64_t > K,M ;
    std::mt19937_64 my_rand(0);
    compare  comp ;

    for (uint32_t i =0 ; i < KMax ; ++i ) K.push_back( my_rand());
    M = K ;
    //std::cout<<"spin_sort - random elements ---------------\n";
    uint64_t * Ptr = std::get_temporary_buffer<uint64_t>(KMax >>1).first ;
    if ( Ptr == nullptr) throw std::bad_alloc() ;
    range<uint64_t*> Rbuf (Ptr , Ptr + (KMax >>1) );
    //range<iter_t> RK ( K.begin(),K.end());
    hpx_det::spin_sort<iter_t,compare> (K.begin(),K.end(), comp ,Rbuf);

    std::return_temporary_buffer ( Ptr) ;

    std::stable_sort ( M.begin() , M.end() , comp);
    for ( unsigned i = 0 ; i < KMax ; i ++ )
    	assert ( M[i] == K[i] );
};

void prueba6 ( void)
{   //------------------------------------ begin -----------------------------
    std::vector <uint64_t > V ;

    for ( uint32_t i =0 ; i < 2083333 ; ++i)  V.push_back ( i);
    spin_sort ( V.begin() , V.end(), std::less<uint64_t> ());
    for ( uint32_t i =0 ; i < V.size() ; ++i)
    {   assert ( V[i] == i)  ;
    };
};

void prueba7 ( void)
{   //---------------------- begin ------------------------------------
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    const uint32_t NELEM = 416667 ;
    const uint32_t N1 = ( NELEM +1)/2 ;
    std::vector <uint64_t> A ;

    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 0);
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);
    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 0);

    spin_sort ( A.begin() + 1000, A.begin() + (1000+NELEM));

    for ( iter_t it =A.begin() + 1000 ; it != A.begin() + (1000+NELEM) ; ++it)
    {   assert ( (*(it-1)) <= (*it)) ;
    };
    assert (A[998] == 0 and A[999] == 0 and A[1000+NELEM] == 0 and A[1001+NELEM] == 0);

    //------------------------------------------------------------------------
    A.clear() ;
    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 999999999);
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);
    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 999999999);

    spin_sort ( A.begin() + 1000, A.begin() + (1000+NELEM));

    for ( iter_t it =A.begin() + 1001 ; it != A.begin() + (1000+NELEM) ; ++it)
    {   assert ( (*(it-1)) <= (*it));
    };
    assert (A[998] == 999999999 and A[999] == 999999999 and
        A[1000+NELEM] == 999999999 and A[1001+NELEM] == 999999999);

    //------------------------------------------------------------------------
    std::vector<uint64_t> B ( N1 + 2000 , 0);

    A.clear() ;
    range<uint64_t*> Rbuf ( &B[1000], (&B[1000])+ N1  );
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);
    hpx_det::spin_sort<iter_t, std::less<uint64_t>> ( A.begin(), A.end(), std::less<uint64_t>(), Rbuf);
    for ( iter_t it =A.begin() +1 ; it != A.end() ; ++it)
    {   if ( (*(it-1)) > (*it))  std::cout<<"error 1\n";
    };
    assert (B[998] == 0 and B[999] == 0 and B[1000+N1] == 0 and B[1001+N1] == 0);

    for ( uint32_t i =0 ; i < B.size() ; ++i)B[i] = 999999999 ;
    A.clear() ;
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);
    hpx_det::spin_sort<iter_t, std::less<uint64_t>> ( A.begin(), A.end(), std::less<uint64_t>(), Rbuf);

    for ( iter_t it =A.begin() +1 ; it != A.end() ; ++it)
    {   assert ( (*(it-1)) <= (*it)) ;
    };
    assert (B[998] == 999999999 and B[999] == 999999999 and
        B[1000+N1] == 999999999 and B[1001+N1] == 999999999);
};

int main( int, char*[] )
{   //-------------- begin------------
    prueba2() ;
    prueba3() ;
    prueba4() ;
    prueba5() ;
    prueba6() ;
    prueba7() ;
    return 0 ;
};



