//
// (c) Copyright 2017 DESY
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
// Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
// Created on: Dec 19, 2017
//

#include "container_test_fixture.hpp"
#include <pni/nexus.hpp>

using namespace pni::io;

ContainerTestFixture::ContainerTestFixture()
{
  file = nexus::create_file("ContainerTest.nxs",hdf5::file::AccessFlags::TRUNCATE);
  root_group = file.root();
  nexus::xml::create_from_file(root_group,"container_test.xml");
}
