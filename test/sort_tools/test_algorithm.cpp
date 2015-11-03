//----------------------------------------------------------------------------
/// @file test_algorithms.cpp
/// @brief Test program of several routines containes in the files
///        algorithms32.hpp and algorithms64.hpp
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

#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <cassert>
#include <hpx/parallel/sort/tools/algorithm.hpp>
#define NELEM 1000000

using namespace hpx::parallel::_sort::tools;

void prueba32 ( void)
{   //------------------------------- begin ----------------------------

    //------------------------------------------------------------------
    // Test LS1B_32 ( Least Significant 1 Bit )
    // Test MS1B_32 ( Most  Significant 1 Bit )
    //-----------------------------------------------------------------
    assert ( LS1B_32 (0x5324) == 2 );
    assert ( LS1B_32 (0x5320U) == 5 );
    assert ( LS1B_32 (0x80000000U) == 31);
    assert ( LS1B_32 (0x80001000U) == 12);


    assert ( MS1B_32 (0x5324) == 14);
    assert ( MS1B_32 (0x80000000U) == 31);
    assert ( MS1B_32 (1U) == 0);

    //--------------------------------------------------------------------
    // NBits
    //--------------------------------------------------------------------
    assert ( NBits32 (63) ==6);
    assert ( NBits32 (64) ==7);
    assert ( NBits32 (65) ==7);
    assert ( NBits (63) ==6);
    assert ( NBits (64) ==7);
    assert ( NBits (65) ==7);

    //------------------------------------------------------------------
    // Test LS1B_64 ( Least Significant 1 Bit )
    // Test MS1B_64 ( Most  Significant 1 Bit )
    //-----------------------------------------------------------------

    assert ( MS1B_32 (0x5324) == 14);
    assert ( MS1B_32 (0x80000000U) == 31);
    assert ( MS1B_32 (1U) == 0);

    //std::cout<<BitScanReverse32 (0x5324) <<std::endl ;
    assert ( BitScanReverse32 (0x5324) == 14);
    assert ( BitScanReverse32 (0x80000000U) == 31);
    assert ( BitScanReverse32 (1U) == 0);

};
void prueba64 ( void)
{   //------------------------------- begin ----------------------------

    //------------------------------------------------------------------
    // Test LS1B_64 ( Least Significant 1 Bit )
    // Test MS1B_64 ( Most  Significant 1 Bit )
    //-----------------------------------------------------------------
    assert ( LS1B_64 (0x5324ULL) == 2 );
    assert ( LS1B_64 (0x5320ULL) == 5 );
    assert ( LS1B_64 (0x80000000ULL) == 31);
    assert ( LS1B_64 (0x80001000ULL) == 12);

    assert ( MS1B_64 (0x5324) == 14);
    assert ( MS1B_64 (0x80000000U) == 31);
    assert ( MS1B_64 (1U) == 0);

    //--------------------------------------------------------------------
    // NBits
    //--------------------------------------------------------------------
    assert ( NBits64 (63) ==6);
    assert ( NBits64 (64) ==7);
    assert ( NBits64 (65) ==7);
    assert ( NBits (63) ==6);
    assert ( NBits (64) ==7);
    assert ( NBits (65) ==7);

};


int main(int argc, char* argv[])
{
    // We force this example to use 2 threads by default as one of the threads
    // will be sitting most of the time in the kernel waiting for user input.
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=2");

    // Initialize and run HPX.
    return hpx::init(argc, argv, cfg);

}
int hpx_main(boost::program_options::variables_map&)
{
    {
        prueba32 ();
        prueba64 ();
    }

    // Initiate shutdown of the runtime systems on all localities.
    return hpx::finalize();
}

