/*
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
 *
 * This file is part of libpninx.
 *
 * libpninx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * libpninx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libpninx.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************
 *
 * Declaration of an HDF5 attribute object.
 *
 * Created on: Jan 10, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */


#ifndef __H5ATTRIBUTE_HPP__
#define __H5ATTRIBUTE_HPP__

#include<sstream>

#include <pni/utils/Types.hpp>
#include <pni/utils/Array.hpp>
#include <pni/utils/DBuffer.hpp>
#include <pni/utils/SBuffer.hpp>
#include <pni/utils/RBuffer.hpp>

using namespace pni::utils;

#include "H5NamedObject.hpp"
#include "H5Dataspace.hpp"
#include "H5Datatype.hpp"
#include "H5Exceptions.hpp"
#include "H5DatatypeFactory.hpp"

namespace pni{
namespace nx{
namespace h5{
    /*! 
    \ingroup nxh5_classes
    \brief HDF5 attribute object

    An HDF5 attribute behaves basically like a dataset with the 
    exception that (for obvious reasons) no other attributes 
    can be attached to it. In addition attributes cannot use 
    compression nor can they be used along with a chunked layout.
    Attribute objects are derived from H5NamedObject. However, 
    the semantics of the name(), base(), and path() methods is 
    slightly different. name() returns the name of the attribute
    while base() and path() return emtpy strings as an attribute
    */ 
    class H5Attribute:public H5NamedObject
    {
        private:
            H5Dataspace _dspace; //!< dataspace of the attribute
            H5Datatype _dtype;   //!< data type of the attribute

            //-----------------------------------------------------------------
            /*! 
            \brief set dataspace and datatype

            This ethod uses the HDF5 ID of an attribute object
            to obtain the dataspace and the datatype. 
            Thus it sets the internal state of the class.
            */
            void __set_space_type();

        public:
            //================constructors and destructors=============
            //! default constructor
            explicit H5Attribute();
            //! copy constructor
            H5Attribute(const H5Attribute &o);
            //! move constructor
            H5Attribute(H5Attribute &&o);
            //! construct from HDF5 type ID

            //! This constructor takes the ID to an attribute object
            //! and initializes the class. The ID can be obtained 
            //! with HDF5s C interface.
            //! \param id HDF5 id of the attribute object.
            explicit H5Attribute(const hid_t &id);
            
            //! destructor
            ~H5Attribute();

            //=============assignment operators========================
            //! copy assignment operator
            H5Attribute &operator=(const H5Attribute &a);
            //! move assignment operator
            H5Attribute &operator=(H5Attribute &&o);


            //===================reading and writting data=====================
            /*
            \brief pointer write

            Private method writing data from memory addressed by 
            ptr to the attribute.
            \throws H5AttributeError in case of IO errors
            \param ptr pointer to memory
            */
            template<typename T> void write(const T *ptr) const
            {
                H5Datatype mem_type = H5DatatypeFactory::create_type<T>();
                herr_t err = H5Awrite(id(),mem_type.id(),(void *)ptr);
                if(err<0)
                    throw H5AttributeError(EXCEPTION_RECORD, 
                        "Error writing attribute ["+this->name()+"]!");
            }

            //-----------------------------------------------------------------
            void write(const String &s) const;
           
            /*! 
            \brief pointer read

            Private method reading data form the attribute to a 
            memory region addressed by ptr. An exception is 
            thrown if an error occurs
            \throws H5AttributeError in case of IO errors
            \param ptr pointer to memory
            */
            template<typename T> void read(T *ptr) const
            {
                H5Datatype mem_type = H5DatatypeFactory::create_type<T>();
                herr_t err = H5Aread(id(),mem_type.id(),(void *)ptr);
                if(err < 0)
                    throw H5AttributeError(EXCEPTION_RECORD,
                            "Error reading attribute ["+this->name()+"]!");
            }

            //-----------------------------------------------------------------
            /*! 
            \brief read to string

            Reads a string value from an attribute.
            \throws ShapeMissmatchError if attribute is not scalar
            \throws H5AttributeError in case of general IO errors
            \param s string variable to which data will be written
            */
            void read(String &s) const;
           
            //=================attribute inquery methods=======================
            /*! 
            \brief obtain attribute shape

            Returns the shape of the attribute.
            \return shape object
            */
            template<typename CTYPE> CTYPE shape() const
            {
                return this->_dspace.template shape<CTYPE>();
            }

            //-----------------------------------------------------------------
            /*! 
            \brief return type_id

            Return the ID of the datatype used for the attribute.
            \return type ID
            */
            TypeID type_id() const;

            //-----------------------------------------------------------------
            /*! 
            \brief close attribute

            This method closes the attribute object.
            */
            virtual void close();

            //-----------------------------------------------------------------
            /*! 
            \brief return base name

            Returns an empty string since an attribute has no base name.
            \return empty string
            */
            virtual String base() const;

            //-----------------------------------------------------------------
            /*! 
            \brief return path

            Returns an empty string as an attribute has no path.
            \return empty string
            */
            virtual String path() const; 

            //-----------------------------------------------------------------
            /*! 
            \brief get name

            Return the name of the attribute as string.
            \return attribute name
            */
            virtual String name() const;


    };


//end of namespace
}
}
}


#endif

