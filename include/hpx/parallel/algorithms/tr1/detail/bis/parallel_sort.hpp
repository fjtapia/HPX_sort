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
#ifndef  __HPX_PARALLEL_SORT_DETAIL_BIS_PARALLEL_SORT_HPP
#define  __HPX_PARALLEL_SORT_DETAIL_BIS_PARALLEL_SORT_HPP

#include <hpx/parallel/algorithms/tr1/detail/bis/backbone.hpp>

namespace hpx       {
namespace parallel  {
namespace tr1       {
namespace detail    {
namespace bis       {

using hpx::parallel::tr1::detail::util::nbits64 ;
//
///---------------------------------------------------------------------------
/// @struct parallel_sort
/// @brief This contains all the information shared betwen the classes of the
///        algorithm
/// @tparam
/// @remarks
//----------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
struct parallel_sort
{
    //-------------------------------------------------------------------------
    //                  D E F I N I T I O N S
    //-------------------------------------------------------------------------
    typedef typename std::iterator_traits<iter_t>::value_type   value_t ;
    typedef std::atomic<uint32_t>                               atomic_t ;
    typedef std::function <void (void) >                        function_t ;
    typedef backbone<block_size, iter_t, compare>               backbone_t;

    //------------------------------------------------------------------------
    //                V A R I A B L E S
    //------------------------------------------------------------------------
    backbone_t &bk ;
    size_t max_per_thread ;
    atomic_t counter ;

    //------------------------------------------------------------------------
    //                F U N C T I O N S
    //------------------------------------------------------------------------
    parallel_sort ( backbone_t &bkbn, iter_t first, iter_t last);

    void divide_sort(iter_t first, iter_t last , uint32_t level  );
//--------------------------------------------------------------------------
};// end struct parallel_sort
//--------------------------------------------------------------------------
//
//############################################################################
//                                                                          ##
//                                                                          ##
//            N O N     I N L I N E      F U N C T I O N S                  ##
//                                                                          ##
//                                                                          ##
//############################################################################
//
//
//------------------------------------------------------------------------
//  function : parallel_sort
/// @brief constructor of the class
/// @param [in] bkbn : backbone struct with all the information to sort
/// @param [in] first : iterator to the first element to sort
/// @param [in] last : iterator to the next element after the last
//------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
parallel_sort <block_size, iter_t, compare>::
parallel_sort (backbone_t &bkbn, iter_t first, iter_t last):bk(bkbn), counter(0)
{   //-------------------------- begin ------------------------------------
    assert ( (last-first) >= 0 ) ;
    size_t nelem = size_t ( last - first ) ;

    //------------------- check if sort --------------------------------------
    bool sorted = true ;
    for ( iter_t it1 = first, it2 = first+1 ;
          it2 != last and (sorted = not bk.cmp(*it2,*it1));it1 = it2++);
    if (sorted) return ;

    //-------------------max_per_thread ---------------------------
    uint32_t nbits_size = (nbits64(sizeof (value_t)))>>1;
    if ( nbits_size > 5 ) nbits_size = 5 ;
    max_per_thread = 1<< (18 - nbits_size);
    uint32_t level = ((nbits64(nelem / max_per_thread)) * 2);

    //---------------- check if only single thread -----------------------
    if ( nelem < ( max_per_thread))
    {   intro_sort (first, last, bk.cmp);
        return ;
    };

    divide_sort ( first, last,level ) ;
    // wait until all the parts are finished
    bk.exec(counter);
};

//------------------------------------------------------------------------
//  function : divide_sort
/// @brief this function divide the data in two part, for to be sorted in
///        a parallel mode
/// @param [in] first : iterator to the first element to sort
/// @param [in] last : iterator to the next element after the last
//------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
void parallel_sort <block_size, iter_t, compare>::
divide_sort(iter_t first, iter_t last , uint32_t level  )
{   //------------------- check if sort -----------------------------------
    bool sorted = true ;
    for ( iter_t it1 = first, it2 = first+1 ;
          it2 != last and (sorted = not bk.cmp(*it2,*it1));it1 = it2++);
    if (sorted) return ;

    //---------------- check if finish the subdivision -------------------
    size_t nelem  = last - first ;
    if ( level == 0 or nelem < ( max_per_thread))
    {   return intro_sort (first, last, bk.cmp);
    };

    //-------------------- pivoting  ----------------------------------
    pivot9 ( first, last ,bk.cmp);
    const value_t & val = const_cast < value_t &>(* first);
    iter_t c_first = first+1 , c_last  = last-1;

    while ( bk.cmp(*c_first, val) ) ++c_first ;
    while ( bk.cmp ( val,*c_last ) ) --c_last ;
    while (not( c_first > c_last ))
    {   std::swap ( *(c_first++), *(c_last--));
        while ( bk.cmp (*c_first, val) ) ++c_first;
        while ( bk.cmp ( val, *c_last) ) --c_last ;
    }; // End while
    std::swap ( *first , * c_last);

    // insert  the work of the second half in the stack of works
    util::atomic_add ( counter , 1 );
    function_t f1 = [=] ( ){    divide_sort ( c_first, last,level -1) ;
                                util::atomic_sub( this->counter,1);
                           };
    bk.works.emplace_back( f1) ;

    // The first half is done by the same thread
    divide_sort (first, c_last, level -1);
};
//
//****************************************************************************
};//    End namespace bis
};//    End namespace detail
};//    End namespace tr1
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
//
#endif
