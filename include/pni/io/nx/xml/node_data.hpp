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
// Created on: Jul 15, 2013
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#pragma once

#include <pni/core/types.hpp>
#include "../../exceptions.hpp"
#include "node.hpp"
#include "default.hpp"
#include "object_data.hpp"

namespace pni{
namespace io{
namespace nx{
namespace xml{

    //!
    //! \ingroup xml_classes
    //! \brief node IO 
    //! 
    //! This child class of object_data manages data IO for XML nodes.
    //! 
    class node_data : public object_data
    {
        public:

            //!
            //! \brief read data from node
            //! 
            //! Eventually reads data from a node and returns it encoded 
            //! in a string. If no data is stored in the tag an empty 
            //! string will be returned.
            //! 
            //! \param n node from which to read
            //! \return data as string
            //! 
            virtual string read(const node &n) const;
   
            //----------------------------------------------------------------
            //!
            //! \brief write data to node 
            //! 
            //! Data passed by the user will be written unaltered to to the 
            //! node. Already existing data will be replaced.
            //! 
            //! \param data the data which to write to the node
            //! \param n the node to which to write
            //! 
            virtual void write(const string &data,node &n) const; 

    };


//end of namespace
}
}
}
}
