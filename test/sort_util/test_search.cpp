//----------------------------------------------------------------------------
/// @file  test_search.cpp
/// @brief Test program for the search functions
///
/// @author Copyright (c) 2010 2015 Francisco José Tapia (fjtapia@gmail.com )\n
///         Distributed under the Boost Software License, Version 1.0.\n
///         ( See accompanyingfile LICENSE_1_0.txt or copy at
///           http://www.boost.org/LICENSE_1_0.txt  )
/// @version 0.1
///
/// @remarks
//-----------------------------------------------------------------------------
#include <iostream>
#include <boost/sort/util/search.hpp>

using namespace boost::sort::util ;


int main ( void)
{   //-------------------------------- begin ---------------------------------
    uint64_t A[]= {3,3 ,7,9,11,11,11,17,19,19 };

    //------------------------------------------------------------------------
    //                    FIND_FIRST_REP
    //------------------------------------------------------------------------
    uint64_t * Alfa = find_first_rep ( &A[0], &A[10], 3);
    if ( *Alfa != 3 or *(Alfa+1) != 3)
        std::cout<<"Error in find_first_rep\n";

    Alfa = find_first_rep ( &A[0], &A[10], 19);
    if ( *Alfa != 19 or *(Alfa+1) != 19)
        std::cout<<"Error in find_first_rep\n";

    Alfa = find_first_rep ( &A[0], &A[10], 7);
    if ( *Alfa != 7)
        std::cout<<"Error in find_first_rep\n";

    Alfa = find_first_rep ( &A[0], &A[10], 8);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_first_rep\n";

    Alfa = find_first_rep ( &A[0], &A[10], 20);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_first_rep\n";

    Alfa = find_first_rep ( &A[0], &A[10], 1);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_first_rep\n";

    //------------------------------------------------------------------------
    //                          FIND_LAST_REP
    //------------------------------------------------------------------------
    Alfa = find_last_rep ( &A[0], &A[10], 3);
    if ( *Alfa != 3 or *(Alfa+1) != 7)
        std::cout<<"Error in find_last_rep\n";

    Alfa = find_last_rep ( &A[0], &A[10], 19);
    if ( *Alfa != 19 or *(Alfa-1) != 19)
        std::cout<<"Error in find_last_rep\n";

    Alfa = find_last_rep ( &A[0], &A[10], 7);
    if ( *Alfa != 7 )
        std::cout<<"Error in find_last_rep\n";

    Alfa = find_last_rep ( &A[0], &A[10], 8);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_last_rep\n";

    Alfa = find_last_rep ( &A[0], &A[10], 20);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_last_rep\n";

    Alfa = find_last_rep ( &A[0], &A[10], 1);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_last_rep\n";


    //------------------------------------------------------------------------
    //                    FIND_EQUAL_GREATER
    //------------------------------------------------------------------------
    Alfa = find_equal_greater ( &A[0], &A[10], 3);
    if ( *Alfa != 3 or *(Alfa+1) != 3)
        std::cout<<"Error in find_equal_greater\n";

    Alfa = find_equal_greater ( &A[0], &A[10], 19);
    if ( *Alfa != 19 or *(Alfa+1) != 19)
        std::cout<<"Error in find_equal_greater\n";

    Alfa = find_equal_greater ( &A[0], &A[10], 7);
    if ( *Alfa != 7)
        std::cout<<"Error in find_equal_greater\n";

    Alfa = find_equal_greater ( &A[0], &A[10], 8);
    if ( *Alfa != 9 )
        std::cout<<"Error in find_equal_greater\n";

    Alfa = find_equal_greater ( &A[0], &A[10], 20);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_equal_greater\n";

    Alfa = find_equal_greater ( &A[0], &A[10], 1);
    if ( *Alfa != 3 )
        std::cout<<"Error in find_equal_greater\n";


    //------------------------------------------------------------------------
    //                          FIND_EQUAL_LESS
    //------------------------------------------------------------------------
    Alfa = find_equal_less ( &A[0], &A[10], 3);
    if ( *Alfa != 3 or *(Alfa+1) != 7)
        std::cout<<"Error in find_equal_less\n";

    Alfa = find_equal_less ( &A[0], &A[10], 19);
    if ( *Alfa != 19 or *(Alfa-1) != 19)
        std::cout<<"Error in find_equal_less\n";

    Alfa = find_equal_less ( &A[0], &A[10], 7);
    if ( *Alfa != 7 )
        std::cout<<"Error in find_equal_less\n";

    Alfa = find_equal_less ( &A[0], &A[10], 8);
    if ( *Alfa != 7 )
        std::cout<<"Error in find_equal_less\n";

    Alfa = find_equal_less ( &A[0], &A[10], 20);
    if ( *Alfa != 19 and *(Alfa+1) != 19 )
        std::cout<<"Error in find_equal_less\n";

    Alfa = find_equal_less ( &A[0], &A[10], 1);
    if ( Alfa != &A[10] )
        std::cout<<"Error in find_equal_less\n";

    //------------------------------------------------------------------------
    //                          LOWER_BOUND
    //------------------------------------------------------------------------
    Alfa = lower_bound ( &A[0], &A[10], 3);
    if ( *Alfa != 3 or *(Alfa+1) != 3)
        std::cout<<"Error in lower_bound\n";

    Alfa = lower_bound ( &A[0], &A[10], 19);
    if ( *Alfa != 19 or *(Alfa+1) != 19)
        std::cout<<"Error in lower_bound\n";

    Alfa = lower_bound ( &A[0], &A[10], 7);
    if ( *Alfa != 7 )
        std::cout<<"Error in lower_bound\n";

    Alfa = lower_bound ( &A[0], &A[10], 8);
    if ( *Alfa != 9 )
        std::cout<<"Error in lower_bound\n";

    Alfa = lower_bound ( &A[0], &A[10], 20);
    if ( Alfa != &A[10] )
        std::cout<<"Error in lower_bound\n";

    Alfa = lower_bound ( &A[0], &A[10], 1);
    if ( *Alfa != 3 and * (Alfa +1) != 3 )
        std::cout<<"Error in lower_bound\n";

    //------------------------------------------------------------------------
    //                          UPPER_BOUND
    //------------------------------------------------------------------------
    Alfa = upper_bound ( &A[0], &A[10], 3);
    if ( *Alfa != 7)
        std::cout<<"Error in upper_bound\n";

    Alfa = upper_bound ( &A[0], &A[10], 19);
    if ( Alfa != &A[10])
        std::cout<<"Error in upper_bound\n";

    Alfa = upper_bound ( &A[0], &A[10], 7);
    if ( *Alfa != 9)
        std::cout<<"Error in upper_bound\n";

    Alfa = upper_bound ( &A[0], &A[10], 8);
    if ( *Alfa != 9 )
        std::cout<<"Error in upper_bound\n";

    Alfa = upper_bound ( &A[0], &A[10], 20);
    if ( Alfa != &A[10] )
        std::cout<<"Error in upper_bound\n";

    Alfa = upper_bound ( &A[0], &A[10], 1);
    if ( *Alfa != 3 )
        std::cout<<"Error in upper_bound\n";

    return 0 ;
};
