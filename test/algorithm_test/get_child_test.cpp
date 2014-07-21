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

#include "get_child_test.hpp"


CPPUNIT_TEST_SUITE_REGISTRATION(get_child_test);

//-----------------------------------------------------------------------------
void get_child_test::setUp()
{
    file = h5::nxfile::create_file("groups.nx",true,0);
    root = file.root();
    group = root.create_group("scan_1","NXentry");
    group.create_group("beamline","NXinstrument");
    group.create_field<string>("date");
    group.create_field<string>("experiment_id");
    group.create_group("data","NXdata");
    group.create_group("control","NXmonitor");
    field = root.create_field<uint32>("data");
    attribute = group.attributes["NX_class"];
}

//-----------------------------------------------------------------------------
void get_child_test::tearDown() 
{ 
    attribute.close();
    field.close();
    group.close();
    root.close();
    file.close();
}

//-----------------------------------------------------------------------------
void get_child_test::test_group_by_name()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    h5::nxobject o = get_child(group,"control","");
    CPPUNIT_ASSERT(get_name(o)=="control");
    CPPUNIT_ASSERT(is_class(o,"NXmonitor"));

    CPPUNIT_ASSERT_THROW(get_child(group,"bla",""), key_error);

}

//----------------------------------------------------------------------------
void get_child_test::test_group_by_class()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject o = get_child(group,"","NXinstrument");
    CPPUNIT_ASSERT(get_name(o)=="beamline");
    CPPUNIT_ASSERT(is_class(o,"NXinstrument"));

    CPPUNIT_ASSERT_THROW(get_child(group,"","NXfoo"),key_error);
}

//----------------------------------------------------------------------------
void get_child_test::test_group_by_name_class()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject o = get_child(root,"scan_1","NXentry");
    CPPUNIT_ASSERT(get_name(o)=="scan_1");
    CPPUNIT_ASSERT(is_class(o,"NXentry"));

    CPPUNIT_ASSERT_THROW(get_child(root,"scan_1","NXfoo"),key_error);
    CPPUNIT_ASSERT_THROW(get_child(root,"foo","NXentry"),key_error);
    CPPUNIT_ASSERT_THROW(get_child(root,"foo","NXfoo"),key_error);
}

//-----------------------------------------------------------------------------
void get_child_test::test_field_by_name()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject o = get_child(group,"date","");
    CPPUNIT_ASSERT(is_field(o));
    CPPUNIT_ASSERT(get_name(o)=="date");
}

//-----------------------------------------------------------------------------
void get_child_test::test_group_object()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    h5::nxobject object = root;
    CPPUNIT_ASSERT(is_valid(get_child(object,"scan_1","")));
    CPPUNIT_ASSERT(is_valid(get_child(object,"","NXentry")));
    CPPUNIT_ASSERT(is_valid(get_child(object,"scan_1","NXentry")));
    CPPUNIT_ASSERT_THROW(get_child(object,"log",""),key_error);
    CPPUNIT_ASSERT_THROW(get_child(object,"","NXlog"),key_error);
    CPPUNIT_ASSERT_THROW(get_child(object,"log","NXlog"),key_error);
}

//-----------------------------------------------------------------------------
void get_child_test::test_field_object()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject object = field;
    CPPUNIT_ASSERT_THROW(get_child(object,"",""),type_error); 
}

//-----------------------------------------------------------------------------
void get_child_test::test_attribute_object()
{
    std::cerr<<BOOST_CURRENT_FUNCTION<<std::endl;

    h5::nxobject object = attribute;
    CPPUNIT_ASSERT_THROW(get_child(object,"",""),type_error);
   
}

