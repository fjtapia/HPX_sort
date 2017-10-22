//----------------------------------------------------------------------------
/// @file merge_blocks.hpp
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
#ifndef  __HPX_PARALLEL_SORT_DETAIL_BIS_MERGE_BLOCKS_HPP
#define  __HPX_PARALLEL_SORT_DETAIL_BIS_MERGE_BLOCKS_HPP

#include <atomic>
#include <future>
#include <iterator>
#include <iostream>
#include <hpx/parallel/algorithms/tr1/detail/bis/backbone.hpp>


namespace hpx       {
namespace parallel  {
namespace tr1       {
namespace detail    {
namespace bis       {

//
///---------------------------------------------------------------------------
/// @struct merge_blocks
/// @brief This contains all the information shared betwen the classes of the
///        algorithm
/// @tparam
/// @remarks
//----------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
struct merge_blocks
{
    //----------------------------------------------------------------------------
    //                  D E F I N I T I O N S
    //----------------------------------------------------------------------------
    typedef typename std::iterator_traits<iter_t>::value_type   value_t ;
    typedef std::atomic<uint32_t> 				                atomic_t ;
    typedef util::range <size_t>				                range_pos ;
    typedef util::range <iter_t>                                range_it ;
    typedef util::range <value_t*>                              range_buf ;
    typedef std::function <void (void) >                        function_t ;
    typedef backbone<block_size, iter_t, compare>				backbone_t;
    typedef compare_block_pos <block_size, iter_t, compare> compare_block_pos_t ;

	//------------------------------------------------------------------------
	//                V A R I A B L E S
	//------------------------------------------------------------------------
    backbone_t &bk ;
    atomic_t counter ;
    //
    //------------------------------------------------------------------------
    //                F U N C T I O N S
    //------------------------------------------------------------------------
    merge_blocks ( backbone_t &bkb, size_t pos_index1, size_t pos_index2,
                   size_t pos_index3 );

    void tail_process ( std::vector<block_pos> & vblkpos1,
                        std::vector<block_pos> & vblkpos2  );

    void cut_range  (range_pos rng , std::vector <range_pos> & vrpos_out);

