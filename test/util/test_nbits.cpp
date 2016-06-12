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

#include <hpx/parallel/sort/tools/nbits.hpp>
#include <cassert>
using namespace hpx::parallel::sort::tools;

int main( int, char*[] )
{    //--------------------------------------------------------------------
    // NBits
    //--------------------------------------------------------------------
    assert ( NBits32 (63UL) ==6);
    assert ( NBits32 (64UL) ==7);
    assert ( NBits32 (65UL) ==7);
    //--------------------------------------------------------------------
    // NBits
    //--------------------------------------------------------------------
    assert ( NBits64 (63ULL) ==6);
    assert ( NBits64 (64ULL) ==7);
    assert ( NBits64 (65ULL) ==7);
    return 0;
};
