//----------------------------------------------------------------------------
/// @file test_block_indirect_sort.cpp
/// @brief Test program of the block_indirect_sort algorithm
///
/// @author Copyright (c) 2016 Francisco Jose Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <algorithm>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <ciso646>
#include <hpx/parallel/algorithms/tr1/detail/block_indirect_sort.hpp>

namespace hpx_par = hpx::parallel::tr1::detail;
using hpx_par::blk_detail::block_indirect_sort;
using hpx_par::util::range;



void test1 (void)
{
    std::vector< uint32_t > VN;
    std::mt19937 my_rand (0);
    std::less<uint32_t> comp;
    for (uint32_t i = 0; i < 130; ++i) {
        VN.push_back (my_rand ( ) % 1000);
    };
    block_indirect_sort< 4, 4, uint32_t * > (&VN[ 0 ], &VN[ 130 ],comp, 2);
    for (uint32_t i = 1; i < VN.size ( ); ++i)
        assert (VN[ i - 1 ] <= VN[ i ]);

    VN.clear ( );
    for (uint32_t i = 0; i < 130; ++i) {
        VN.push_back (my_rand ( ) % 1000);
    };
    block_indirect_sort< 4, 4, uint32_t * > (&VN[ 0 ], &VN[ 130 ],comp, 0);
    for (uint32_t i = 1; i < VN.size ( ); ++i)
        assert (VN[ i - 1 ] <= VN[ i ]);
};

void test2 ( )
{
    uint64_t V1[ 300 ];
    std::less< uint64_t > comp;

    for (uint32_t i = 0; i < 200; ++i) V1[ i ] = i;

    block_indirect_sort< 4, 4, uint64_t *, std::less< uint64_t > > (
        &V1[ 0 ], &V1[ 200 ], comp, 2);
    for (unsigned i = 1; i < 200; i++) {
        assert (V1[ i - 1 ] <= V1[ i ]);
    };

    for (uint32_t i = 0; i < 200; ++i) V1[ i ] = 199 - i;

    block_indirect_sort< 4, 4, uint64_t *, std::less< uint64_t > > (
        &V1[ 0 ], &V1[ 200 ], comp, 2);

    for (unsigned i = 1; i < 200; i++) {
        assert (V1[ i - 1 ] <= V1[ i ]);
    };

    for (uint32_t i = 0; i < 300; ++i) V1[ i ] = 299 - i;

    block_indirect_sort< 4, 4, uint64_t *, std::less< uint64_t > > (
        &V1[ 0 ], &V1[ 300 ], comp, 2);
    for (unsigned i = 1; i < 300; i++) {
        assert (V1[ i - 1 ] <= V1[ i ]);
    };

    for (uint32_t i = 0; i < 300; ++i) V1[ i ] = 88;

    block_indirect_sort< 4, 4, uint64_t *, std::less< uint64_t > > (
        &V1[ 0 ], &V1[ 300 ], comp, 2);
    for (unsigned i = 1; i < 300; i++) {
        assert (V1[ i - 1 ] <= V1[ i ]);
    };
    return;
};

void test3 (void)
{
    typedef typename std::vector< uint64_t >::iterator iter_t;
    typedef std::less< uint64_t > compare;

    const uint32_t NElem = 500000;
    std::vector< uint64_t > V1;
    std::mt19937_64 my_rand (0);
    compare comp;

    for (uint32_t i = 0; i < NElem; ++i) V1.push_back (my_rand ( ) % NElem);
    block_indirect_sort< 1024, 16, iter_t, compare >
        (V1.begin ( ), V1.end ( ), comp, 2);

    for (unsigned i = 1; i < NElem; i++) {
        assert (V1[ i - 1 ] <= V1[ i ]);
    };

    V1.clear ( );
    for (uint32_t i = 0; i < NElem; ++i) V1.push_back (i);

    block_indirect_sort< 1024, 16, iter_t, compare > (
        V1.begin ( ), V1.end ( ), comp, 2);
    for (unsigned i = 1; i < NElem; i++) {
        assert (V1[ i - 1 ] <= V1[ i ]);
    };

    V1.clear ( );
    for (uint32_t i = 0; i < NElem; ++i) V1.push_back (NElem - i);

    block_indirect_sort< 1024, 16, iter_t, compare > (
        V1.begin ( ), V1.end ( ), comp, 2);
    for (unsigned i = 1; i < NElem; i++) {
        assert (V1[ i - 1 ] <= V1[ i ]);
    };

    V1.clear ( );
    for (uint32_t i = 0; i < NElem; ++i) V1.push_back (1000);

    block_indirect_sort< 1024, 16, iter_t, compare > (
        V1.begin ( ), V1.end ( ), comp, 2);
    for (unsigned i = 1; i < NElem; i++) {
        assert (V1[ i - 1 ] == V1[ i ]);
    };
};

void test4 (void)
{
    typedef typename std::vector< uint64_t >::iterator iter_t;
    typedef std::less< uint64_t > compare;
    std::less< uint64_t > comp;
    std::vector< uint64_t > V;

    for (uint32_t i = 0; i < 2083333; ++i) V.push_back (i);

    block_indirect_sort< 1024, 32, iter_t, compare > (
        V.begin ( ), V.end ( ),comp, 8);
    for (uint32_t i = 0; i < V.size ( ); ++i) {
        assert (V[ i ] == i);
    };
};

