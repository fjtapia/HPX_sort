//----------------------------------------------------------------------------
/// @file block.hpp
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
#ifndef  __HPX_PARALLEL_SORT_DETAIL_BIS_BLOCK_HPP
#define  __HPX_PARALLEL_SORT_DETAIL_BIS_BLOCK_HPP


#include <hpx/parallel/algorithms/tr1/detail/util/range.hpp>

namespace hpx       {
namespace parallel  {
namespace tr1       {
namespace detail    {
namespace bis       {


using hpx::parallel::tr1::detail::util::range ;
//
//---------------------------------------------------------------------------
/// @struct block_pos
/// @brief represent a group of N contiguous elements, beginning with the
///        pointed by first
/// @tparam
/// @remarks
//----------------------------------------------------------------------------
class block_pos
{   //------------------------------ VARIABLES -----------------------------
	size_t num ;

public :
	//----------------------------- FUNCTIONS ------------------------------
    block_pos ( void): num(0){};
    block_pos (size_t position , bool side = false)
    {	num = (position << 1) + ((side)?1:0);
    };
    block_pos operator = ( size_t position )
    {   num = (position <<1) + (num&1);
        return *this ;
    };

    size_t pos (void) const{ return ( num>>1);};
    void   pos (size_t position ){ num = (position <<1) + (num&1);};

    bool side ( void) const { return ( (num &1) !=0 ) ;};
    void side ( bool b ){ num = ( num & ~1 ) + (b) ?1:0 ;};
    uint32_t code ( void)const { return ( num &1) ;};
    void code ( uint32_t  cd ){ num = ( num & ~1 ) + (cd &1) ;};
//---------------------------------------------------------------------------
}; // end struct block_pos
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
/// @struct block
/// @brief represent a group of N contiguous elements, beginning with the
///        pointed by first
/// @tparam
/// @remarks
//----------------------------------------------------------------------------
template <uint32_t block_size, class iter_t>
struct block
{   typedef range <iter_t>   range_it ;
    //------------------------------ VARIABLES ------------------------------
	iter_t first ;

	//-------------------------- FUNCTIONS --------------------------------
    block ( iter_t  it ):first(it){};

    iter_t 	 front     (void) const { return first ;};
    iter_t 	 back      (void) const { return first + block_size ;};
    range_it get_range (void)       { return range_it(first,first+block_size);};

//----------------------------------------------------------------------------
};// end struct block
//----------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
//  function : compare_block
/// @brief compare two blocks using the content of the pointed by first
/// @param [in] b1 : first block to compare
/// @param [in] b2 : second block to compare
/// @param [in] cmp : comparison operator
//-------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
bool compare_block ( block<block_size, iter_t> b1,
		                    block<block_size, iter_t>  b2 ,
							compare cmp = compare())
{   //---------------------------- begin ----------------------------------
    return cmp (*b1.first, *b2.first ) ;
};
//
///---------------------------------------------------------------------------
/// @struct compare_block_pos
/// @brief This is a object for to compare two blocks from their positions
///        in the index
//----------------------------------------------------------------------------
template <uint32_t block_size, class iter_t, class compare>
struct compare_block_pos
{   //-------------------------- VARIABLES --------------------------------
	iter_t first ;
    compare comp ;

    //-------------------------  FUNCTIONS -----------------------------------
    compare_block_pos ( iter_t f1, compare  cmp ):first (f1),comp(cmp){};

    inline bool operator() ( block_pos b1, block_pos b2 )const
    {   return comp ( *(first + (b1.pos() * block_size)),
    			      *(first + (b2.pos() * block_size))   ) ;
    };
//---------------------------------------------------------------------------
};// end struct compare_block_pos
//---------------------------------------------------------------------------
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
