/*
 * (c) Copyright 2013 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 * This file is part of libpniio.
 *
 * libpniio is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libpniio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpniio.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 *
 *  Created on: Jul 11, 2013
 *      Author: Eugen Wintersberger
 */

#include <vector>
#include "xml_lowlevel_test.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(xml_lowlevel_test);

//-----------------------------------------------------------------------------
void xml_lowlevel_test::setUp() 
{
    file = h5::nxfile::create_file("xml_lowlevel_test.nx",true);
    root_group = h5::nxgroup(file["/"]);
    int_vec = std::vector<int32>{1,2,3,4,5,6,10};
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::tearDown() 
{ 
    close(group);
    close(root_group);
    close(field);
    file.close();
} 


//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_create_group()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    //test creation with full specification
    root = xml::create_from_string("<group name=\"entry\" type=\"NXentry\"/>");
    child = root.get_child("group");
    group = xml::create_group(root_group,child);
    CPPUNIT_ASSERT(get_name(group) == "entry");
    CPPUNIT_ASSERT(get_class(group) == "NXentry");
    CPPUNIT_ASSERT(is_valid(group));

    //test test creation without NX_class
    root = xml::create_from_string("<group name=\"entry2\"> </group>");
    child = root.get_child("group");
    group = xml::create_group(root_group,child);
    CPPUNIT_ASSERT(get_name(group) == "entry2");
    CPPUNIT_ASSERT(is_valid(group));

    //test exception without name
    root = xml::create_from_string("<group type=\"entry2\"> </group>");
    child = root.get_child("group");
    CPPUNIT_ASSERT_THROW(xml::create_group(root_group,child),
            pni::io::parser_error);

    //test creation of an already existing group
    root = xml::create_from_string("<group name=\"entry2\"> </group>");
    child = root.get_child("group");
    CPPUNIT_ASSERT_THROW(xml::create_group(root_group,child),
            nxgroup_error);

}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_dim2shape_1()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    root = xml::create_from_file("xml_test/dim1.xml");
    child = root.get_child("dimensions");

    auto shape = xml::dim2shape<shape_t>(child);
    CPPUNIT_ASSERT(shape.size()==2);
    CPPUNIT_ASSERT(shape[0] == 55);
    CPPUNIT_ASSERT(shape[1] == 100);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_dim2shape_2()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    root = xml::create_from_file("xml_test/dim2.xml");
    child = root.get_child("dimensions");

    CPPUNIT_ASSERT_THROW(xml::dim2shape<shape_t>(child),shape_mismatch_error);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_dim2shape_3()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    root = xml::create_from_file("xml_test/dim3.xml");
    child = root.get_child("dimensions");

    CPPUNIT_ASSERT_THROW(xml::dim2shape<shape_t>(child),pni::io::parser_error);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_dim2shape_4()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    root = xml::create_from_file("xml_test/dim4.xml");
    child = root.get_child("dimensions");

    CPPUNIT_ASSERT_THROW(xml::dim2shape<shape_t>(child),pni::io::parser_error);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_dim2shape_5()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
    
    root = xml::create_from_file("xml_test/dim5.xml");
    child = root.get_child("dimensions");

    CPPUNIT_ASSERT_THROW(xml::dim2shape<shape_t>(child),pni::io::parser_error);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_createfield_1()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    root = xml::create_from_file("xml_test/field1.xml");
    child = root.get_child("field");

    CPPUNIT_ASSERT_NO_THROW(field = xml::create_field(root_group,child));
    CPPUNIT_ASSERT(is_valid(field));
    CPPUNIT_ASSERT(get_rank(field) == 1);
    CPPUNIT_ASSERT(get_size(field) == 1);
    CPPUNIT_ASSERT(get_type(field) == type_id_t::FLOAT32);
    CPPUNIT_ASSERT(get_unit(field) == "m");
    auto attr = get_attribute(field,"long_name");
    read(attr,buffer);
    CPPUNIT_ASSERT(buffer == "motor along x-axis");
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_createfield_2()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    root = xml::create_from_file("xml_test/field2.xml");
    child = root.get_child("field");

    CPPUNIT_ASSERT_NO_THROW(field = xml::create_field(root_group,child));
    CPPUNIT_ASSERT(is_valid(field));
    CPPUNIT_ASSERT(get_rank(field) == 1);
    CPPUNIT_ASSERT(get_size(field) == 1);
    CPPUNIT_ASSERT(get_type(field) == type_id_t::FLOAT32);

    float32 buffer;
    read(field,buffer);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(buffer,1.,1.e-8);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_createfield_3()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    root = xml::create_from_file("xml_test/field3.xml");
    child = root.get_child("field");

    CPPUNIT_ASSERT_THROW(xml::create_field(root_group,child),pni::io::parser_error);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_createfield_4()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    root = xml::create_from_file("xml_test/field4.xml");
    child = root.get_child("field");

    CPPUNIT_ASSERT_THROW(xml::create_field(root_group,child),pni::io::parser_error);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_createfield_5()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    root = xml::create_from_file("xml_test/field5.xml");
    child = root.get_child("field");

    CPPUNIT_ASSERT_NO_THROW(field = xml::create_field(root_group,child));

    CPPUNIT_ASSERT(is_valid(field));
    CPPUNIT_ASSERT(get_rank(field) == 3);
    CPPUNIT_ASSERT(get_size(field) == 55000);
    CPPUNIT_ASSERT(get_type(field) == type_id_t::FLOAT32);
    auto shape = get_shape<shape_t>(field);
    CPPUNIT_ASSERT(shape[0] == 100);
    CPPUNIT_ASSERT(shape[1] == 55);
    CPPUNIT_ASSERT(shape[2] == 10);
}

//-----------------------------------------------------------------------------
void xml_lowlevel_test::test_create_objects_1()
{
    std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;

    root = xml::create_from_file("xml_test/object_tree1.xml");
    
    CPPUNIT_ASSERT_NO_THROW(xml::create_objects(root_group,root));
    
}