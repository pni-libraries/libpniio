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
//************************************************************************
//
// Created on: Jul 8, 2014
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//
#pragma once

#include <pni/core/utilities.hpp>
#include "nxattribute.hpp"
#include "../exceptions.hpp"

namespace pni{
namespace io{
namespace nx{
    using pni::io::object_error;

    //!
    //! \brief attribute manager template
    //! 
    //! Provides attribute functionality to other objects that are capable of
    //! holding attributes.
    //! 
    //! \tparam OTYPE parent object type
    template<typename OTYPE>
    class nxattribute_manager
    {
        public:
            typedef nxattribute_manager<OTYPE> manager_type;
            typedef typename OTYPE::attribute_type attribute_type;
            typedef attribute_type value_type;
            typedef pni::core::container_iterator<const manager_type> iterator_type;
            typedef typename OTYPE::imp_type imp_type;
        private:
            imp_type &_imp;
        public:
            nxattribute_manager(imp_type &imp):_imp(imp) {}

            //! 
            //! \brief create an array attribute
            //!
            //! Template method creating a multidimensional attribute of type 
            //! T and shape s. By default an exception will be thrown if an 
            //! attribute of same name already exists. If ov=true an existing 
            //! attribute will be overwritten
            //! 
            //! \throws object_error in case of errors
            //! \param n name of the attribute
            //! \param s shape of the array
            //! \param ov overwrite flag
            //! \return instance of nxattribute
            //!
            template<
                     typename T,
                     typename CTYPE
                    > 
            attribute_type create(const string &n, const CTYPE &s,bool ov=true) const
            {
                attribute_type attr;

                try
                {
                    attr = attribute_type(_imp.template attr<T>(n,s,ov));
                }
                catch(...)
                {
                    throw object_error(EXCEPTION_RECORD,
                            "Cannot create attribute ["+n+"]!");
                }
                return attr;
            }
            
            //-----------------------------------------------------------------
            //!
            //! \brief create scalar attribute
            //!
            //! Template method creating a scalar atribute of type T. By 
            //! default an exception is raised if an attribute of same name 
            //! already exists. If ov=true the existing attribute will be 
            //! overwritten and no exeption will be thrown.
            //!
            //! \throws object_error in case of attribute related errors
            //! \param n name of the attribute
            //! \param ov overwrite flag
            //! \return an instance of nxattribute
            //!
            template<typename T> 
            attribute_type create(const string &n,bool ov=false) const
            {
                return create<T>(n,shape_t{1},ov);
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief open an existing attribute by name
            //! 
            //! Opens an existing attribute of name n and returns an instance 
            //! of nxattribute<> to the callee. An exception will be thrown 
            //! if the attribute does not exist.
            //!
            //! \throws object_error in case of problems
            //! \param n name of the attribute
            //! \return instance of nxattribute
            //!
            attribute_type operator[](const string &n) const
            {
                attribute_type attr;

                try
                {
                    attr = attribute_type(_imp.attr(n));
                }
                catch(...)
                {
                    throw object_error(EXCEPTION_RECORD,
                            "Cannot open attribute ["+n+"]!");
                }
                return attr;
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief open an attribute by index
            //! 
            //! Opens an existing attribute by its index. If the index exceeds 
            //! the total number of attributes attached to this object an 
            //! exception will be thrown.
            //!
            //! \throws object_error in case of errors
            //! \param i index of the attribute
            //! \return instance of nxattribute
            //!
            attribute_type operator[](size_t i) const
            {
                attribute_type attr;

                try
                {
                    attr = attribute_type(_imp.attr(i));
                }
                catch(...)
                {
                    std::stringstream istr;
                    istr<<"Cannot open attribute ["<<i<<"]!";
                    throw object_error(EXCEPTION_RECORD,istr.str());
                }

                return attr;
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief delete an attribute
            //!
            //! Deletes an attribute attached to this object.
            //! \throws object_error in case of errors
            //! \param n name of the attribute
            //!
            void remove(const string &n) const
            {
                try
                {
                    _imp.del_attr(n);
                }
                catch(...)
                {
                    throw object_error(EXCEPTION_RECORD,
                            "Error deleting attribute ["+n+"]!");
                }
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief checks for attribute existance
            //!
            //! Checks whether or not an attribute with a particular name 
            //! exits. If it does true is returned otherwise false.
            //!
            //! \throws object_error in case of errors
            //! \param n name of the attribute
            //! \return true if n exists otherwise false
            //!
            bool exists(const string &n) const
            {
                try
                {
                    return _imp.has_attr(n);
                }
                catch(...)
                {
                    throw object_error(EXCEPTION_RECORD,
                            "Error checking for attribute ["+n+"]!");
                }
                return false;
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief get number of attributes
            //!
            //! Returns the number of attributes attached to this object.
            //! \return number of attributes
            //!
            size_t size() const { return _imp.nattr(); }

            //-----------------------------------------------------------------
            /*! 
            \brief iterator to frist attribute

            Return an iterator to the first attribute attached to the object.
            \return iterator to first attribute
            */
            iterator_type begin() const { return iterator_type(*this); }

            //-----------------------------------------------------------------
            /*! 
            \brief iterator to last attribute

            Return an iterator to the last attribute attached to the object.
            \return iterator to last attribute
            */
            iterator_type end() const
            {
                return iterator_type(*this,size());
            }
    };


//end of namespace
}
}
}
