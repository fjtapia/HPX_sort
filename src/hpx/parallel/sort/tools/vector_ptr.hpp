//----------------------------------------------------------------------------
/// @file vector_ptr.hpp
/// @brief
///
/// @author Copyright (c) 2010 2013 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//
//  template <class T , class Allocator>
//  class allocator_delete
//
//  template <class  allocator_t >
//  class vector_ptr
//
//  template <class  allocator_t >
//  class vector_ptr_crude
//
//  template <class iter_t ,class comp_t  >
//  iter_t delete_repeated_sorted ( iter_t first , iter_t last, comp_t comp )
//
//  template <class iter_t>
//  iter_t delete_nullptr_end (iter_t first, iter_t last )
//
//  template <class iter_t>
//  iter_t delete_nullptr (iter_t first , iter_t last )
//
//-----------------------------------------------------------------------------
#ifndef __CNTREE_UTIL_VECTOR_PTR_HPP
#define __CNTREE_UTIL_VECTOR_PTR_HPP

#include <vector>
#include <memory>
#include <boost/countertree/util/config.hpp>


namespace boost
{
namespace countertree
{
namespace util
{
//
//##########################################################################
//                                                                        ##
//         c l a s s     a l l o c a t o r _ d e l e t e                  ##
//                                                                        ##
//##########################################################################
//---------------------------------------------------------------------------
/// @class allocator_delete
///
/// @remarks This class is for to obtain pointers with nullptr value by default
//---------------------------------------------------------------------------
template <class T , class Allocator>
class allocator_delete
{   Allocator alloc ;

public:
    allocator_delete ( const Allocator & A1 = Allocator()):alloc(A1){} ;
    void operator () ( T * ptr)
    {   assert ( ptr != nullptr);
        alloc.destroy ( ptr);
        alloc.deallocate ( ptr , 1 );
    };
};
//
//##########################################################################
//                                                                        ##
//                 C L A S S     V E C T O R _ P T R                      ##
//                                                                        ##
//##########################################################################
//---------------------------------------------------------------------------
/// @class vector_ptr
///
/// @remarks This class have a vector of pointers to object value_t
///          extracted from the definition of the allocator.
///          This class have an allocator for to delete the objects pointed,
///          when a exception is throwed and the objects must be deleted
//---------------------------------------------------------------------------
template <class  allocator_t >
class vector_ptr
{
public:
//***************************************************************************
//          D E F I N I T I O N S    A N D   T Y P E    C H E C K
//***************************************************************************
typedef typename allocator_t::value_type            value_t ;
typedef typename allocator_t::size_type             size_type ;
typedef typename allocator_t::difference_type       difference_type;
typedef typename std::vector<value_t*,allocator_t>  vector_t ;
typedef typename vector_t::iterator                 iterator ;
typedef typename vector_t::const_iterator           const_iterator ;
typedef value_t *                                   pvalue_t ;
typedef const value_t *                             cpvalue_t ;

private:
//***************************************************************************
//             P R I V A T E      V A R I A B L E S
//***************************************************************************
std::vector< pvalue_t, allocator_t > V ;
allocator_t  alloc ;

public:
//##########################################################################
//                                                                        ##
//                                                                        ##
//                   P U B L I C    F U N C T I O N S                     ##
//                                                                        ##
//                                                                        ##
//##########################################################################
vector_ptr ( const vector_ptr & ) = delete ;
vector_ptr & operator = ( const vector_ptr &) = delete ;


vector_ptr ( allocator_t  sb1 = allocator_t())noexcept : alloc( sb1){  };

vector_ptr ( size_type N1 ,allocator_t sb1= allocator_t())
            : V(N1, nullptr), alloc( sb1){  };

virtual ~vector_ptr ( ){ clear() ;};


 pvalue_t  & operator [] ( size_t pos)      { return V[pos] ;};
cpvalue_t  & operator [] ( size_t pos)const { return V[pos] ;};

 pvalue_t  &at ( size_t pos)      {  return V.at(pos) ;};
cpvalue_t  &at ( size_t pos)const {  return V.at(pos) ;};

void push_back ( pvalue_t  P1){ V.push_back(P1);};

 pvalue_t & back ()      { return V.back();};
cpvalue_t & back ()const { return V.back();};

      iterator begin ( void)      { return V.begin(); };
const_iterator begin ( void)const { return V.cbegin();};

