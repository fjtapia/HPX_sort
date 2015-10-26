//----------------------------------------------------------------------------
/// @file test_deque_cnc.cpp
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
#include <iostream>
#include <thread>
#include <chrono>
#include <tools/time_measure.hpp>
#include <mutex>
#include <cassert>
#include <tools/deque_cnc.hpp>

using std::cout ;
using std::endl;
using tools::deque_cnc ;
using tools::time_point ;
using tools::now ;
using tools::subtract_time ;

#define NELEM 5000000

static const uint32_t NC = std::thread::hardware_concurrency() ;
static const uint32_t NCores = (NC >8) ? 8 : NC ;
deque_cnc<int> S;

void function1() ;
void function2() ;
void prueba_cnc ( void);
void prueba ( void);


int main( int, char*[] )
{   //----------------- begin ------------
    //prueba() ;
    prueba_cnc ();
    return 0;
};

void function1()
{   //---------------------------- begin -----------------
    for ( int i =0 ; i < NELEM ; ++i)
        S.push_back(i);
};
void function2()
{   //---------------------------- begin -----------------
    int k ;
    while ( S.pop_copy_back(k) );
};


void prueba_cnc ( void)
{   //--------------------------------- begin ------------------
    //S.reserve ( (int)NELEM * (int)NCores  );
    //double duracion ;
	time_point start, finish;
    std::thread  T [8] ;
    start = now();
    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i] = std::thread ( function1 ) ;
    };
    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i].join();
    };
    finish = now() ;
    //duracion =  subtract_time(finish , start) ;
    //cout<<"Time spent :"<<duracion<<" seconds\n";
    //cout<<"The size of S is :"<<S.size()<<endl;
    start = now();
    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i] = std::thread ( function2 ) ;
    };
    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i].join();
    };
    finish = now() ;
    //duracion =  subtract_time(finish , start) ;
    //cout<<"Time spent :"<<duracion<<" seconds\n";
    //cout<<"The size of S is :"<<S.size()<<endl;

};
/*
void prueba ( void)
{   //-------------------------- begin ----------------------
    deque_cnc<int>  A ;
    std::deque<int> V1 = { 0 , 1 , 2};
    deque_cnc<int>  B (V1) ;
    V1.clear() ;
    assert ( V1.size() == 0);
    B.copy ( V1 ) ;
    for ( int i =0 ; i < 3 ; ++i)
        assert ( V1[i] == i );
    B.clear() ;
    assert ( B.size() == 0 );
    B.push_back ( V1);
    assert ( B.size() == 3);
    B.push_back( 3);
    B.emplace_back( 4);
    assert ( B.size() == 5);
    B.copy ( V1 );
    for ( int i =0 ; i < 5 ; ++i)
        assert ( V1[i] == i );
    V1.clear() ;
    int z ;
    assert ( B.pop_copy_back( z) and z == 4 ) ;
    assert ( B.pop_copy_back ( V1 , 2 ));
    assert ( V1.size() == 2 and V1[0] == 2 and V1[1] == 3);
    assert ( B.size() == 2 );
    B.pop_back() ;
    assert ( B.size() == 1 );
    B.copy ( V1 );
    assert ( V1.size() == 1 and V1[0] == 0);
}
*/
