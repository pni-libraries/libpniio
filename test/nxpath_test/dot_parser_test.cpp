//
// (c) Copyright 2014 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
//
// This file is part of libpniio.
//
// libpniio is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// libpniio is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libpniio.  If not, see <http://www.gnu.org/licenses/>.
// ===========================================================================
//
//  Created on: Jun 19, 2014
//      Author: Eugen Wintersberger
//

#include <boost/exception/diagnostic_information.hpp>
#include <boost/spirit/home/qi/parse.hpp>
#include "dot_parser_test.hpp"
#include "../EqualityCheck.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(dot_parser_test);

using namespace boost::spirit;

//-----------------------------------------------------------------------------
void dot_parser_test::setUp() { }

//-----------------------------------------------------------------------------
void dot_parser_test::tearDown() {}


//----------------------------------------------------------------------------
void dot_parser_test::test_one()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    set_input(".");

    CPPUNIT_ASSERT(qi::parse(start_iter,stop_iter,parser,output));
    
    CPPUNIT_ASSERT(output == ".");
}

//----------------------------------------------------------------------------
void dot_parser_test::test_two()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    set_input("..");

    CPPUNIT_ASSERT(qi::parse(start_iter,stop_iter,parser,output));
    
    CPPUNIT_ASSERT(output == "..");
}

//----------------------------------------------------------------------------
void dot_parser_test::test_many()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    set_input("...");

    CPPUNIT_ASSERT(qi::parse(start_iter,stop_iter,parser,output));
    
    CPPUNIT_ASSERT(output == "..");
    
}


