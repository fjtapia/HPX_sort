//----------------------------------------------------------------------------
/// @file indirect.hpp
/// @brief Indirect algorithm
///
/// @author Copyright (c) 2010 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#ifndef __SORT_ALGORITHM_INDIRECT_HPP
#define __SORT_ALGORITHM_INDIRECT_HPP

#include <hpx/parallel/sort/tools/algorithm.hpp>
#include <hpx/parallel/sort/tools/atomic.hpp>
#include <hpx/parallel/sort/tools/util_iterator.hpp>
#include <vector>
#include <type_traits>
#include <functional>

namespace hpx
{
namespace parallel
{
namespace _sort
{
namespace algorithm
{

//
//-----------------------------------------------------------------------------
//  function : create_index
/// @brief Create a index of iterators to the elements
/// @tparam iter_t : iterator to store in the index vector
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the element after the last of the range
/// @param [in/out] VP : vector where store the iterators of the index
//-----------------------------------------------------------------------------
template <class iter_t>
void create_index (iter_t first, iter_t last, std::vector<iter_t> &VP )
{   //----------------------------- begin -------------------------------
    auto N1 = last-first ;
    assert ( N1 >= 0 );
    VP.clear() ;
    VP.reserve ( N1);
    for ( ; first != last ; ++first) VP.push_back( first);
};
//
//-----------------------------------------------------------------------------
//  function : sort_index
/// @brief sort the elements according of the sort of the index
/// @tparam iter_t : iterators of the index
/// @param [in] first : iterator to the first element of the data
/// @param [in] VP : vector sorted of the iterators
//-----------------------------------------------------------------------------
template <class iter_t>
void sort_index (iter_t first, std::vector<iter_t> &VP )
{   //-------------------------- begin -------------------------------------
    typedef typename tools::iter_value<iter_t>::type       value_t ;
    size_t Ax  = 0 , Bx =0 , Pos =0 , N = VP.size();
    iter_t itA, itB ;
    while ( Pos < N )
    {   while  ( Pos < N and (size_t (VP[Pos]-first)) == Pos ) ++Pos;
        if ( Pos == N ) return ;
        Ax = Bx = Pos ;
        itA = first + Ax  ;
        value_t Aux = std::move ( *itA);
        while ( (Bx = (size_t (VP[Ax]-first)))!= Pos  )
        {   VP[Ax] = itA;
            itB = first + Bx ;
            *itA = std::move ( *itB);
            itA = itB ;
            Ax = Bx ;
        };
        *itA = std::move ( Aux ) ;
        VP[Ax] = itA ;
        ++Pos ;
    };
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