void test5 (void)
{
    typedef typename std::vector< uint64_t >::iterator iter_t;
    typedef std::less< uint64_t > compare;
    typedef range< iter_t > range_it;

    const uint32_t NELEM = 416667;
    std::vector< uint64_t > A;
    std::less< uint64_t > comp;

    for (uint32_t i = 0; i < 1000; ++i) A.push_back (0);
    for (uint32_t i = 0; i < NELEM; ++i) A.push_back (NELEM - i);
    for (uint32_t i = 0; i < 1000; ++i) A.push_back (0);

    range_it R1 (A.begin ( ) + 1000, A.begin ( ) + (1000 + NELEM));
    block_indirect_sort< 1024, 32, iter_t, compare > (
        A.begin ( ) + 1000, A.begin ( ) + (1000 + NELEM), comp, 8);
    for (iter_t it = A.begin ( ) + 1000; it != A.begin ( ) + (1000 + NELEM);
         ++it)
    {
        assert ((*(it - 1)) <= (*it));
    };
    assert (A[ 998 ] == 0 and A[ 999 ] == 0 and A[ 1000 + NELEM ] == 0 and
                 A[ 1001 + NELEM ] == 0);


    A.clear ( );
    for (uint32_t i = 0; i < 1000; ++i) A.push_back (999999999);
    for (uint32_t i = 0; i < NELEM; ++i) A.push_back (NELEM - i);
    for (uint32_t i = 0; i < 1000; ++i) A.push_back (999999999);

    R1 = range_it (A.begin ( ) + 1000, A.begin ( ) + (1000 + NELEM));
    block_indirect_sort< 1024, 32, iter_t, compare > (
        A.begin ( ) + 1000, A.begin ( ) + (1000 + NELEM), comp, 4);

    for (iter_t it = A.begin ( ) + 1001; it != A.begin ( ) + (1000 + NELEM);
         ++it)
    {
        assert ((*(it - 1)) <= (*it));
    };
    assert (A[ 998 ] == 999999999 and A[ 999 ] == 999999999 and
                 A[ 1000 + NELEM ] == 999999999 and
                 A[ 1001 + NELEM ] == 999999999);
};

void test6 (void)
{
    typedef typename std::vector< uint32_t >::iterator iter_t;
    typedef std::less< uint32_t > compare;

    const uint32_t NElem = 1000000;
    std::vector< uint32_t > V1, V2, V3;
    V1.reserve ( NElem ) ;
    std::mt19937 my_rand (0);

    for (uint32_t i = 0; i < NElem; ++i) V1.push_back (my_rand ( ));

    V2 = V1;
    V3 = V1;
    std::sort (V2.begin ( ), V2.end ( ));

    V1 = V3;
    block_indirect_sort< 1024, 32, iter_t, compare > (
        V1.begin ( ), V1.end ( ), compare(), 2);
    for (unsigned i = 0; i < V1.size(); i++)
    {
        assert (V1[ i ] == V2[ i ]);
    };

    V1 = V3;
    block_indirect_sort< 1024, 32, iter_t, compare > (
        V1.begin ( ), V1.end ( ), compare(), 4);
    for (unsigned i = 0; i < V1.size(); i++)
    {
        assert (V1[ i ] == V2[ i ]);
    };

    V1 = V3;

    block_indirect_sort< 1024, 32, iter_t, compare > (
        V1.begin ( ), V1.end ( ), compare(), 8);

    for (uint32_t i = 1 ; i < V1.size() ; ++i)
    {
        assert ( V1[i] >= V1[i-1]);
    }

    for (unsigned i = 0; i < V1.size(); i++)
    {
        assert (V1[ i ] == V2[ i ]);
    };

    V1 = V3;
    block_indirect_sort< 1024, 32, iter_t, compare > (
        V1.begin ( ), V1.end ( ), compare(), 16);
    for (unsigned i = 0; i < V1.size(); i++) {
        assert (V1[ i ] == V2[ i ]);
    };

    V1 = V3;
    block_indirect_sort< 1024, 32, iter_t, compare > (
        V1.begin ( ), V1.end ( ), compare(), 100);
    for (unsigned i = 1; i < V1.size(); i++) {
        assert (V1[ i ] == V2[ i ]);
    };

};

void test7 ( )
{
    typedef typename std::vector< uint64_t >::iterator iter_t;
    typedef std::less< uint64_t > compare;

    const uint32_t NELEM = 10000000;
    std::mt19937_64 my_rand (0);
    std::vector< uint64_t > A, B;
    A.reserve (NELEM);
    compare comp;

    for (uint32_t i = 0; i < NELEM; i++) {
        A.push_back (my_rand ( ));
    };
    B = A;

    block_indirect_sort< 1024, 32, iter_t, compare >
    ( A.begin ( ), A.end ( ), comp, 8);
    for (unsigned i = 0; i < (A.size ( ) - 1); i++) {
        assert (A[ i ] <= A[ i + 1 ]);
    };
    std::sort (B.begin ( ), B.end ( ));
    for (unsigned i = 0; i < NELEM; i++) {
        assert (A[ i ] == B[ i ]);
    };
};



int test_main (void)
{
    test1  ( );
    test2  ( );
    test3  ( );
    test4  ( );
    test5  ( );
    test6  ( );
    test7  ( );

    return 0;
};


int main(int argc, char* argv[])
{
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=all");

    // Initialize and run HPX.
    return hpx::init(argc, argv, cfg);

}
int hpx_main(boost::program_options::variables_map&)
{	{	test_main();
    };
    // Initiate shutdown of the runtime systems on all localities.
    return hpx::finalize();
};
