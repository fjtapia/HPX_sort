//----------------------------------------------------------------------------
/// @file   test_circular.cppTest program of the circular class
///
/// @author Copyright (c) 2010 2013 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------

#include <iostream>
#include <boost/countertree/util/circular.hpp>

using namespace boost::countertree::util ;


int main ( void)
{
    circular<uint32_t> C(4);

    for ( uint32_t i =0 ; i < 16 ; ++i)
        C.push_back( i);

    for ( uint32_t i =0 ; i < C.size() ; ++i)
        std::cout<<C[i]<<" ";
    std::cout<<std::endl ;
    return 0 ;
};