      iterator end  ( void)      { return V.end(); };
const_iterator end  ( void)const { return V.cend();};

void        reserve ( size_type new_cap ){ V.reserve ( new_cap);};
size_type   size    ( ) const            { return V.size();};
size_type   capacity( ) const            { return V.capacity();};

allocator_t & get_alloc( void){ return alloc;};

void resize( size_t count )
{   //--------------------- begin ---------------------
    if ( count < V.size())
    {   for ( size_t i = count ; i < V.size() ;++i)
        {   if ( V[i] != nullptr)
            {   alloc.destroy ( V[i]);
                alloc.deallocate ( V[i], 1);
                V[i] = nullptr ;
            };
        };
    };
    V.resize ( count, nullptr) ;
};

void clear ()
{   //------------------- begin ----------------------
    for ( size_type i =0 ; i < V.size() ;++i)
    {   if ( V[i] != nullptr)
        {   alloc.destroy ( V[i]);
            alloc.deallocate( V[i],1);
            V[i] = nullptr ;
        };
    };
    V.clear() ;
};


//***************************************************************************
};// E N D    C L A S S     V E C T O R _ P T R
//***************************************************************************

//
//##########################################################################
//                                                                        ##
//                 C L A S S     V E C T O R _ P T R _ C R U D E          ##
//                                                                        ##
//##########################################################################
//---------------------------------------------------------------------------
/// @class vector_ptr_crude
///
/// @remarks This class have a vector of pointers to object without
///          initialization, extracted from the definition of the
///          allocator.
///          This class have an allocator for to deallocate the objects pointed,
///          when a exception is throwed and the objects must be deleted
//---------------------------------------------------------------------------
template <class  allocator_t >
class vector_ptr_crude
{
public:
//***************************************************************************
//          D E F I N I T I O N S    A N D   T Y P E    C H E C K
//***************************************************************************
typedef typename allocator_t::value_type            value_t ;
typedef typename allocator_t::size_type             size_type ;
typedef typename allocator_t::difference_type       difference_type;
typedef typename std::vector<value_t*,allocator_t>  vector_t ;
typedef typename vector_t::iterator                 iterator ;
typedef typename vector_t::const_iterator           const_iterator ;

private:
//***************************************************************************
//             P R I V A T E      V A R I A B L E S
//***************************************************************************
std::vector< value_t *, allocator_t > V ;
allocator_t  alloc ;

public:
//##########################################################################
//                                                                        ##
//                                                                        ##
//                   P U B L I C    F U N C T I O N S                     ##
//                                                                        ##
//                                                                        ##
//##########################################################################
vector_ptr_crude ( const vector_ptr_crude & ) = delete ;
vector_ptr_crude & operator = ( const vector_ptr_crude &) = delete ;

//-----------------------------------------------------------------------------
//  function : vector_ptr
/// @brief Constructor of the class
/// @param [in] sb1: allocator for to create the internal allocator
/// @exception none
/// @remarks
//-----------------------------------------------------------------------------
vector_ptr_crude ( allocator_t  sb1 = allocator_t())noexcept : alloc( sb1){  };
//-----------------------------------------------------------------------------
//  function : vector_ptr
/// @brief COnstructor of the class
/// @param [in] N1 : number of null pointers in the internal vector
/// @param [in] sb1: allocator for to create the internal allocator
/// @exception bad_alloc if fail the allocation of the vector
/// @remarks
//-----------------------------------------------------------------------------
vector_ptr_crude ( size_type N1 ,allocator_t sb1= allocator_t())
            : V(N1, nullptr), alloc( sb1){  };
//-----------------------------------------------------------------------------
//  function :~vector_ptr_crude
/// @brief Destructor of the class
/// @param [in/out]
/// @exception bad_alloc if fail the deallocation of the objects, or the
///                      deallocation of the vector
/// @remarks
//-----------------------------------------------------------------------------
virtual ~vector_ptr_crude ( ){ clear() ;};


      value_t * &operator [] ( size_t pos)     { return V[pos] ;};
const value_t * &operator [] ( size_t pos)const{ return V[pos] ;};

      value_t * &at ( size_t pos)      { return V.at(pos) ; };
const value_t * &at ( size_t pos)const { return V.at(pos) ; };

void push_back ( value_t * P1){ V.push_back(P1);};

