//----------------------------------------------------------------------------
/// @file test_sort.cpp
/// @brief Benchmark of several sort methods with different uint64_t numbers
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
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
#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

#include <hpx/parallel/sort/sort.hpp>

#define NELEM 10000000
using namespace std ;
namespace hpx_sort = hpx::parallel::sort ;

int test_main ( )
{   //------------------------------ Inicio ----------------------------------
    std::mt19937 my_rand(0);
    vector<uint64_t> A, B, C ;
    B.reserve ( NELEM);

    for ( uint32_t i =0 ; i< NELEM ; ++i) B.push_back ( my_rand());
    C = B ;
    std::sort( C.begin() , C.end());
    //------------------------ Inicio -----------------------------

    A = B ;
    hpx_sort::sort (hpx::parallel::v1::seq,A.begin() , A.end()  );
    assert ( C.size() == A.size());
    for ( uint32_t i =0 ; i < B.size() ; ++i)
    	assert ( A[i] == C[i]);

    A = B ;
    hpx_sort::stable_sort (hpx::parallel::v1::seq,A.begin() , A.end()  );
    assert ( C.size() == A.size());
    for ( uint32_t i =0 ; i < B.size() ; ++i)
    	assert ( A[i] == C[i]);

    A = B ;
    hpx_sort::sort (hpx::parallel::v1::par,A.begin() , A.end()  );
    assert ( C.size() == A.size());
    for ( uint32_t i =0 ; i < B.size() ; ++i)
    	assert ( A[i] == C[i]);

    A = B ;
    hpx_sort::stable_sort (hpx::parallel::v1::par,A.begin() , A.end()  );
    assert ( C.size() == A.size());
    for ( uint32_t i =0 ; i < B.size() ; ++i)
    	assert ( A[i] == C[i]);

    A = B ;
    hpx_sort::sample_sort (hpx::parallel::v1::par,A.begin() , A.end()  );
    assert ( C.size() == A.size());
    for ( uint32_t i =0 ; i < B.size() ; ++i)
    	assert ( A[i] == C[i]);

    return 0 ;
};
int main(int argc, char* argv[])
{
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=all");

    // Initialize and run HPX.
    return hpx::init(argc, argv, cfg);

}
int hpx_main(boost::program_options::variables_map&)
{	{	test_main() ;
    };
    // Initiate shutdown of the runtime systems on all localities.
    return hpx::finalize();
};
