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
#pragma once
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <pni/core/types.hpp>
#include <pni/io/nx/nx.hpp>

using namespace pni::core;
using namespace pni::io::nx;

//!
//! \ingroup algorithm_test
//! \brief test validity function
//! 
//! Test the is_valid function template. There are two implementations of this 
//! template. The first one works on the following types
//!
//! \li nxfile
//! \li nxgroup
//! \li nxfield
//! \li nxattribute
//! 
//! and one using a visitor class to obtain the same information for objects 
//! stored in an nxobject template. 
//! 
class is_valid_test : public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(is_valid_test);
        CPPUNIT_TEST(test_nxobject_field);
        CPPUNIT_TEST(test_nxobject_group);
        CPPUNIT_TEST(test_nxobject_attribute);
        CPPUNIT_TEST(test_file);
        CPPUNIT_TEST(test_field);
        CPPUNIT_TEST(test_attribute);
        CPPUNIT_TEST(test_group);
        CPPUNIT_TEST_SUITE_END();

        h5::nxfile file;
        h5::nxfield field;
        h5::nxgroup group,root;
        h5::nxattribute attribute;

        typedef nxobject_trait<nximp_code::HDF5>::object_type object_type;
        
    public:
        void setUp();
        void tearDown();

        void test_field();
        void test_file();
        void test_attribute();
        void test_group();
        
        void test_nxobject_field();
        void test_nxobject_group();
        void test_nxobject_attribute();

};