      value_t *& back ()      { return V.back();};
const value_t *& back ()const { return V.back();};

iterator begin( void){ return V.begin();};
iterator end  ( void) { return V.end();};

const_iterator begin ( void)const{ return V.cbegin();};
const_iterator end   ( void)const { return V.cend();};

void reserve( size_type new_cap ){ V.reserve ( new_cap);};

size_t size( ) const{ return V.size();};

allocator_t & get_alloc( void){ return alloc;};

size_type capacity() const{ return V.capacity();};

void resize( size_t count )
{   //--------------------- begin ---------------------
    if ( count < V.size())
    {   for ( size_t i = count ; i < V.size() ;++i)
        {   if ( V[i] != nullptr)
            {   //alloc.destroy ( V[i]);
                alloc.deallocate ( V[i], 1);
                V[i] = nullptr ;
            };
        };
    };
    V.resize ( count, nullptr) ;
};

void clear ()
{   //------------------- begin ----------------------
    for ( size_type i =0 ; i < V.size() ;++i)
    {   if ( V[i] != nullptr)
        {   //alloc.destroy ( V[i]);
            alloc.deallocate( V[i],1);
            V[i] = nullptr ;
        };
    };
    V.clear() ;
};


//***************************************************************************
};// E N D    C L A S S     V E C T O R _ P T R _ C R U D E
//***************************************************************************

//-----------------------------------------------------------------------------
//  function :delete_repeated_sorted
/// @brief : delete the repeated elements in a sorted data structure
/// @param [in] first: iterator to the first element to examine
/// @param [in] last : iterator to the last element to examina
/// @param [in] comp : comparison function
/// @exception iterator to the first element repeated. Then the valid range is
///            [ first, #return) , and the range of the repeated elements is
///            [ #return , last).
/// @return
/// @remarks This algorithm run well witha objects and pointers
//-----------------------------------------------------------------------------
template <class iter_t ,class comp_t  >
iter_t delete_repeated_sorted ( iter_t first , iter_t last, comp_t comp )
{   //-------------------------begin-------------------------------
    auto N = last-first ;
    assert ( N >= 0 );
    if ( N < 2 ) return last ;

    iter_t Alfa = first, Beta = first +1 ;

    for (; Beta != last and comp (*Alfa, *Beta); ++Alfa , ++Beta) ;
    if ( Beta == last ) return Beta ;

    while ( Beta != last )
    {   if ( comp( *Alfa , *Beta))
			std::swap ( *(Alfa++), *Beta ) ;
		++Beta ;
    };
    return ( Alfa +1);
}

//-----------------------------------------------------------------------------
//  function : delete_nullptr_end
/// @brief return the iterator to the first element no null beginning to
///        scan by the last iterator
/// @param [in] first : iterator to the first element of the range
/// @param [in] last : iterator to the last element of the range
/// @return iterator to the first element null, beginnin to scan by the end
///         Then the valid range is [ first, #return) , and the range of the
///         nullptr elements is  [ #return , last).
/// @remarks
//-----------------------------------------------------------------------------
template <class iter_t>
iter_t delete_nullptr_end (iter_t first, iter_t last )
{   //---------------------------- begin ------------------
    auto N = last-first ;
    assert ( N >= 0 );
    if ( N < 2 ) return last ;

    iter_t Alfa = first + ( N-1), Beta = last ;
    while ( *Alfa == nullptr and Beta != first ) Beta = Alfa--;
    return Beta ;
};


//-----------------------------------------------------------------------------
//  function : delete_nullptr
/// @brief put all the elements with a null value to the end, and return a
///        pointer to the first null element
/// @param [in] first :iterator to the first element of the range
/// @param [in] last :iterator to the last element of the range
/// @return iterator to the first element null, after puhed to the end
/// @remarks
//-----------------------------------------------------------------------------
template <class iter_t>
iter_t delete_nullptr (iter_t first , iter_t last )
{   //------------------------- begin --------------------
    auto N = last-first ;
    assert ( N >= 0 );
    if ( N < 2 ) return last    ;

    iter_t Alfa = first ;
    for ( ; Alfa != last and *Alfa != nullptr ; ++Alfa);

    iter_t Beta = Alfa -- ;

    while (Beta != last )
    {   if ( *Beta != nullptr)
        {   *(Alfa++)  = *Beta ;
            *Beta = nullptr ;
        };
        ++Beta ;
    }
    return Alfa ;
};


//***************************************************************************
};//    End namespace util
};//    End namespace countertree
};//    End namespace boost
//***************************************************************************
#endif
