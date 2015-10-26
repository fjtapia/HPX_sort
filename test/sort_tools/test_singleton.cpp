//----------------------------------------------------------------------------
/// @file bjam_mysingleton.cpp
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
#define __DEBUG_CNTREE 1
#include <cntree/util/singleton.hpp>
#include <cassert>
#include <vector>

#include <iostream>

//---------------------------------------------------------------------------
//              GLOBAL VARIABLES
//---------------------------------------------------------------------------
struct chapuza
{   int N ;
    ~chapuza () { N =0 ;};
};
//---------------------------------------------------------------------------
//  singleton
//---------------------------------------------------------------------------
struct glb_chapuza
{
    static chapuza & ch  ;

    glb_chapuza ( ) {}
    ~glb_chapuza ()
    {   assert ( ch.N == 17); // This can't be assert because fail
    };
};

chapuza & glb_chapuza::ch = cntree::util::singleton <chapuza>::instance()  ;

static glb_chapuza &gch = cntree::util::singleton <glb_chapuza>::instance() ;

//---------------------------------------------------------------------------
//  singleton_cnc
//---------------------------------------------------------------------------
struct glb_chapuza_cnc
{
    static chapuza & ch  ;

    glb_chapuza_cnc ( ) {}
    ~glb_chapuza_cnc ()
    {   assert ( ch.N == 17); // This can't be assert because fail
    };
};

chapuza & glb_chapuza_cnc::ch = cntree::util::singleton_cnc <chapuza>::instance()  ;

static glb_chapuza_cnc &gch_cnc = cntree::util::singleton_cnc <glb_chapuza_cnc>::instance() ;

//---------------------------------------------------------------------------
//
//               F U N C I O N E S
//
//---------------------------------------------------------------------------

void prb_singleton( void)
{   //---------------------- begin ---------------------
    gch.ch.N = 17 ;
    chapuza & ch1 ( cntree::util::singleton <chapuza>::instance());
    assert ( ch1.N == 17);
    //glb_chapuza & gch1 ( cntree::util::singleton <glb_chapuza>::instance() );
    glb_chapuza gch1 ;
    assert ( gch1.ch.N == 17);
    gch1.ch = ch1 ;
    assert ( gch1.ch.N == 17);
    {   chapuza & ch2 ( cntree::util::singleton <chapuza>::instance());
        assert ( ch1.N == 17);
        ch1 = ch2 ;
    }
    assert ( ch1.N == 17);
    assert ( gch1.ch.N == 17);
}

void prb_singleton_cnc ( void)
{   //---------------------- begin ---------------------
    gch_cnc.ch.N = 17 ;
    chapuza & ch1 ( cntree::util::singleton_cnc <chapuza>::instance());
    assert ( ch1.N == 17);
    //glb_chapuza & gch1 ( cntree::util::mysingleton <glb_chapuza, false>::instance() );
    glb_chapuza gch1 ;
    assert ( gch1.ch.N == 17);
    gch1.ch = ch1 ;
    assert ( gch1.ch.N == 17);
    {   chapuza & ch2 ( cntree::util::singleton_cnc <chapuza>::instance());
        assert ( ch1.N == 17);
        ch1 = ch2 ;
    }
    assert ( ch1.N == 17);
    assert ( gch1.ch.N == 17);
}


void prb_thread ( int N)
{   //---------------------- begin ---------------------
    cntree::util::singleton_thread<std::vector<int> > V1 ;
    assert ( V1->size() == 0 );

    for ( int i =0 ; i < 1000 ; ++i)
        V1->push_back (N+i);

    cntree::util::singleton_thread<std::vector<int> > V2 ;
    for ( int i =0 ; i < 1000 ; ++i)
        (*V2)[i]+= 1000;

    cntree::util::singleton_thread<std::vector<int> > V3 ;
    for ( int i =0 ; i < 1000 ; ++i)
        assert ( V1()[i] == V3()[i] and V1()[i] == (i + N + 1000 ));
}
void  prb_singleton_thread_local()
{
    std::thread T1 ( prb_thread,1000);
    std::thread T2 ( prb_thread,2000);
    std::thread T3 ( prb_thread,3000);
    std::thread T4 ( prb_thread,4000);
    prb_thread (9000) ;

    T1.join() ;
    T2.join() ;
    T3.join() ;
    T4.join() ;

}

//--------------------------------------------------------------------------
//test functions of the singleton_thread with the thread_local
// format oc C++11
//---------------------------------------------------------------------------
void prueba ( int N)
{   //---------------------- begin ---------------------
    std::vector<int> *V1, *V2 , *V3 ;
    V1 = & cntree::util::singleton_thread_local<std::vector<int> >::instance() ;
    assert ( V1->size() == 0 );

    for ( int i =0 ; i < 1000 ; ++i)
        V1->push_back (N+i);

    V2 = & cntree::util::singleton_thread_local<std::vector<int> >::instance() ;
    for ( int i =0 ; i < 1000 ; ++i)
        (*V2)[i]+= 1000;

    V3 = & cntree::util::singleton_thread_local<std::vector<int> >::instance() ;
    assert ( V1 == V2 and V2 == V3);
    for ( int i =0 ; i < 1000 ; ++i)
        assert ( (*V1)[i] == (*V3)[i] and (*V1)[i] == (i + N + 1000 ));
    //std::cout<<V1<<std::endl;
}
void  start()
{
    std::thread T1 ( prueba,1000);
    std::thread T2 ( prueba,2000);
    std::thread T3 ( prueba,3000);
    std::thread T4 ( prueba,4000);
    prueba (9000) ;

    T1.join() ;
    T2.join() ;
    T3.join() ;
    T4.join() ;

}



int main (int argc, char* argv[] )
{
    prb_singleton ();
    prb_singleton_cnc ();
    prb_singleton_thread_local();
    start() ;
    return 0;
}
