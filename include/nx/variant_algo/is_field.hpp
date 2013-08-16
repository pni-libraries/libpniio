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
 * Created on: Jun 28, 2013
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */
#pragma once

#include "../nxvariant_traits.hpp"
#include "nxvariant_algo_helper.hpp"

namespace pni{
namespace io{
namespace nx{

    /*!
    \ingroup variant_code
    \brief check field visitor

    This visitor checks a variant type if the object stored is a field. 
    In this case true is returned.
    
    \tparam VTYPE variant type
    */
    template<typename VTYPE> 
    class is_field_visitor : public boost::static_visitor<bool>
    {
        public:
            //! result type (bool)
            typedef bool result_type;   
            //! Nexus group type
            typedef typename nxvariant_group_type<VTYPE>::type group_type;
            //! Nexus field type
            typedef typename nxvariant_field_type<VTYPE>::type field_type;
            //! Nexus attribute type
            typedef typename nxvariant_attribute_type<VTYPE>::type attribute_type;
           
            //-----------------------------------------------------------------
            /*!
            \brief process group instances

            \param g reference to group instance
            \return false
            */
            result_type operator()(const group_type &g) const
            {
                return false;
            }

            //-----------------------------------------------------------------
            /*!
            \brief process field instances

            \param f reference to field instance
            \return true
            */
            result_type operator()(const field_type &f) const
            {
                return true;
            }

            //-----------------------------------------------------------------
            /*!
            \brief process attribute instances

            \param a reference to attribute instance
            \return false
            */
            result_type operator()(const attribute_type &a) const
            {
                return false;
            }
    };

    /*!
    \ingroup variant_code
    \brief check if field

    Wrapper function around the is_field_visitor. The function returns true if
    the object stored in the variant type is a field type.
    \tparam VTYPE Nexus object variant type
    \param o instance of VTYPE
    \return true if the object is a field
    */
    template<typename VTYPE> 
    typename is_field_visitor<VTYPE>::result_type is_field(const VTYPE &o)
    {
        return boost::apply_visitor(is_field_visitor<VTYPE>(),o);
    }

//end of namespace
}
}
}
