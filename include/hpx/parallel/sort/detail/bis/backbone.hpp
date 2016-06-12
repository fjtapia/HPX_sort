//----------------------------------------------------------------------------
/// @file backbone.hpp
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
#ifndef  __HPX_PARALLEL_SORT_DETAIL_BIS_BACKBONE_HPP
#define  __HPX_PARALLEL_SORT_DETAIL_BIS_BACKBONE_HPP

#include <atomic>
#include <future>
#include <iterator>
#include <iostream>
#include <hpx/parallel/sort/detail/util/nbits.hpp>
#include <hpx/parallel/sort/detail/intro_sort.hpp>
#include <hpx/parallel/sort/detail/util/stack_cnc.hpp>
#include <hpx/parallel/sort/detail/util/atomic.hpp>

#include <hpx/parallel/sort/detail/bis/block.hpp>


namespace hpx		{
namespace parallel	{    
namespace sort		{
namespace detail	{
namespace bis       {

using hpx::parallel::sort::detail::util::stack_cnc ;
///---------------------------------------------------------------------------
/// @struct backbone
/// @brief This contains all the information shared betwen the classes of the
///        algorithm
/// @tparam
/// @remarks
//----------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
struct backbone
{   //-------------------------------------------------------------------------
    //                  D E F I N I T I O N S
    //-------------------------------------------------------------------------
    typedef typename std::iterator_traits<iter_t>::value_type   value_t ;
    typedef std::atomic<uint32_t> 				                atomic_t ;
    typedef range <size_t>				                		range_pos ;
    typedef range <iter_t>                                		range_it ;
    typedef range <value_t*>                              		range_buf ;
    typedef std::function <void (void) >                        function_t ;
    typedef block <block_size, iter_t>							block_t ;


	//------------------------------------------------------------------------
	//                V A R I A B L E S
	//------------------------------------------------------------------------
    util::range<iter_t> global_range;
    std::vector<block_pos> index ;
    size_t nelem, nblock,ntail ;
    compare cmp ;
    range_it range_tail ;
    static thread_local value_t* buf;
    stack_cnc <std::function <void (void) > > works ;

    //
    //------------------------------------------------------------------------
    //                F U N C T I O N S
    //------------------------------------------------------------------------
    backbone ( iter_t first, iter_t last , compare comp);

    //------------------------------------------------------------------------
    //  function : get_block
    /// @brief obtain the block in the position pos
    /// @param [in] pos : position of the range
    /// @return block required
    //------------------------------------------------------------------------
    block_t get_block ( size_t pos) const
    {	return block_t (global_range.first + ( pos * block_size ));
    };
    //-------------------------------------------------------------------------
    //  function : get_range
    /// @brief obtain the range in the position pos
    /// @param [in] pos : position of the range
    /// @return range required
    //-------------------------------------------------------------------------
    range_it get_range ( size_t pos) const
    {	//--------------------------- begin --------------------------------
    	iter_t it1 = global_range.first + ( pos * block_size );
        iter_t it2 = ( pos == (nblock-1) )?global_range.last:it1+block_size;
        return range_it ( it1 , it2);
    };
    //-------------------------------------------------------------------------
    //  function : get_range_buf
    /// @brief obtain the range in the position pos
    /// @param [in] pos : position of the range
    /// @return range
    //-------------------------------------------------------------------------
    range_buf get_range_buf ( ) const
    {	return range_buf ( buf, buf+ block_size) ;
    };

    //-------------------------------------------------------------------------
    //  function : exec
    /// @brief begin with the execution of the functions stored in works
    /// @param [in] counter : atomic counter. When 0 exits the function
    /// @return range
    //-------------------------------------------------------------------------
    void exec( value_t * ptr_buf ,atomic_t & counter )
    {   //------------------------ begin --------------------------------
    	buf = ptr_buf ;
        exec ( counter) ;
	};
    void exec( atomic_t & counter );
//---------------------------------------------------------------------------
};// end struct backbone
//---------------------------------------------------------------------------
//
//############################################################################
//                                                                          ##
//                                                                          ##
//            N O N     I N L I N E      F U N C T I O N S                  ##
//                                                                          ##
//                                                                          ##
//############################################################################
//
template <uint32_t block_size,class iter_t,class compare>
thread_local typename std::iterator_traits<iter_t>::value_type*
backbone<block_size, iter_t, compare>::buf= nullptr ;

//------------------------------------------------------------------------
//  function : backbone
/// @brief constructor of the class
/// @param [in] pos : position of the range
/// @return iterator to the first position of the block
//------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
backbone <block_size, iter_t, compare>::
backbone ( iter_t first, iter_t last , compare comp):
           global_range ( first, last ), cmp(comp)
{   //------------------------- begin ---------------------------------
    assert ( (last-first) >= 0 ) ;
    if ( first == last ) return ; //nothing to do

    nelem = size_t ( last - first ) ;
    nblock = (nelem + block_size -1 ) / block_size ;
    ntail = (nelem %block_size );
    index.reserve (nblock+1) ;

    for ( size_t i =0 ; i < nblock;++i)
    	index.emplace_back( block_pos(i));

    range_tail.first = (ntail ==0)?last:(first+ ((nblock-1) * block_size)) ;
    range_tail.last = last ;
};
//-------------------------------------------------------------------------
//  function : exec
/// @brief begin with the execution of the functions stored in works
/// @param [in] counter : atomic counter. When 0 exits the function
/// @return range
//-------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
void backbone <block_size, iter_t, compare>::exec( atomic_t & counter )
{   //------------------------ begin --------------------------------
 	std::function <void (void) >  func_exec ;
    while ( util::atomic_read (counter) != 0)
    {   if ( works.pop_move_back( func_exec) ) func_exec() ;
        else std::this_thread::yield() ;
    };
};
//
//****************************************************************************
};//    End namespace bis
};//    End namespace detail
};//    End namespace sort
};//    End namespace parallel
};//    End namespace hpx
//****************************************************************************
//
#endif
