//----------------------------------------------------------------------------
/// @file test_spinlock.cpp
/// @brief Test program of the class spinlock
///
/// @author Copyright (c) 2010 2012 Francisco Jose Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <tools/atomic.hpp>
#include <iostream>


int main( int, char*[] )
{   //----------------- begin ------------
	std::atomic<uint32_t> A ( 7) ;
	
	std::atomic_uint B  ( 6 ) ;
	auto X = std::atomic_load_explicit<uint32_t> (&A, std::memory_order_acquire );
	//auto X = std::atomic_load (&A );
    std::cout<<X<<std::endl ;
    X = tools::atomic_read <uint32_t> ( A);
    std::cout<<X<<std::endl ;
    return 0;
};


