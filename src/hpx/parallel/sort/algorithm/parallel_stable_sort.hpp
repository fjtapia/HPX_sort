//----------------------------------------------------------------------------
/// @file parallel_stable_sort.hpp
/// @brief
///
/// @author Copyright (c) 2010 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __SORT_ALGORITHM_PARALLEL_STABLE_SORT_HPP
#define __SORT_ALGORITHM_PARALLEL_STABLE_SORT_HPP

#include <hpx/parallel/sort/tools/atomic.hpp>
#include <hpx/parallel/sort/tools/nthread.hpp>
#include <hpx/parallel/sort/tools/stack_cnc.hpp>
#include <hpx/parallel/sort/algorithm/sample_sort.hpp>
#include <functional>
#include <vector>
#include <hpx/parallel/sort/algorithm/indirect.hpp>
#include <iostream>

namespace hpx
{
namespace parallel
{
namespace _sort
{
namespace algorithm
{
namespace bspu = hpx::parallel::_sort::util;
using hpx::parallel::_sort::tools::NThread ;
using hpx::parallel::_sort::tools::NThread_HW ;
//
///---------------------------------------------------------------------------
/// @struct parallel_stable_sort_tag
/// @brief This a structure for to implement a parallel stable sort, exception
///        safe
//----------------------------------------------------------------------------
template < class iter_t,
          typename compare = std::less < typename iter_value<iter_t>::type >
        >
struct parallel_stable_sort_tag
{
//-------------------------------------------------------------------------
//                      DEFINITIONS
//-------------------------------------------------------------------------
typedef typename iter_value<iter_t>::type value_t ;

//-------------------------------------------------------------------------
//                     VARIABLES
//-------------------------------------------------------------------------
size_t NElem ;
value_t *Ptr ;
const size_t NELEM_MIN = 1<<16 ;

//#########################################################################
//
//                F U N C T I O N S
//
//#########################################################################
//
//-----------------------------------------------------------------------------
//  function : parallel_stable_sort_tag
/// @brief constructor of the class
///
/// @param [in] R : range of elements to sort
/// @param [in] comp : object for to compare two elements
/// @param [in] NT : NThread object for to define the number of threads to use
///                  in the process. By default is the number of thread HW
//-----------------------------------------------------------------------------
parallel_stable_sort_tag ( range<iter_t> R, compare comp,
                           const NThread &NT=NThread() ):NElem(0),Ptr(nullptr)
{   //------------------------------- begin -----------------------------
	assert ( R.valid());

    NElem = R.size() ;
    size_t NPtr = ( NElem +1 )>>1 ;

    if ( NElem < NELEM_MIN or NT() == 1)
    {   spin_sort ( R.first , R.last, comp );
        return ;
    };
    //------------------- check if sort --------------------------------------
    bool SW = true ;
    for ( iter_t it1 = R.first, it2 = R.first+1 ;
          it2 != R.last and (SW = not comp(*it2,*it1));it1 = it2++);
    if (SW) return ;

    Ptr = std::get_temporary_buffer<value_t>(NPtr).first ;
    if ( Ptr == nullptr) throw std::bad_alloc();
    
    //---------------------------------------------------------------------
    //     Parallel Process
    //---------------------------------------------------------------------
    range<iter_t> R1 ( R.first, R.first + NPtr), R2(R.first + NPtr, R.last );
    range <value_t*> Rbuf (Ptr , Ptr + NPtr );
    sample_sort_tag<iter_t, compare> ( R1, comp,NT, Rbuf);
    sample_sort_tag<iter_t, compare> ( R2, comp, NT,Rbuf);

    Rbuf = move (Rbuf, R1);
    R = half_merge_big (R, Rbuf, R2, comp );
}; // end of constructor
//
//-----------------------------------------------------------------------------
//  function :~parallel_stable_sort_tag
/// @brief destructor of the class. The utility is to destroy the temporary
///        buffer used in the sorting process
//-----------------------------------------------------------------------------
~parallel_stable_sort_tag ()
{   //------------------------------- begin ----------------------------------
    if ( Ptr != nullptr) std::return_temporary_buffer ( Ptr) ;
};
//----------------------------------------------------------------------------
};// end struct parallel_stable_sort_tag
//----------------------------------------------------------------------------

//
//-----------------------------------------------------------------------------
//  function : parallel_stable_sort
/// @brief envelope function for to call to a parallel_stable_sort_tag object
/// 
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to next element after the last of the range
/// @param [in] NT : NThread object for to define the number of threads to use
///                  in the process. By default is the number of thread HW
//-----------------------------------------------------------------------------
template < class iter_t >
void parallel_stable_sort ( iter_t first, iter_t last , 
                            const NThread &NT = NThread() )
{   //------------------------------- begin ----------------------------------
    typedef std::less<typename iter_value<iter_t>::type > compare;
    range<iter_t> R ( first, last);
    parallel_stable_sort_tag <iter_t,compare> ( R,compare(), NT);
};
//
//-----------------------------------------------------------------------------
//  function : parallel_stable_sort
/// @brief envelope function for to call to a parallel_stable_sort_tag object
///
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to next element after the last of the range
/// @param [in] comp1 : object for to compare two elements
/// @param [in] NT : NThread object for to define the number of threads to use
///                  in the process. By default is the number of thread HW
//-----------------------------------------------------------------------------
template < class iter_t,
          typename compare = std::less <typename iter_value<iter_t>::type> >
void parallel_stable_sort ( iter_t first, iter_t last,
                           compare comp1, const NThread &NT = NThread() )
{   //----------------------------- begin ----------------------------------
    range<iter_t> R ( first, last);
    parallel_stable_sort_tag<iter_t,compare> ( R,comp1,NT);
};

//############################################################################
//                                                                          ##
//                I N D I R E C T     F U N C T I O N S                     ##
//                                                                          ##
//############################################################################
//
//-----------------------------------------------------------------------------
//  function : indirect_parallel_stable_sort
/// @brief indirect sorting using the parallel_stable_sort algorithm
///
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to next element after the last of the range
/// @param [in] NT : NThread object for to define the number of threads to use
///                  in the process. By default is the number of thread HW
//-----------------------------------------------------------------------------
template < class iter_t >
void indirect_parallel_stable_sort ( iter_t first, iter_t last ,
                                      const NThread &NT = NThread() )
{   //------------------------------- begin--------------------------
    typedef std::less <typename iter_value<iter_t>::type> compare ;
    typedef less_ptr_no_null <iter_t, compare>      compare_ptr ;

    std::vector<iter_t> VP ;
    create_index ( first , last , VP);
    parallel_stable_sort  ( VP.begin() , VP.end(), compare_ptr(),NT );
    sort_index ( first , VP) ;
};
//
//-----------------------------------------------------------------------------
//  function : indirect_parallel_stable_sort
/// @brief indirect sorting using the parallel_stable_sort algorithm
///
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to next element after the last of the range
/// @param [in] comp : object for to compare two elements
/// @param [in] NT : NThread object for to define the number of threads to use
///                  in the process. By default is the number of thread HW
//-----------------------------------------------------------------------------
template < class iter_t,
          typename compare = std::less <typename iter_value<iter_t>::type> >
void indirect_parallel_stable_sort ( iter_t first, iter_t last,
                                    compare comp1, const NThread & NT = NThread() )
{   //----------------------------- begin ----------------------------------
    typedef less_ptr_no_null <iter_t, compare>      compare_ptr ;

    std::vector<iter_t> VP ;
    create_index ( first , last , VP);
    parallel_stable_sort  ( VP.begin() , VP.end(), compare_ptr(comp1),NT );
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
