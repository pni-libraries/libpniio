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
//  Created on: Dec 2, 2014
//      Author: Eugen Wintersberger
//
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testing XML dimensions tag 

#include <boost/test/unit_test.hpp>
#include <pni/io/nx/xml/dimensions.hpp>
#include <pni/core/types.hpp>
#include <pni/core/error.hpp>
#include <pni/io/exceptions.hpp>
#include <boost/current_function.hpp>

using namespace pni::io::nx;
using namespace pni::core;

struct read_fixture
{
    xml::node root;
    xml::node child;

    read_fixture(const string &fname):
        root(xml::create_from_file(fname)),
        child(root.get_child("dimensions"))
    {}
};

BOOST_AUTO_TEST_SUITE(read_test)


    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_read_1)
    {
        read_fixture f("dim1.xml");
        
        auto shape = xml::dimensions::object_from_xml<shape_t>(f.child);
        BOOST_CHECK_EQUAL(shape.size(),2);
        BOOST_CHECK_EQUAL(shape.front(),55);
        BOOST_CHECK_EQUAL(shape.back(),100);
    }

    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_read_2)
    {
        read_fixture f("dim2.xml");

        BOOST_CHECK_THROW(xml::dimensions::object_from_xml<shape_t>(f.child),
                          shape_mismatch_error);
    }

    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_read_3)
    {
        read_fixture f("dim3.xml");

        BOOST_CHECK_THROW(xml::dimensions::object_from_xml<shape_t>(f.child),
                          key_error);
    }

    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_read_4)
    {
        read_fixture f("dim4.xml");

        BOOST_CHECK_THROW(xml::dimensions::object_from_xml<shape_t>(f.child),
                          key_error);
    }

    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_read_5)
    {
        read_fixture f("dim5.xml");

        BOOST_CHECK_THROW(xml::dimensions::object_from_xml<shape_t>(f.child),
                          key_error);
    }

    //-------------------------------------------------------------------------
    BOOST_AUTO_TEST_CASE(test_read_6)
    {
        read_fixture f("dim6.xml");
        auto s = xml::dimensions::object_from_xml<shape_t>(f.child);
        BOOST_CHECK_EQUAL(s.size(),0);
    }

BOOST_AUTO_TEST_SUITE_END()