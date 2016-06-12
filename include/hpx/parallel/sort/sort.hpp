//----------------------------------------------------------------------------
/// @file sort.hpp
/// @brief This file contains the sort functions of the sort library
///
/// @author Copyright (c) 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __HPX_PARALLEL_SORT_SORT_HPP
#define __HPX_PARALLEL_SORT_SORT_HPP

#include <iostream>
#include <iterator>
#include <iterator>
#include <hpx/parallel/sort/detail/util/compare_traits.hpp>
#include <hpx/parallel/sort/detail/parallel_stable_sort.hpp>
#include <hpx/parallel/sort/detail/block_indirect_sort.hpp>

#include <hpx/parallel/execution_policy.hpp>

namespace hpx
{
namespace parallel
{
namespace sort
{

//****************************************************************************
//             USING AND DEFINITIONS
//****************************************************************************
using hpx::parallel::sort::detail::util::compare_iter ;

using std::iterator_traits ;
//----------------------------------------------------------------------------
// The code of the class NThread is in boost/sort/parallel/util/atomic.hpp
//----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//  function : sample_sort
/// @brief this function implement a stable parallel sort with the algorithm of
///        sample sort. The number of threads to use is defined by the NThread
///        parameter
///
/// @param [in] parallel_execution_policy : This sorting is parallel
/// @param [in] firts : iterator to the first element of the range to sort
/// @param [in] last : iterator after the last element to the range to sort
/// @param [in] comp : object for to compare two elements pointed by iter_t
///                    iterators
/// @exception
/// @return
/// @remarks
//-----------------------------------------------------------------------------
template <class iter_t, class compare= compare_iter<iter_t> >
void sample_sort (	hpx::parallel::parallel_execution_policy ,
            iter_t first, iter_t last , compare comp= compare()  )
{   //------------------------------ begin ---------------------------------
    uint32_t nthread = (uint32_t) hpx::get_os_thread_count() ;
	detail::sample_sort <iter_t,compare>( first, last,comp, nthread);
};
//
//-----------------------------------------------------------------------------
//  function : sort
/// @brief this function implement a non stable parallel sort. The number of
///        threads to use is defined by the NThread parameter
///
/// @param [in] parallel_execution_policy : This sorting is parallel
/// @param [in] firts : iterator to the first element of the range to sort
/// @param [in] last : iterator after the last element to the range to sort
/// @param [in] comp : object for to compare two elements pointed by iter_t
///                    iterators
/// @exception
/// @return
/// @remarks
//-----------------------------------------------------------------------------
template <class iter_t, class compare= compare_iter<iter_t> >
void sort (	hpx::parallel::parallel_execution_policy ,
            iter_t first, iter_t last , compare comp= compare()  )
{	//----------------------------- begin -------------------------------
    uint32_t nthread = (uint32_t) hpx::get_os_thread_count() ;
	detail::block_indirect_sort<1024, 64,iter_t,compare> ( first, last,comp, nthread);
};

//
//-----------------------------------------------------------------------------
//  function : sort
/// @brief this function implement a non stable sort, based internally in the
///        intro_sort algorithm. Run with 1 thread
///
/// @param [in] sequential_execution_policy : sort with 1 thread
/// @param [in] firts : iterator to the first element of the range to sort
/// @param [in] last : iterator after the last element to the range to sort
/// @param [in] comp : object for to compare two elements pointed by iter_t
///                    iterators
/// @exception
/// @return
/// @remarks
//-----------------------------------------------------------------------------
template <class iter_t, class compare= compare_iter<iter_t> >
void sort (	hpx::parallel::sequential_execution_policy ,
            iter_t first, iter_t last ,compare comp= compare()  )
{	detail::intro_sort(first, last,comp);
};

//
//-----------------------------------------------------------------------------
//  function : stable_sort
/// @brief this function implement a stable parallel sort. The number of
///        threads to use is defined by the NThread parameter
///
/// @param [in] parallel_execution_policy : This sorting is parallel
/// @param [in] firts : iterator to the first element of the range to sort
/// @param [in] last : iterator after the last element to the range to sort
/// @param [in] comp : object for to compare two elements pointed by iter_t
///                    iterators
/// @exception
/// @return
/// @remarks
//-----------------------------------------------------------------------------
template <class iter_t, class compare= compare_iter<iter_t> >
void stable_sort (	hpx::parallel::parallel_execution_policy ,
            iter_t first, iter_t last , compare comp= compare()  )
{	//--------------------------- begin --------------------------------------
    uint32_t nthread = (uint32_t) hpx::get_os_thread_count() ;
	detail::parallel_stable_sort <iter_t,compare>( first, last,comp, nthread);
};

//
//-----------------------------------------------------------------------------
//  function : stable_sort
/// @brief this function implement a stable sort, based internally in the new
///        smart_merge_sort algorithm. Run with 1 thread
///
/// @param [in] sequential_execution_policy : sort with 1 thread
/// @param [in] firts : iterator to the first element of the range to sort
/// @param [in] last : iterator after the last element to the range to sort
/// @param [in] comp : object for to compare two elements pointed by iter_t
///                    iterators
/// @exception
/// @return
/// @remarks
//-----------------------------------------------------------------------------
template <class iter_t, class compare= compare_iter<iter_t> >
void stable_sort (	hpx::parallel::sequential_execution_policy ,
            iter_t first, iter_t last ,compare comp= compare()  )
{	detail::spin_sort<iter_t,compare>(first, last,comp);
};
//
//****************************************************************************
};//    End namespace sort
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
//
#endif