    void merge_range_pos ( range_pos rng );

//----------------------------------------------------------------------------
};// end struct merge_blocks
//----------------------------------------------------------------------------
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
//-------------------------------------------------------------------------
//  function : merge_blocks
/// @brief make the indirect merge of the two range_pos inside RP
/// @param [in] pf : pointer to the atomic counter of the father
/// @param [in]	RP : rnage_pos which contains the two ranges to merge
/// @param [in]	Rbuf : auxiliary memory for to do the merge
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
merge_blocks < block_size, group_size, iter_t, compare>::
merge_blocks ( backbone_t &bkb, size_t pos_index1, size_t pos_index2,
               size_t pos_index3 ):bk(bkb),counter(0)
{   //----------------------------- begin --------------------------------
    size_t nblock1 = pos_index2 - pos_index1 ;
    size_t nblock2 = pos_index3 - pos_index2 ;
    if ( nblock1 == 0 or nblock2 == 0 ) return ;

    //-----------------------------------------------------------------------
    // Merging of the two intervals
    //-----------------------------------------------------------------------
    std::vector<block_pos> vpos1, vpos2 ;
    vpos1.reserve ( nblock1 +1 );
    vpos2.reserve ( nblock2 +1 );

    for ( size_t i = pos_index1 ; i < pos_index2 ; ++i)
    {   vpos1.emplace_back( bk.index[i].pos(), true) ;
    };

    for ( size_t i = pos_index2; i < pos_index3 ; ++i)
    {   vpos2.emplace_back( bk.index[i].pos(), false) ;
    };
    //-------------------------------------------------------------------
    //  tail process
    //-------------------------------------------------------------------
    if ( vpos2.back().pos() == (bk.nblock -1) and
         bk.range_tail.first != bk.range_tail.last)
    {   tail_process ( vpos1, vpos2);
      	nblock1 = vpos1.size();
       	nblock2 = vpos2.size() ;
    };

    compare_block_pos_t  cmp_blk ( bk.global_range.first, bk.cmp) ;

    util::full_merge (vpos1.begin() , vpos1.end(),
                vpos2.begin() , vpos2.end(),
                bk.index.begin() + pos_index1, cmp_blk);

    //--------------------------------------------------------------------
    // Extracting the ranges for to merge the elements
    //--------------------------------------------------------------------
    std::vector <range_pos> vrange_pos ;
    cut_range (range_pos(pos_index1,pos_index1+nblock1+nblock2),vrange_pos);

    //-------------------------------------------------------------------
    //           parallel process
    //-------------------------------------------------------------------
    util::atomic_write ( counter , vrange_pos.size());
    for ( size_t i =0 ; i < vrange_pos.size() ; ++i)
    {	function_t f1=[=, &vrange_pos ]( )
     	              {	merge_range_pos ( vrange_pos[i]);
                        util::atomic_sub ( this->counter , 1);
     				  };
     	bk.works.emplace_back ( f1 );
    };
    bk.exec ( counter) ; // wait until finish all the ranges
};

//
//-------------------------------------------------------------------------
//  function : tail_process
/// @brief make the indirect merge of the two range_pos inside RP
/// @param [in] pf : pointer to the atomic counter of the father
/// @param [in]	RP : rnage_pos which contains the two ranges to merge
/// @param [in]	Rbuf : auxiliary memory for to do the merge
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
void merge_blocks < block_size, group_size, iter_t, compare>::
tail_process ( std::vector<block_pos> & vblkpos1,
                        std::vector<block_pos> & vblkpos2  )
{   //-------------------------- begin ---------------------------------
 	if ( vblkpos1.size() == 0 or vblkpos2.size() == 0 ) return ;

    vblkpos2.pop_back();

    size_t posback1 = vblkpos1.back().pos() ;
    range_it range_back1 = bk.get_range (posback1 );

    if ( util::is_mergeable (range_back1,bk.range_tail, bk.cmp))
    { 	util::in_place_merge_uncontiguous ( range_back1, bk.range_tail,
                                                bk.get_range_buf(), bk.cmp);
      	if ( vblkpos1.size() >1 )
      	{   //---------------------------------------------------------------
      		size_t pos_aux = vblkpos1[ vblkpos1.size() -2].pos() ;
       		range_it range_aux = bk.get_range ( pos_aux) ;

       		if ( util::is_mergeable( range_aux,range_back1, bk.cmp))
       		{	vblkpos2.emplace_back( posback1, false ) ;
               	vblkpos1.pop_back() ;
       		};
       	};
    };
};

//
//-------------------------------------------------------------------------
//  function : cut_range
/// @brief divide the range_pos rng in several ranges. The ranges obtained
///        are inserted in the vector of range_pos vrpos_out
/// @param [in] rng : range to divide
/// @param [in]	vrpos_out : vector where insert the ranges obtained in the
///                         division of rng
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
void merge_blocks < block_size, group_size, iter_t, compare>::
cut_range  (range_pos rng , std::vector <range_pos> & vrpos_out)
{   //---------------------------- begin -----------------------------
    if ( rng.size() < group_size )
    {   vrpos_out.push_back ( rng);
        return ;
    };
    size_t npart = (rng.size() + group_size -1 )/ group_size ;
    size_t size_part = rng.size() / npart ;

    size_t pos_ini = rng.first ;
    size_t pos_last = rng.last ;
    while ( pos_ini < pos_last)
    {	size_t pos = pos_ini + size_part ;
        while ( pos < pos_last and
         		bk.index[pos-1].side() == bk.index[pos].side())
        {	++pos ;
        };
        if ( pos < pos_last)
        {	 in_place_merge_uncontiguous
					 ( bk.get_range ( bk.index[pos-1].pos()),
					   bk.get_range ( bk.index[pos].pos() ),
					   bk.get_range_buf(), bk.cmp);
        }
        else pos = pos_last ;
        if ( (pos - pos_ini) > 1 )
        	vrpos_out.emplace_back(  pos_ini, pos);
        pos_ini = pos ;
    };
};

//
//-------------------------------------------------------------------------
//  function : merge_range_pos
/// @brief make the indirect merge of the two range_pos inside RP
/// @param [in] rng : rango de positions of ranges for to merge
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
void merge_blocks < block_size, group_size, iter_t, compare>::
merge_range_pos ( range_pos rng )
{   //------------------------- begin ------------------------------------
    if ( rng.size() < 2 ) return ;
    range_buf rbuf = bk.get_range_buf () ;

    range_it  rng_prev = bk.get_range ( bk.index[ rng.first ].pos());
    init_move ( rbuf, rng_prev);
    range_it rng_posx (rng_prev);

    for (size_t posx = rng.first +1 ; posx != rng.last ; ++posx )
    {	 rng_posx = bk.get_range ( bk.index[posx].pos());
     	 util::merge_flow ( rng_prev, rbuf, rng_posx, bk.cmp);
      	 rng_prev = rng_posx;

    }; // end while
    init_move (rng_posx, rbuf);
}; // end  merge_range_pos ( range_pos rng )
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
