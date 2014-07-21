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

#include "is_class_test.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(is_class_test);

//-----------------------------------------------------------------------------
void is_class_test::setUp()
{
    file = h5::nxfile::create_file("is_valid.nx",true,0);
    root = file.root();
    group = root.create_group("group","NXentry");
    field = root.create_field<uint32>("data");
}

//-----------------------------------------------------------------------------
void is_class_test::tearDown() 
{ 
    field.close();
    group.close();
    root.close();
    file.close();
}


//-----------------------------------------------------------------------------
void is_class_test::test_group()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    h5::nxobject object = group;
    CPPUNIT_ASSERT(pni::io::nx::is_class(object,"NXentry"));
    CPPUNIT_ASSERT(!pni::io::nx::is_class(object,"NXlog"));
}

//-----------------------------------------------------------------------------
void is_class_test::test_field()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject object = field;
    CPPUNIT_ASSERT_THROW(pni::io::nx::is_class(object,"NXentry"),type_error);
}

//-----------------------------------------------------------------------------
void is_class_test::test_attribute()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject object = group.attributes["NX_class"];
    CPPUNIT_ASSERT_THROW(pni::io::nx::is_class(object,"NXentry"),type_error);
}

