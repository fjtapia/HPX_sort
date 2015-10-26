//----------------------------------------------------------------------------
/// @file test_vector_ptrc.cpp
/// @brief Test program of the class spinlock
///
/// @author Copyright (c) 2010 2012 Francisco Jose Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See a ccompanying file LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <iostream>
#include <cassert>
#include <boost/countertree/util/vector_ptr.hpp>
#include <boost/countertree/util_sort.hpp>

#include <vector>
#include <algorithm>
#include <boost/countertree/forest/tree.hpp>
#include <boost/countertree/util/time_measure.hpp>
#include <boost/countertree/alloc/suballocator.hpp>

using std::cout ;
using std::endl;

#define NELEM 30000000

using boost::countertree::util::vector_ptr;
using boost::countertree::util::NThread ;

using namespace boost::countertree::forest ;
using namespace boost::countertree::alloc ;
using namespace boost::countertree::util::sort ;

std::vector<uint64_t> A ;

NThread NT ;

int main ()
{   //------------------------- begin -------------------------------------
    // Sorting of vector_ptr of int
    //---------------------------------------------------------------------
    cntree::util::vector_ptr <std::allocator<int> > V;
    V.resize ( NELEM);

    for ( uint32_t i =0 ; i < NELEM ; ++i)
    {   V[i]= V.get_alloc().allocate (1);
        V.get_alloc().construct ( V[i], rand());
    }

    typedef typename cntree::srt::less_ptr<int > less_t ;

    //V.sort< less_t> (V.begin() , V.end(), less_t()  );
    parallel_sort ( V.begin() , V.end(), less_t(),NT );
    for ( uint32_t i =0 ; i < NELEM-1 ; ++i)
    {   if (V[i]== nullptr )
            std::cout<<"Error puntero nullptr\n";
        if ( *V[i] > *V[i+1])
            std::cout<<"Error en la ordenacion \n";
    }

    //----------------------------------------------------------------------
    // sort the nodes in a vector
    //----------------------------------------------------------------------
    typedef node<uint64_t>                              node_t ;
    typedef suballocator<std::allocator <node_t> >      sballoc_t ;
    typedef tree<uint64_t, sballoc_t>                   tree_t ;

    typedef typename vector_ptr<sballoc_t>::iterator    iter_pnode_t ;
    typedef less_node<uint64_t, std::less <uint64_t> >  less_node_t ;


    node_t N1 (7), N2 (5);
    less_node_t L ;
    if ( L ( N1, N2))
        std::cout<<"7" ;
    else std::cout<<"5";
    std::cout<<std::endl;



    double duracion ;
	time_point start, finish, point1, point2, point3, point4, point5;
	for ( uint32_t i=0 ; i < NELEM ; ++i) A.push_back( rand() );
	std::vector<node_t> VN ;
	VN.reserve ( NELEM);

	for ( uint32_t i =0 ; i < NELEM ; ++i)
        VN.emplace_back(A[i] );

    point1 = now() ;

    cntree::parallel_merge_sort<iter_t,less_node_t> ( VN.begin() , VN.end() ,less_node_t(), NThread() )    ;

    point2 = now () ;
    cout<<"point2 :"<<(subtract_time(point2 , point1))<<" seconds\n";

    VN.clear () ;
    VN.shrink_to_fit();
    start = now();

    alloc2_t SB ;

    tree_t  TR1 ;
    vector_ptr<alloc2_t> VPtr ;
    std::vector <pos_alloc64> VPA ;
    point1 = now();
    VPtr.resize ( NELEM);
    for ( uint32_t i =0 ; i < NELEM ; ++i)
    {   node_t * PX = static_cast < node_t*> (SB.allocate ( 1));
        SB.construct ( PX, A[i]);
        VPtr[i] =  PX ;
    }
    //SB.multi_alloc ( NELEM, A.begin(), VPtr, NT);
    point2 = now() ;
    typedef less_ptr_no_null <node_t, less_node <uint64_t> > comp_t ;
    point3 = now() ;
    parallel_stable_sort<comp_t> ( VPtr.begin(), VPtr.end(), comp_t(), NThread());
    point4 = now() ;
    //auto Aux = VPtr.delete_repeated ( VPtr.begin() , VPtr.end(), comp_t());
    //auto N2 = Aux - VPtr.begin() ;
    VPtr.resize ( N2);
    point5 = now() ;

    parallel_load_ptr<tree_t, iter_ptr_t> ( TR1, VPtr.begin(), VPtr.size() , NT);

    finish = now();
    cout<<"point1 :"<<(subtract_time(point1 , start))<<" seconds\n";
    cout<<"point2 :"<<(subtract_time(point2 , point1))<<" seconds\n";
    cout<<"point3 :"<<(subtract_time(point3 , point2))<<" seconds\n";
    cout<<"point4 :"<<(subtract_time(point4 , point3))<<" seconds\n";
    cout<<"point5 :"<<(subtract_time(point5 , point4))<<" seconds\n";
    cout<<"finish :"<<(subtract_time(finish , point5))<<" seconds\n";

    duracion = subtract_time(finish , start) ;
    cout<<"Time spent :"<<duracion<<" seconds\n";

    return 0;
};




