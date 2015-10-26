//----------------------------------------------------------------------------
/// @file test_sample_sort.cpp
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
#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <hpx/parallel/sort/algorithm/sample_sort.hpp>


namespace hpx_algo = hpx::parallel::sort::algorithm;
namespace hpx_tools = hpx::parallel::sort::tools;
namespace hpx_util = hpx::parallel::sort::util ;
using hpx_tools::NThread ;
using hpx_util::range ;
typedef typename std::vector<uint64_t>::iterator iter_t ;

std::mt19937_64 my_rand(0);

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
std::ostream & operator <<( std::ostream & salida, const xk &s)
{   salida<<"["<<s.num<<" : "<<s.tail<<"] ";
    return salida ;
};

void prueba3 ( )
{   //---------------------------------- begin ---------------------------
    std::mt19937_64 my_rand(0);

    const uint32_t NMAX = 5000000 ;

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
    hpx_algo::sample_sort ( V1.begin() , V1.end());
    hpx_algo::indirect_sample_sort ( V3.begin() , V3.end());
    std::stable_sort( V2.begin() , V2.end() );

    //------------------------------------------------------------------------
    //            Comprobation
    //------------------------------------------------------------------------
    if ( V1.size() != V2.size()) std::cout<<"Error in the size\n";
    for ( uint32_t i = 0 ; i < V1.size() ; ++i)
    {   assert ( V1[i].num == V2[i].num and V1[i].tail == V2[i].tail);
    };

    //------------------------------------------------------------------------
    //            Comprobation
    //------------------------------------------------------------------------
    if ( V3.size() != V2.size()) std::cout<<"Error in the size\n";
    for ( uint32_t i = 0 ; i < V3.size() ; ++i)
    {   assert ( V3[i].num == V2[i].num and V3[i].tail == V2[i].tail);
    };
};

void prueba4(void)
{   //----------------------------- begin-------------------------------------
    const uint32_t NElem = 5000000 ;
    std::vector<uint64_t>  V1 ;
    std::mt19937_64 my_rand(0);

    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back(my_rand() %NElem) ;
    //std::cout<<"sample_sort unsorted ---------------\n";
    hpx_algo::sample_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] <= V1[i] ) ;
    };

    V1.clear() ;
    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back (i );
    //std::cout<<"sample_sort sorted ---------------\n";
    hpx_algo::sample_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] <= V1[i] ) ;
    };

    V1.clear() ;
    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back( NElem-i) ;
    //std::cout<<"sample_sort reverse sorted ---------------\n";
    hpx_algo::sample_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] <= V1[i] )  ;
    };

    V1.clear() ;
    for ( uint32_t i =0 ; i < NElem ; ++i) V1.push_back( 1000) ;
    //std::cout<<"sample_sort equals ---------------\n";
    hpx_algo::sample_sort (  V1.begin() , V1.end() );
    for ( unsigned i = 1 ; i < NElem ; i ++ )
    {	assert ( V1[i-1] == V1[i] ) ;
    };
};

void prueba5 ( void)
{   //---------------------------------------- begin--------------------------
    const uint32_t KMax = 5000000 ;

    std::vector<uint64_t > K,M ;
    std::mt19937_64 my_rand(0);
    std::less<uint64_t>  comp ;

    for (uint32_t i =0 ; i < KMax ; ++i ) K.push_back( my_rand());
    M = K ;
    //std::cout<<"sample_sort - random elements ---------------\n";
    uint64_t * Ptr = std::get_temporary_buffer<uint64_t>(KMax ).first ;
    if ( Ptr == nullptr) throw std::bad_alloc() ;
    range<uint64_t*> Rbuf ( Ptr , Ptr + KMax);

    hpx_algo::sample_sort_tag <iter_t,std::less<uint64_t> >(range<iter_t>(K.begin(),K.end()),
                        std::less<uint64_t>(), NThread(), Rbuf);
    //hpx_algo::sample_sort(K.begin(),K.end());


    std::return_temporary_buffer ( Ptr) ;

    std::stable_sort ( M.begin() , M.end() , comp);
    for ( unsigned i = 0 ; i < KMax ; i ++ )
    	assert ( M[i] == K[i] )  ;
};

