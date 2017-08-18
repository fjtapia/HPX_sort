//----------------------------------------------------------------------------
/// @file block_indirect_sort.hpp
/// @brief Sample Sort algorithm
///
/// @author Copyright (c) 2016 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef  __HPX_PARALLEL_SORT_DETAIL_BLOCK_INDIRECT_SORT_HPP
#define  __HPX_PARALLEL_SORT_DETAIL_BLOCK_INDIRECT_SORT_HPP

#include <atomic>
#include <iterator>
#include <iostream>
#include <hpx/hpx.hpp>
#include <hpx/parallel/sort/detail/bis/parallel_sort.hpp>
#include <hpx/parallel/sort/detail/bis/merge_blocks.hpp>
#include <hpx/parallel/sort/detail/bis/move_blocks.hpp>

namespace hpx       {
namespace parallel  {
namespace sort      {
namespace detail    {


//
///---------------------------------------------------------------------------
/// @struct block_indirect_sort
/// @brief This a structure for to implement a sample sort, exception
///        safe
/// @tparam
/// @remarks
//----------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t,
          class compare = util::compare_iter<iter_t> >
struct block_indirect_sort
{

//----------------------------------------------------------------------------
//                  D E F I N I T I O N S
//----------------------------------------------------------------------------
typedef typename std::iterator_traits<iter_t>::value_type   value_t ;
typedef std::atomic<uint32_t>                               atomic_t ;
typedef util::range <size_t>                                range_pos ;
typedef util::range <iter_t>                                range_it ;
typedef util::range <value_t*>                              range_buf ;
typedef std::function <void (void) >                        function_t ;

typedef bis::block_pos                      block_pos_t;
typedef bis::block<block_size, iter_t>      block_t;
typedef bis::backbone<block_size, iter_t, compare>      backbone_t;
typedef bis::parallel_sort<block_size, iter_t, compare>     parallel_sort_t;
typedef bis::merge_blocks < block_size, group_size, iter_t, compare> merge_blocks_t;
typedef bis::move_blocks <block_size, group_size, iter_t, compare> move_blocks_t;
typedef bis::compare_block_pos <block_size, iter_t, compare> compare_block_pos_t ;

//
//------------------------------------------------------------------------
//       V A R I A B L E S   A N D  C O N S T A N T S
//------------------------------------------------------------------------
backbone_t bk ;
atomic_t counter ;
value_t *ptr ;
bool construct ;
range_buf rglobal_buf ;
uint32_t nthread ;
//
//------------------------------------------------------------------------
//                F U N C T I O N S
//------------------------------------------------------------------------

block_indirect_sort ( iter_t first , iter_t last , compare cmp, uint32_t nthr );

block_indirect_sort ( iter_t first , iter_t last ):
block_indirect_sort ( first, last, compare(),
                      std::thread::hardware_concurrency() ) {};

block_indirect_sort (iter_t first , iter_t last , compare cmp):
block_indirect_sort ( first, last, cmp, std::thread::hardware_concurrency()){};

block_indirect_sort ( iter_t first , iter_t last , uint32_t nthread):
block_indirect_sort  ( first, last, compare(), nthread){};

//
//-----------------------------------------------------------------------------
//  function :~block_indirect_sort
/// @brief destructor of the class. The utility is to destroy the temporary
///        buffer used in the sorting process
/// @exception
/// @return
/// @remarks
//-----------------------------------------------------------------------------
~block_indirect_sort ( void)
{   //----------------------------------- begin -------------------------
    if (ptr != nullptr)
    {   if ( construct)
        {   util::destroy (rglobal_buf);
            construct = false ;
        };
        std::return_temporary_buffer ( ptr) ;
    };
};

void split_range ( size_t pos_index1,size_t pos_index2, uint32_t level_thread);

void start_function ( void);
//----------------------------------------------------------------------------
};// End class block_indirect_sort
//----------------------------------------------------------------------------
//
//
//############################################################################
//                                                                          ##
//                                                                          ##
//            N O N     I N L I N E      F U N C T I O N S                  ##
//                                                                          ##
//                                                                          ##
//############################################################################
//
//-------------------------------------------------------------------------
//  function : block_indirect_sort
/// @brief begin with the execution of the functions stored in works
/// @param [in] counter : atomic counter. When 0 exits the function
/// @return range
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
block_indirect_sort<block_size, group_size, iter_t, compare>::
block_indirect_sort ( iter_t first , iter_t last , compare cmp, uint32_t nthr )
                :bk (first, last, cmp), counter(0), ptr(nullptr),
                 construct (false ),nthread ( nthr)
{   //-------------------------- begin -------------------------------------
    assert ( (last-first) >= 0 ) ;
    size_t nelem = size_t ( last - first ) ;
    if ( nelem == 0 ) return ;

    //------------------- check if sort --------------------------------------
    bool sorted = true ;
    for ( iter_t it1 = first, it2 = first+1 ;
          it2 != last and (sorted = not bk.cmp(*it2,*it1));it1 = it2++);
    if (sorted) return ;

    //---------------- check if only single thread -----------------------
    size_t nthreadmax = nelem / (block_size * group_size) + 1;
    if (nthread > nthreadmax) nthread = (uint32_t) nthreadmax;

    uint32_t nbits_size = (util::nbits64(sizeof (value_t)))>>1;
    if ( nbits_size > 5 ) nbits_size = 5 ;
    size_t max_per_thread = 1<< (18 - nbits_size);


    if ( nelem < ( max_per_thread) or nthread < 2)
    {   intro_sort (first, last, bk.cmp);
        return ;
    };

    //----------------- creation of the temporary buffer --------------------
    ptr =std::get_temporary_buffer<value_t>( block_size*nthread).first ;
    if ( ptr == nullptr) throw std::bad_alloc() ;

    rglobal_buf = range_buf ( ptr , ptr + (block_size * nthread) );
    util::init ( rglobal_buf, *first);
    construct = true ;

    std::vector< value_t*> vbuf ( nthread);
    for ( uint32_t i =0 ; i < nthread ; ++i)
        vbuf[i] = ptr + (i * block_size);

    //------------------------------------------------------------------------
    util::atomic_write ( counter , 1 ) ;
    function_t f1 = [&]()
                    {    start_function( );
                         util::atomic_sub( counter , 1 ) ;
                    };
    bk.works.emplace_back ( f1 ) ;

    //------------------------------------------------------------------------
    //                    PROCESS
    //------------------------------------------------------------------------
    std::vector <hpx::future <void> > vfuture (nthread);

    for ( uint32_t i =0 ; i < nthread ; ++i)
    {   auto f1 = [=, &vbuf ] () {  bk.exec (vbuf[i], this->counter);};
        vfuture[i] = hpx::async (  f1 );
    };
    for ( uint32_t i =0 ; i < nthread ; ++i) vfuture[i].get() ;
};


//
//-----------------------------------------------------------------------------
//  function : split_rage
/// @brief this function splits a range, and depending of the size, sort
///        directly or call to a recursive split_range
/// @param [in] pf : pointer to the atomic counter of the father
/// @param [in] RP : range of positions to split
/// @param [in] Level : depth of the split
//-----------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
void block_indirect_sort<block_size, group_size, iter_t, compare>::
split_range ( size_t pos_index1 ,size_t pos_index2, uint32_t level_thread )
{   //----------------------------- begin -----------------------------------
    size_t nblock = pos_index2 - pos_index1;

    //-------------------------------------------------------------------------
    // In the blocks not sorted, the physical position is the logical position
    //-------------------------------------------------------------------------
    iter_t first = bk.get_block ( pos_index1).first ;
    iter_t last = bk.get_range ( pos_index2 -1).last ;

    if ( nblock < group_size)
    {   intro_sort ( first, last , bk.cmp ) ;
        return ;
    };

    size_t pos_index_mid = pos_index1 + (nblock >>1 ) ;
    atomic_t son_counter (1) ;

    //-------------------------------------------------------------------------
    // Insert in the stack the work for the second part, and the actual thread,
    // execute the first part
    //-------------------------------------------------------------------------
    if ( level_thread != 0 )
    {   auto f1 = [=, &son_counter]( )
                  { split_range ( pos_index_mid,pos_index2, level_thread-1) ;
                    util::atomic_sub ( son_counter , 1 ) ;
                  };
        bk.works.emplace_back ( f1 ) ;
        split_range ( pos_index1 , pos_index_mid,level_thread -1) ;
    }
    else
    {   iter_t mid = first + ( (nblock >>1 ) * block_size ) ;
        auto f1 = [=, &son_counter]()
                  { parallel_sort_t ( bk, mid , last ) ;
                    util::atomic_sub ( son_counter , 1 ) ;
                  };
        bk.works.emplace_back ( f1 ) ;
        parallel_sort_t ( bk , first ,mid ) ;
    };
    bk.exec ( son_counter ) ;
    merge_blocks_t ( bk, pos_index1 , pos_index_mid , pos_index2 ) ;
};
//
//-----------------------------------------------------------------------------
//  function : start_function
/// @brief this function splits a range, and depending of the size, sort
///        directly or call to a recursive split_range
/// @param [in] pf : pointer to the atomic counter of the father
/// @param [in] RP : range of positions to split
/// @param [in] Level : depth of the split
//-----------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
void block_indirect_sort<block_size, group_size, iter_t, compare>::
start_function ( void)
{   //----------------------- begin -----------------------------------
    if (nthread < 6 )
    {   parallel_sort_t (bk, bk.global_range.first, bk.global_range.last);
    }
    else
    {   size_t level_thread = nbits64 ( nthread -1 );
        split_range ( 0 , bk.nblock, level_thread -1) ;
        move_blocks_t k( bk ) ;
    };
};
//
//****************************************************************************
};//    End namespace detail
};//    End namespace parallel
};//    End namespace sort
};//    End namespace boost
//****************************************************************************
//
#endif
