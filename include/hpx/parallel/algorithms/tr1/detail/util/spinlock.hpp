//----------------------------------------------------------------------------
/// @file spinlock_t.hpp
/// @brief
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __HPX_PARALLEL_SORT_DETAIL_UTIL_SPINLOCK_HPP
#define __HPX_PARALLEL_SORT_DETAIL_UTIL_SPINLOCK_HPP


#include <atomic>
#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <ctime>

namespace hpx		{
namespace parallel	{ 
namespace tr1		{
namespace detail	{
namespace util		{

//##########################################################################
//                                                                        ##
//                 C L A S S     S P I N L O C K                          ##
//                                                                        ##
//##########################################################################
//---------------------------------------------------------------------------
/// @class spinlock_t
///
/// @remarks This class meet the BasicLockable requirements ( lock, unlock )
//---------------------------------------------------------------------------
class spinlock_t
{
private:
//***************************************************************************
//             P R I V A T E      V A R I A B L E S
//***************************************************************************
std::atomic_flag af ;

public :
//
//---------------------------------------------------------------------------
//  function : spinlock_t
/// @brief  class constructor
/// @param [in]
//---------------------------------------------------------------------------
explicit inline spinlock_t() noexcept{ af.clear(); };
//
//---------------------------------------------------------------------------
//  function : lock
/// @brief  Lock the spinlock_t
//---------------------------------------------------------------------------
inline void lock ( ) noexcept
{
	while (af.test_and_set (std::memory_order_acquire))
    {
        std::this_thread::yield ( );
    };
};
//
//---------------------------------------------------------------------------
//  function : try_lock
/// @brief Try to lock the spinlock_t, if not, return false
/// @return true : locked
///         false: not previous locked
//---------------------------------------------------------------------------
inline bool try_lock() noexcept
{   return not af.test_and_set(std::memory_order_acquire);
};
//
//---------------------------------------------------------------------------
//  function : unlock
/// @brief  unlock the spinlock_t
//---------------------------------------------------------------------------
inline void unlock() noexcept
{   //----------------------- begin -----------------
    af.clear(std::memory_order_release );
};
//
//***************************************************************************
};// E N D    C L A S S     S P I N L O C K
//***************************************************************************
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
