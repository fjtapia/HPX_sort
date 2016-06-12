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
#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>

#include <iostream>
//#include <thread>
//#include <future>
//#include <mutex>

#include <hpx/parallel/sort/tools/deque_cnc.hpp>

namespace hpx_tools = hpx::parallel::sort::tools ;
using std::cout ;
using std::endl;
using hpx_tools::deque_cnc ;


#define NELEM 5000000

static const uint32_t NC = std::thread::hardware_concurrency() ;
static const uint32_t NCores = (NC >8) ? 8 : NC ;
deque_cnc<int> S;

void function1() ;
void function2() ;
void prueba_cnc ( void);
void prueba ( void);



int main(int argc, char* argv[])
{
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=all");

    // Initialize and run HPX.
    return hpx::init(argc, argv, cfg);

}
int hpx_main(boost::program_options::variables_map&)
{	{	prueba_cnc ();
    };
    // Initiate shutdown of the runtime systems on all localities.
    return hpx::finalize();
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

    hpx::future<void>  T [8] ;

    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i] = hpx::async ( function1 ) ;
    };
    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i].get();
    };
;
    //duracion =  subtract_time(finish , start) ;
    //cout<<"Time spent :"<<duracion<<" seconds\n";
    //cout<<"The size of S is :"<<S.size()<<endl;

    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i] = hpx::async ( function2 ) ;
    };
    for (uint32_t i =0 ; i < NCores ; ++i)
    {   T[i].get();
    };

    //duracion =  subtract_time(finish , start) ;
    //cout<<"Time spent :"<<duracion<<" seconds\n";
    //cout<<"The size of S is :"<<S.size()<<endl;

};

