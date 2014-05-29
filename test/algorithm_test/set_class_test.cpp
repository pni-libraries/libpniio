//
// (c) Copyright 2013 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
//  Created on: Jun 28, 2013
//      Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#include <boost/current_function.hpp>
#include<cppunit/extensions/HelperMacros.h>

#include "set_class_test.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(set_class_test);

//-----------------------------------------------------------------------------
void set_class_test::setUp()
{
    file = h5::nxfile::create_file("is_valid.nx",true,0);
    root = file.root();
    group = root.create_group("group","NXentry");
    field = root.create_field<uint32>("data");
}

//-----------------------------------------------------------------------------
void set_class_test::tearDown() 
{ 
    field.close();
    group.close();
    root.close();
    file.close();
}


//-----------------------------------------------------------------------------
void set_class_test::test_group()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    h5::nxobject object = group;
    CPPUNIT_ASSERT(is_valid(object));
    CPPUNIT_ASSERT_NO_THROW(set_class(object,"NXlog"));
    string buffer;
    group.attr("NX_class").read(buffer);
    CPPUNIT_ASSERT(buffer == "NXlog");
}

//-----------------------------------------------------------------------------
void set_class_test::test_field()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject object = field;
    CPPUNIT_ASSERT_THROW(set_class(object,"NXentry"),nxfield_error);
}

//-----------------------------------------------------------------------------
void set_class_test::test_attribute()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject object = group.attr("NX_class");
    CPPUNIT_ASSERT_THROW(set_class(object,"NXentry"),nxattribute_error);
   
}
