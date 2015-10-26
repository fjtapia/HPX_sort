//----------------------------------------------------------------------------
/// @file test_file_vector.cpp
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
#include <iostream>
#include <tools/file_vector.hpp>
#include <vector>

#include <algorithm>

namespace bs_util = tools ;

void prueba1 ();
void prueba2 ();
void prueba3 ();
void prueba4 ();
#define NELEM 100
//
//****************************************************************************
//  TEST MOVE, CONSTRUCT, UNINITIALIZED_MOVE, DESTROY
//****************************************************************************
void prueba1 ( void )
{   //-------------------- test of move --------------------------------------
    std::cout<<"FILE CREATION\n";
    if ( bs_util::generate_file ("input.bin",NELEM) != 0)
        std::cout<<"Error in the file creation\n" ;

    std::cout<<"VECTOR<UINT64> FILL FROM FILE\n";
    std::vector<uint64_t> A ;
    if ( bs_util::fill_vector_uint64 ("input.bin", A, NELEM) != 0)
        std::cout<<"Error in the load of the vector from the file\n" ;

    std::cout<<"FILE FILL FROM VECTOR<UINT64>\n";
    if ( bs_util::write_file_uint64 ( A,"output.bin") != 0)
        std::cout<<"Error in the load of the file from the vector\n" ;
};
//
//****************************************************************************
//      TEST OF FULL_MERGE
//****************************************************************************
void prueba2 ()
{   //---------------------------- begin -------------------------------------
    std::cout<<"VECTOR<STRING> FILL FROM FILE\n";
    std::vector<std::string> A ;
    if ( bs_util::fill_vector_string ("input.bin", A, NELEM/10) != 0)
        std::cout<<"Error in the load of the vector from the file\n" ;
    std::cout<<"FILE FILL FROM VECTOR<STRING>\n";
    bs_util::write_file_string ( A,"output.bin")  ;


};
//
//****************************************************************************
//      TEST OF HALF_MERGE
//****************************************************************************
void prueba3 ()
{   //---------------------------- begin -------------------------------------
    std::cout<<"FILE LOAD ELEMENT TO ELEMENT\n";
    bs_util::uint64_file_generator input ("input.bin");
    std::vector<uint64_t> A ;
    A.reserve ( NELEM);
    if ( input.size() < NELEM)
    {   std::cout<<"Error in the file size\n";
        return  ;
    };
    for ( size_t i =0 ; i < NELEM; ++i)
        A.push_back ( input.get());
    input.reset() ;
    for ( uint32_t i =0 ; i < 10 ; ++i)
        std::cout <<input()<<" " ;
    std::cout<<std::endl ;
};

//
//****************************************************************************
//      TEST OF UNINITIALIZED_FULL_MERGE
//****************************************************************************
void prueba4 ()
{   //---------------------------- begin -------------------------------------

};

int main ()
{   //-------------- begin------------
    prueba1() ;
    prueba2() ;
    prueba3() ;
    prueba4() ;

};
