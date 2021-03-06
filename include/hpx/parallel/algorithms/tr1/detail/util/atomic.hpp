//----------------------------------------------------------------------------
/// @file atomic.hpp
/// @brief Basic layer for to simplify the use of atomic functions
/// @author Copyright(c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __HPX_PARALLEL_SORT_DETAIL_UTIL_ATOMIC_HPP
#define __HPX_PARALLEL_SORT_DETAIL_UTIL_ATOMIC_HPP

#include <atomic>
#include <cassert>
#include <type_traits>


namespace hpx		{
namespace parallel	{ 
namespace tr1		{
namespace detail	{
namespace util		{

//---------------------------------------------------------------------------
//         D E F I N I T I O N S
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//   A T O M I C  I N C R E M E N T    A N D   R E A D
//---------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//  function : atomic_read
/// @brief make the atomic read of an atomic variable, using a memory model
/// @param [in] at_var : atomic variable to read
/// @return value obtained
//-----------------------------------------------------------------------------
template <typename T >
inline T atomic_read ( std::atomic<T> & at_var)
{   return std::atomic_load_explicit<T> (&at_var, std::memory_order_acquire);
};
//
//-----------------------------------------------------------------------------
//  function : atomic_add
/// @brief Add a number to an atomic variable, using a memory model
/// @param [in] at_var : variable to add
/// @param [in] num : value to add to at_var
/// @return result of the operation
/// @remarks
//-----------------------------------------------------------------------------
template <typename T, typename T2>
inline T atomic_add ( std::atomic<T> &at_var, T2 num)
{   static_assert ( std::is_integral <T2>::value , "Bad parameter");
    return
    	std::atomic_fetch_add_explicit<T> (&at_var,(T)num,std::memory_order_acq_rel);
};
//
//-----------------------------------------------------------------------------
//  function : atomic_sub
/// @brief Atomic subtract of an atomic variable using memory model
/// @param [in] at_var : Varibale to subtract
/// @param [in] num : value to sub to at_var
/// @return result of the operation
/// @remarks
//-----------------------------------------------------------------------------
template <typename T, typename T2>
inline T atomic_sub ( std::atomic<T> &at_var, T2 num)
{   static_assert ( std::is_integral <T2>::value , "Bad parameter");
    return
    	std::atomic_fetch_sub_explicit<T> (&at_var,(T)num,std::memory_order_acq_rel);
};
//
//-----------------------------------------------------------------------------
//  function : atomic_write
/// @brief Write a value in an atomic variable using memory model
/// @param [in] at_var : varible to write
/// @param [in] num : value to write in at_var
/// @return resuñlts of the operation
/// @remarks
//-----------------------------------------------------------------------------
template <typename T, typename T2>
inline void atomic_write ( std::atomic<T> &at_var, T2 num)
{   static_assert ( std::is_integral <T2>::value , "Bad parameter");
    std::atomic_store_explicit<T> (&at_var, (T)num, std::memory_order_release);
};
//
//***************************************************************************
};// end namespace util
};// end namespace detail
};// end namespace tr1
};// end namespace parallel
};// end namespace hpx
//***************************************************************************
//
#endif