void prueba6 ( void)
{   //------------------------------------ begin -----------------------------
    std::vector <uint64_t > V ;

    for ( uint32_t i =0 ; i < 2083333 ; ++i)  V.push_back ( i);
    hpx_algo::sample_sort ( V.begin() , V.end(), std::less<uint64_t> ());
    for ( uint32_t i =0 ; i < V.size() ; ++i)
    {   assert ( V[i] == i) ;
    };
};

void prueba7 ( void)
{   //---------------------- begin ------------------------------------
    typedef typename std::vector<uint64_t>::iterator iter_t ;
    const uint32_t NELEM = 416667 ;
    //const uint32_t N1 = ( NELEM +1)/2 ;
    std::vector <uint64_t> A ;

    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 0);
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);
    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 0);

    hpx_algo::sample_sort ( A.begin() + 1000, A.begin() + (1000+NELEM));

    for ( iter_t it =A.begin() + 1000 ; it != A.begin() + (1000+NELEM) ; ++it)
    {   assert ( (*(it-1)) <= (*it)) ;
    };
    //if (A[998] != 0 or A[999] != 0 or A[1000+NELEM] != 0 or A[1001+NELEM] != 0)
    //    std::cout<< "Out of the data memory\n";
    assert(A[998] == 0 and A[999] == 0 and A[1000+NELEM] == 0 and A[1001+NELEM] == 0);

    //------------------------------------------------------------------------
    A.clear() ;
    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 999999999);
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);
    for ( uint32_t i =0 ; i < 1000 ; ++i) A.push_back ( 999999999);

    hpx_algo::sample_sort ( A.begin() + 1000, A.begin() + (1000+NELEM));

    for ( iter_t it =A.begin() + 1001 ; it != A.begin() + (1000+NELEM) ; ++it)
    {   assert ( (*(it-1)) <= (*it));
    };
    assert (A[998] == 999999999 and A[999] == 999999999 and
        A[1000+NELEM] == 999999999 and A[1001+NELEM] == 999999999);


    //------------------------------------------------------------------------
    std::vector<uint64_t> B ( NELEM + 2000 , 0);

    A.clear() ;
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);
    range<iter_t> R ( A.begin() , A.end());
    range<uint64_t*> Rbuf (&B[1000], (&B[1000] )+NELEM );
    hpx_algo::sample_sort_tag<iter_t,std::less<uint64_t> > (range<iter_t>( A.begin() , A.end() ),
                    std::less<uint64_t>(), NThread(),Rbuf);

    for ( iter_t it =A.begin() +1 ; it != A.end() ; ++it)
    {   assert ( (*(it-1)) <= (*it));
    };
    assert (B[998] == 0 and B[999] == 0 and B[1000+NELEM] == 0 and B[1001+NELEM] == 0);

    //------------------------------------------------------------------------
    for ( uint32_t i =0 ; i < B.size() ; ++i)B[i] = 999999999 ;
    A.clear() ;
    for ( uint32_t i =0 ; i < NELEM ; ++i) A.push_back ( NELEM - i);

    hpx_algo::sample_sort_tag<iter_t,std::less<uint64_t> > (range<iter_t>( A.begin(), A.end()),
                    std::less<uint64_t>(), NThread(), Rbuf);

    for ( iter_t it =A.begin() +1 ; it != A.end() ; ++it)
    {   if ( (*(it-1)) > (*it)) std::cout<<"error 2\n";
    };
    assert (B[998] == 999999999 and B[999] == 999999999 and
        B[1000+NELEM] == 999999999 and B[1001+NELEM] == 999999999);

    //------------------------------------------------------------------------
};


int main(int argc, char* argv[])
{
    // We force this example to use 2 threads by default as one of the threads
    // will be sitting most of the time in the kernel waiting for user input.
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=4");

    // Initialize and run HPX.
    return hpx::init(argc, argv, cfg);
    //return hpx::init(argc, argv);


}
int hpx_main(boost::program_options::variables_map&)
{
    {
    	prueba3() ;
    	prueba4() ;
    	prueba5() ;
    	prueba6() ;
    	prueba7() ;
    }

    // Initiate shutdown of the runtime systems on all localities.
    return hpx::finalize();
}
