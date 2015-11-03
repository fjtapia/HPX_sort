//----------------------------------------------------------------------------
/// @file parallel_merge_sort.hpp
/// @brief Quick Sort algorithm
///
/// @author Copyright (c) 2010 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __BOOST_SORT_ALGORITHM_PARALLEL_MERGE_SORT_HPP
#define __BOOST_SORT_ALGORITHM_PARALLEL_MERGE_SORT_HPP

#include <boost/sort/util/atomic.hpp>
#include <boost/sort/util/stack_cnc.hpp>
#include <boost/sort/algorithm/merge_sort.hpp>
#include <boost/sort/algorithm/indirect.hpp>
#include <functional>
#include <vector>


namespace hpx
{
namespace parallel
{
namespace _sort
{
namespace algorithm
{
using namespace boost::sort::util ;

template < class iter_t,
          typename compare = std::less < typename iter_value<iter_t>::type >
        >
struct parallel_merge_sort_comp
{
//-------------------------------------------------------------------------
//                     definitions
//-------------------------------------------------------------------------
typedef typename iter_value<iter_t>::type value_t ;
typedef std::atomic<size_t> count_t;
typedef std::atomic<uint32_t> join_t ;

//-------------------------------------------------------------------------
//                     definitions
//-------------------------------------------------------------------------
static const size_t NELEM_MIN = 1<<16 ;
iter_t global_first, global_last ;
count_t VCount[48] ;
join_t VJoin[48] ;
count_t NSort ;
join_t JoinSort ;
size_t Chuck_size ;
size_t N ;
uint32_t NLevel ;
NThread NT;
compare comp ;


//#########################################################################
//
//                F U N C T I O N S
//
//#########################################################################
parallel_merge_sort_comp (iter_t first, iter_t last ,NThread NT1=NThread())
                         :parallel_merge_sort_comp(first,last,compare(),NT1){};


parallel_merge_sort_comp ( iter_t first, iter_t last,
                           compare comp1, NThread NT1=NThread())
                         : global_first(first), global_last (last),
                           NT ( NT1),comp(comp1)
{   //------------------------------- begin -----------------------------
    auto  N1 = last -first;
    assert ( N1 >= 0 );
    N = (size_t) N1 ;
    if ( N < NELEM_MIN or NT() == 1)
    {   merge_sort ( first , last, comp );
        return ;
    };

    //---------------------------------------------------------------------
    //     Parallel Process
    //---------------------------------------------------------------------
    NLevel = NBits ( NT() ) ;
    if ( (1U<<NLevel) != NT()) NLevel++ ;

    Chuck_size = ( N + NMax(NLevel) -1)/NMax(NLevel);
    //for ( uint32_t i = 0 ; i < (64-BitsMax) ;++i)
    for ( uint32_t i = 0 ; i < NLevel ;++i)
    {   VCount[i].store (  0);
        VJoin[i].store (  0);
    };
    NSort.store ( 0);
    JoinSort.store (0);

    //---------------------------------------------------------------------
    //     Thread process
    //---------------------------------------------------------------------
    std::vector <std::thread > VT ( NT());

    for ( uint32_t i  =0  ; i < NT() ; ++i)
        VT[i] = std::thread ( &parallel_merge_sort_comp::sort_thread, this );
    for ( uint32_t i  =0  ; i < NT() ; ++i)
        VT[i].join();


}; // end of constructor

inline void merge (iter_t Alfa, value_t *P1, value_t *P1_end,
                                iter_t P2, iter_t P2_end         )
{   //---------------------------- begin -----------------------------------
    //value_t * P = P1 ;
    bool SW ;
    while  ( P2 != P2_end and Alfa != P2 )
    {   //*(Alfa++)= (comp(*P2,*P1))?std::move(*(P2++)):std::move(*(P1++));
        SW =  comp(*P2,*P1);
        *(Alfa++)= ( SW)?std::move(*(P2++)):std::move(*P1);
        if (  not SW) (P1++)->~value_t() ;
    }
    if ( P2 == P2_end)
    {   while ( P1!= P1_end )
        {   *(Alfa++) =std::move(*P1);
            (P1++)->~value_t() ;
        };
    };
};

inline size_t NMax ( uint32_t Level)
{   return ( (size_t)1 )<<Level ;
};

inline void Chuck (uint32_t Level,size_t Pos,iter_t &first,iter_t &last)
{   //-------------------------- begin ----------------------------
    size_t Max = NMax ( Level);
    assert ( Pos < Max);
    size_t Size = Chuck_size <<( NLevel -Level);
    first = global_first + ( Size *Pos);
    last = (Pos == (Max-1))?global_last : first + Size ;
};

void sort_thread( )
{   //------------------------- begin ----------------------
    iter_t first1 , first2 , last1 , last2 ;
    value_t *P = nullptr;
    size_t Size ;
    //---------------------------------------------------------------------
    //                        sorting
    //---------------------------------------------------------------------
    size_t Pos  = 0;
    //while ( (Pos = increment(NSort)) < NMax(NLevel))
    while ( (Pos = atomic_add(NSort,1)) < NMax(NLevel))
    {   Chuck ( NLevel, Pos, first1 , last1);
        Size = (Chuck_size +1) >>1;
        if ( P == nullptr)
        {   P = std::get_temporary_buffer <value_t> (Size).first ;
            if ( P == nullptr) throw std::bad_alloc() ;
        };
        merge_sort (first1,last1,comp,P,Size);
    };
    atomic_add ( JoinSort,1);
    if ( P != nullptr) std::return_temporary_buffer (P) ;
    P = nullptr ;
    while ( atomic_read ( JoinSort) < NT()) std::this_thread::yield() ;

    //---------------------------------------------------------------------
    //                         merging
    //---------------------------------------------------------------------
    for ( uint32_t Level = NLevel ; Level != 0 ; --Level)
    {   Size = Chuck_size <<( NLevel - Level );
        while ((Pos = atomic_add ( VCount[Level-1],1)) < NMax ( Level -1) )
        {   if ( P == nullptr)
            {   P = std::get_temporary_buffer <value_t> (Size).first ;
                if ( P == nullptr) throw std::bad_alloc() ;
            };
            Chuck ( Level, Pos<<1, first1, last1);
            Chuck ( Level, (Pos<<1)+1, first2 , last2);
            //move_to_uninitialized_memory ( first1, P,Size);
            uninit_move_buf (P,first1, first1 + Size );
            //iter_t src = first1;
            //value_t *dest = P ;
            //for ( size_t i =0 ; i < Size ; ++i)
            //    ::new ( static_cast<void*>((dest++))) value_t( std::move(*(src++)) );

            merge ( first1 , P , P+Size, first2 , last2);
            //destroy ( P , Size);
        };
        atomic_add ( VJoin[Level], 1);
        //if ( P != nullptr)  ::operator delete (static_cast <void*> (P)) ;
        if ( P != nullptr) std::return_temporary_buffer (P) ;
        P = nullptr ;
        while ( atomic_read ( VJoin[Level]) < NT()) std::this_thread::yield() ;
    };
};
};// end struct parallel_merge_sort

//------------------------------------------------------------------------------
// These functions are for to select the correct format depending of the number
// and type of the parameters
//------------------------------------------------------------------------------
template < class iter_t >
void parallel_merge_sort ( iter_t first, iter_t last , NThread NT = NThread() )
{   parallel_merge_sort_comp <iter_t> ( first, last, NT);
};

template < class iter_t,
          typename compare = std::less < typename iter_value<iter_t>::type >
        >
void parallel_merge_sort ( iter_t first, iter_t last,
                           compare comp1, NThread NT = NThread() )
{   //----------------------------- begin ----------------------------------
    parallel_merge_sort_comp<iter_t,compare> ( first, last,comp1,NT);
};

//############################################################################
//                                                                          ##
//                I N D I R E C T     F U N C T I O N S                     ##
//                                                                          ##
//############################################################################
template < class iter_t >
void indirect_parallel_merge_sort ( iter_t first, iter_t last ,
                                   NThread NT = NThread() )
{   //------------------------------- begin--------------------------
    typedef std::less <typename iter_value<iter_t>::type> compare ;
    typedef less_ptr_no_null <iter_t, compare>      compare_ptr ;

    std::vector<iter_t> VP ;
    create_index ( first , last , VP);
    parallel_merge_sort  ( VP.begin() , VP.end(), compare_ptr(),NT );
    sort_index ( first , VP) ;
};
template < class iter_t,
          typename compare = std::less < typename iter_value<iter_t>::type >
        >
void indirect_parallel_merge_sort ( iter_t first, iter_t last,
                                    compare comp1, NThread NT = NThread() )
{   //----------------------------- begin ----------------------------------
    typedef less_ptr_no_null <iter_t, compare>      compare_ptr ;

    std::vector<iter_t> VP ;
    create_index ( first , last , VP);
    parallel_merge_sort  ( VP.begin() , VP.end(), compare_ptr(comp1),NT );
    sort_index ( first , VP) ;
};
//
//****************************************************************************
};//    End namespace algorithm
};//    End namespace _sort
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
//
#endif
