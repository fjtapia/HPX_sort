//----------------------------------------------------------------------------
/// @file move_blocks.hpp
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
#ifndef  __HPX_PARALLEL_SORT_DETAIL_BIS_MOVE_BLOCKS_HPP
#define  __HPX_PARALLEL_SORT_DETAIL_BIS_MOVE_BLOCKS_HPP

#include <atomic>
#include <future>
#include <iterator>
#include <iostream>
#include <hpx/parallel/sort/detail/bis/backbone.hpp>


namespace hpx		{
namespace parallel	{    
namespace sort		{
namespace detail	{
namespace bis       {

//
///---------------------------------------------------------------------------
/// @struct move_blocks
/// @brief This contains all the information shared betwen the classes of the
///        algorithm
/// @tparam
/// @remarks
//----------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
struct move_blocks
{
    //-------------------------------------------------------------------------
    //                  D E F I N I T I O N S
    //-------------------------------------------------------------------------
    typedef typename std::iterator_traits<iter_t>::value_type   value_t ;
    typedef std::atomic<uint32_t> 				                atomic_t ;
    typedef util::range <size_t>				                range_pos ;
    typedef util::range <iter_t>                                range_it ;
    typedef util::range <value_t*>                              range_buf ;
    typedef std::function <void (void) >                        function_t ;
    typedef backbone <block_size, iter_t, compare>				backbone_t ;

	//------------------------------------------------------------------------
	//                V A R I A B L E S
	//------------------------------------------------------------------------
    backbone_t &bk ;
    atomic_t counter ;

    //------------------------------------------------------------------------
    //                F U N C T I O N S
    //------------------------------------------------------------------------
    move_blocks ( backbone_t &bkb );

    void move_sequence ( std::vector<size_t> & seq );

    void move_long_sequence ( std::vector<size_t> & seq );
//---------------------------------------------------------------------------
};// end of struct move_blocks
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
//
//-------------------------------------------------------------------------
//  function : move_blocks
/// @brief constructor of the class for to move the blocks to their true
///        position obtained from the index
/// @param [in] bkb : backbone with the index and the blocks
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
move_blocks <block_size, group_size, iter_t, compare>::
move_blocks ( backbone_t &bkb ):bk(bkb),counter(0)
{   //------------------------- begin -------------------------------------
    std::vector < std::vector <size_t> > vsequence ;

    std::vector<size_t>  sequence ;
    size_t pos_index_ini =0 , pos_index_src =0, pos_index_dest =0 ;
    while ( pos_index_ini < bk.index.size() )
    {   while (pos_index_ini < bk.index.size() and
               bk.index[pos_index_ini].pos() == pos_index_ini)
        {   ++pos_index_ini;
        };
        if ( pos_index_ini == bk.index.size() ) break ;

        sequence.clear() ;
        pos_index_src = pos_index_dest = pos_index_ini ;
        sequence.push_back (pos_index_ini);

        while ( bk.index[ pos_index_dest].pos() != pos_index_ini )
        {   pos_index_src = bk.index[ pos_index_dest].pos();
            sequence.push_back ( pos_index_src) ;

            bk.index[pos_index_dest].pos( pos_index_dest);
            pos_index_dest = pos_index_src ;
        };
        bk.index[ pos_index_dest].pos( pos_index_dest);
        vsequence.push_back( sequence);
    };
    util::atomic_write ( counter , vsequence.size() );

    //-------------------------------------------------------------------
    // creation of the works
    //-------------------------------------------------------------------
    atomic_t son_counter ( vsequence.size() ) ;
    for ( size_t i =0 ; i < vsequence.size() ; ++i )
    {   if ( vsequence[i].size() < group_size )
        {   function_t f1 = [ = , &vsequence, &son_counter]( )
    	                    {	move_sequence(vsequence[i] );
                                util::atomic_sub ( son_counter, 1 ) ;
                            };
            bk.works.emplace_back ( f1 ) ;
        }
        else
        {   function_t f1 = [=, &vsequence, &son_counter]()
        	                {	move_long_sequence(vsequence[i]) ;
        						util::atomic_sub ( son_counter, 1 );
                            };
             bk.works.emplace_back ( f1 ) ;
        };
    };
    bk.exec ( son_counter) ;
};
//
//-------------------------------------------------------------------------
//  function : move_sequence
/// @brief make the indirect merge of the two range_pos inside RP
/// @param [in] pf : pointer to the atomic counter of the father
/// @param [in]	RP : rnage_pos which contains the two ranges to merge
/// @param [in]	Rbuf : auxiliary memory for to do the merge
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
void move_blocks <block_size, group_size, iter_t, compare>::
move_sequence ( std::vector<size_t> & seq )
{   //---------------------- begin -------------------------------------
    range_buf  rbuf = bk.get_range_buf () ;
    size_t pos_range2 = seq[0] ;

    range_it range2 = bk.get_range ( pos_range2);
    init_move ( rbuf, range2) ;
    for ( size_t i = 1 ; i < seq.size() ; ++i )
    {   pos_range2 = seq[i] ;
        range_it range1 ( range2) ;
        range2 = bk.get_range ( pos_range2 ) ;
        init_move ( range1, range2 ) ;
    };
    init_move ( range2, rbuf ) ;
};

//
//-------------------------------------------------------------------------
//  function : move_long_sequence
/// @brief make the indirect merge of the two range_pos inside RP
/// @param [in] pf : pointer to the atomic counter of the father
/// @param [in]	RP : rnage_pos which contains the two ranges to merge
/// @param [in]	Rbuf : auxiliary memory for to do the merge
//-------------------------------------------------------------------------
template <uint32_t block_size, uint32_t group_size, class iter_t, class compare>
void move_blocks <block_size, group_size, iter_t, compare>::
move_long_sequence ( std::vector<size_t> & seq )
{   //--------------------------------- begin ----------------------------
    if ( seq.size() < group_size )
        return move_sequence ( seq) ;

    size_t npart = ( seq.size() + group_size -1) / group_size ;
    size_t size_part = seq.size() / npart ;

    // vectors for the sequences and the final sequence
    std::vector< std::vector < size_t> > vseq ;
    vseq.reserve ( npart ) ;

    std::vector < size_t> index_seq ;
    index_seq.reserve ( npart ) ;

    auto it_pos = seq.begin() ;
    for ( size_t i =0 ; i < (npart -1) ; ++i, it_pos += size_part)
    {   vseq.emplace_back ( it_pos, it_pos + size_part) ;
        index_seq.emplace_back ( * ( it_pos+ size_part -1) ) ;
    };
    vseq.emplace_back ( it_pos , seq.end() ) ;
    index_seq.emplace_back ( seq.back() ) ;

    // creation of the functions
    atomic_t son_counter ( vseq.size() ) ;
    for ( size_t i =0 ; i < vseq.size() ; ++i )
    {   function_t f1=[=, &vseq, &son_counter]()
    	              {	move_sequence( vseq[i] ) ;
    	                util::atomic_sub ( son_counter, 1 );
                      };
        bk.works.emplace_back (f1 ) ;
    };
    bk.exec ( son_counter) ;
    move_long_sequence ( index_seq ) ;
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
