//
// Declaration of Nexus object template.
//
// (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
// Created on: Feb 11, 2012
//     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
//

#pragma once

#include <pni/core/arrays.hpp>
#include <pni/core/types.hpp>
#include <pni/core/error.hpp>
#include <pni/core/type_erasures.hpp>

#include "nximp_map.hpp"
#include "nxobject_traits.hpp"
#include "nxobject_type.hpp"
#include "algorithms.hpp"
#include "../exceptions.hpp"


namespace pni{
namespace io{
namespace nx{

    using namespace pni::core;
    //need this here to avoid name collisions with tango headers.
    using pni::core::array;
    using pni::core::string;
    using pni::core::exception;

    //! 
    //! \ingroup nexus_lowlevel
    //! \brief attribute object
    //! 
    //! nxattribute represents an attribute which is attached to one of the 
    //! Nexus objects. Objects of this type can be used to read and write 
    //! attribute data from and to an object.
    //!
    template<nximp_code IMPID> 
    class nxattribute
    {
        public:
            typedef typename nximp_map<IMPID>::attribute_imp   implementation_type;
            typedef typename nximp_map<IMPID>::type_imp type_type;
            //! define the actual type of this object
            typedef nxattribute<IMPID>                attribute_type;
        private:
            //! implementation of the attribute object
            implementation_type _imp;  
           
            //-----------------------------------------------------------------
            //!
            //! \brief write data from array
            //!
            //! Read data from an array type and store it in the attribute.
            //! 
            //! \throws memory_not_allocated_error if the array buffer is not 
            //! allocated
            //! \throws shape_mismatch_error if the shapes of the array and the
            //! attribute do not match
            //! \throws io_error in case of IO errors
            //!
            //! \tparam ATYPE array type
            //! \param a instance of ATYPE
            //!
            template<typename ATYPE> 
            void _write_array(const ATYPE &a) const
            {
                check_allocation_state(a,EXCEPTION_RECORD);
                check_equal_shape(a,*this,EXCEPTION_RECORD);

                _imp.write(pni::core::type_id(a),a.data());
            }
            
            //-----------------------------------------------------------------
            //!
            //! \brief read data to array
            //!
            //! Read attribute data from the file and store it to the array.
            //! \throws memory_not_allocated_error if the arrays buffer is not
            //! allocated
            //! \throws shape_mismatch_error if the shapes of the array and the
            //! attribute do not match
            //! \thorws io_error in case of a general IO error
            //! 
            //! \tparam ATYPE array type
            //! \param a instance of ATYPE
            //!
            template<typename ATYPE> 
            void _read_array(ATYPE &a) const
            {
                check_allocation_state(a,EXCEPTION_RECORD);
                check_equal_shape(a,*this,EXCEPTION_RECORD);


                _imp.read(pni::core::type_id(a),a.data());
            }

        public:
            //==========constructors and destructors============================
            //!
            //! \brief default constructor
            //!
            explicit nxattribute():_imp(){}
            
            //------------------------------------------------------------------
            //!
            //! \brief copy constructor from implementation
            //!
            explicit nxattribute(const implementation_type &i):_imp(i) { }

            //------------------------------------------------------------------
            //!
            //! \brief move constructor from implementation
            //!
            explicit nxattribute(implementation_type &&i):_imp(std::move(i)) 
            { }

            //----------------------------------------------------------------
            //!
            //! \brief conversion constructor
            //!
            //! Constructs an attribute object form an instance of 
            //! object_type. 
            //!
            //! \throws type_error if the object is not an attribute
            //! \param o instance of object_type
            nxattribute(const typename nxobject_trait<IMPID>::object_type &o):
                _imp()
            {
                *this = o;
            }
            
            //===================assignment operators===========================
            //!
            //! \brief conversion assignment
            //!
            //! This assignment operator converts an object_type to an
            //! attribute.
            //! \throws type_error if the object is not an attribute
            //! \param o instance of object_type
            //! \return reference to attribute
            //!
            attribute_type &operator=(const typename
                    nxobject_trait<IMPID>::object_type &o)
            {
                *this = as_attribute(o);
                return *this;
            }

            //====================IO methods====================================
            //! 
            //! \brief write data from an mdarray instance
            //!
            //! Write data from an mdarray instance to the attribute object
            //!
            //! \throws memory_not_allocated_error if array buffer is not 
            //! allocated
            //! \throws shape_mismatch_errror if array and attribute shape 
            //! do not match
            //! \throws io_error in case of IO errors
            //! \throws invalid_object_error if the object is not valid
            //! \throws type_error if the datatype stored in the array is not 
            //! supported
            //! \throws object_error in case of any other error
            //!
            //! \tparam STORAGE storage type of the mdarray
            //! \tparam IMAP index map type of the mdarray
            //! \tparam IPA inplace arithmetic type of the mdarray
            //! \param o instance of DArray from which to write data
            //!
            template<
                     typename STORAGE,
                     typename IMAP,
                     typename IPA
                    >
            void write(const mdarray<STORAGE,IMAP,IPA> &o) const
            {
                _write_array(o);
            }

            //----------------------------------------------------------------
            //!
            //! \brief write data from a plain C pointer
            //!
            //! This method is for interoperability with old C-code. 
            //! It writes data referenced by a plain pointer. It is assumed 
            //! that the user has allocated enough memory.
            //! 
            //! \throws io_error in case of a general IO error
            //! \throws type_error if data type not supported by the library
            //! \throws invalid_object_error if object not valid
            //! \throws object_error in case of any other error
            //! 
            //! \tparam T data type of the pointer
            //! \param data pointer to the data
            //! 
            template<typename T>
            void write(const T *data) const
            {
                static_assert(!std::is_pointer<T>::value,"no const pointer");
                _imp.write(type_id_map<T>::type_id,data);
            }

            //-----------------------------------------------------------------
            //! 
            //! \brief write a single scalar value
            //!
            //! Write a single scalar value. This throws an exception if the 
            //! field is not scalar (size=1).
            //!
            //! \throws shape_mismatch_error if field is not scalar
            //! \throws io_error in case of a general IO error
            //! \throws invalid_object_error in case of an invalid field object
            //! \throws type_error in case the datatype is not supported
            //! \throws object_error in case of any other error
            //! 
            //! \tparam T data type of the scalar to write
            //! \param value reference to the value to write
            //!
            template<typename T > 
            void write(const T &value) const
            {
                static_assert(!std::is_pointer<T>::value,"no const pointer");

                if(size()!=1)
                    throw shape_mismatch_error(EXCEPTION_RECORD,
                            "Field is not scalar!");

                _imp.write(type_id_map<T>::type_id,&value);
            }


            //------------------------------------------------------------------
            //! 
            //! \brief write a C-string
            //!
            //! This is a special implementation of write for classical 
            //! C-strings.
            //! 
            //! \throws shape_mismatch_error if field is not scalar
            //! \throws invalid_object_error if field is not valid
            //! \throws io_error in case of a general IO error
            //! \throws object_error in case of any other error
            //!
            //! \param value pointer to a C-string
            //!
            void write(const char *value) const
            {
                string s(value);
                write(s);
            }

            //-----------------------------------------------------------------
            //!
            //! \brief write array type erasure data
            //!
            //! \throws memory_not_allocated_error if array not allocated
            //! \throws type_error if data type not supported
            //! \throws shape_mismatch_error if array shape does not match
            //! \throws invalid_object_error if field is not valid
            //! \throws io_error in case of a general IO error
            //! \throws object_error in case of any other error
            //!
            //! \param a instance of array
            //!
            void write(const array &a) const
            {
                _imp.write(a.type_id(),a.data());
            }

            //-----------------------------------------------------------------
            //!
            //! \brief read data to an array
            //!
            //! Read data to an DArray instance.
            //!
            //! \throws memory_not_allocated_error if array buffer is not 
            //! allocated
            //! \throws shape_mismatch_error if array and attribute shape do 
            //! not match
            //! \throws invalid_object_error if field is not valid
            //! \throws type_error if data type is not supported
            //! \throws io_error in case of a general IO error
            //! \throws object_error in case of any other error
            //! 
            //! \tparam STORAGE storage type for mdarray
            //! \tparam IMAP index map type for mdarray
            //! \tparam IPA in-place arithmetics type for mdarray
            //!
            //! \param o instance of DArray
            //!
            template<
                     typename STORAGE,
                     typename IMAP,
                     typename IPA
                    > 
            void read(mdarray<STORAGE,IMAP,IPA> &o) const
            {
                _read_array(o);
            }
            
            //-----------------------------------------------------------------
            //!
            //! \brief read data to array
            //!
            //! \throws memory_not_allocated_error if array not allocated
            //! \throws type_error if data type is not supported
            //! \throws shape_mismatch_error if array shape does not match
            //! \throws invalid_object_error if field is not valid
            //! \throws io_error in case of a general IO error
            //! \throws object_error in case of any other error
            //!
            //! \param a instance of array 
            //!
            void read(array &a) const
            {
                check_allocation_state(a,EXCEPTION_RECORD);
                check_equal_shape(a,*this,EXCEPTION_RECORD);
                _imp.read(a.type_id(),a.data());
            }

            //-----------------------------------------------------------------
            //!
            //! \brief read a single scalar value
            //!
            //! Read a single scalar value.
            //!
            //! \throws shape_mismatch_error if the attribute is not scalar
            //! \throws invalid_object_error if the attribute is not valid
            //! \throws io_error in case of a general IO error
            //! \throws type_error if data type is not supported
            //! \throws object_error in case of any other error
            //!
            //! \tparam T type to read to
            //! \param value reference to an instance of T
            //!
            template<typename T> 
            void read(T &value) const
            {
                if(size()!=1)
                    throw shape_mismatch_error(EXCEPTION_RECORD,
                            "Try to read a scalar from an array field!");

                _imp.read(type_id_map<T>::type_id,&value);
            }

            //---------------------------------------------------------------
            //!
            //! \brief read data to pointer
            //!
            //! Read data to a memory location referenced by the pointer value.
            //! This method is for compatability with old C-code.
            //! It is assumed that enough memory is allocated.
            //!
            //! \throws type_error if T is not supported
            //! \throws invalid_object_error if attribute is not valid
            //! \throws io_error in case of a general IO error
            //! \throws object_error in case of any other error
            //! 
            //! \tparam T data type to read to
            //! \param value pointer to memory
            //!
            template<typename T>
            void read(T *value) const
            {
                _imp.read(type_id_map<T>::type_id,value);
            }


            //============simple maintenance methods========================
            //!
            //! \brief obtain attribute shape
            //!
            //! \throws invalid_object_error if attribute not valid
            //! \throws object_error in case of any other error
            //! 
            //! \tparam CTYPE user requested container type
            //! \return instance of CTYPE with shape information
            //!
            template<typename CTYPE> 
            CTYPE shape() const
            {
                return type_type::template from_index_vector<CTYPE>(_imp.shape());
            }

            //--------------------------------------------------------------
            //!
            //! \brief return attribute rank
            //!
            //! This method returns the rank of the attribute - the number of 
            //! dimensions. 
            //! 
            //! \throws invalid_object_error if the attribute is not valid
            //! \throws object_error in case of any other error
            //! \return number of dimensions
            //!
            size_t rank() const { return this->_imp.rank(); }

            //--------------------------------------------------------------
            //!
            //! \brief obtain attribute size
            //! 
            //! Return the number of elements stored in the attribute.
            //! 
            //! \throws invalid_object_error if the attribute is not valid
            //! \throws object_error in case of any other error
            //! 
            //! \return number of elements
            //!
            size_t size() const { return _imp.size(); }

            //--------------------------------------------------------------
            //! 
            //! \brief obtain type id
            //!
            //! Returns the type ID of the data stored in the attribute.
            //!
            //! \throws invalid_object_error if attribute is not valid
            //! \throws object_error in case of any other error
            //!
            //! \return type id of the elements stored in the field
            //!
            type_id_t type_id() const { return _imp.type_id(); }

            //--------------------------------------------------------------
            //! 
            //! \brief close attribute
            //!
            void close() { _imp.close(); }

            //---------------------------------------------------------------
            //!
            //! \brief check validity of the attribute
            //! 
            //! \throws object_error if the validity check fails
            //! \return true if object is valid, false otherwise
            //!
            bool is_valid() const { return _imp.is_valid(); } 

            //---------------------------------------------------------------
            //! 
            //! \brief get attribute name
            //!
            //! Obtain the name of the attribute. With this name it can be
            //! identified at a particular object.
            //!
            //! \throws invalid_object_error if object is not valid
            //! \throws io_error in case of an IO error
            //! \throws object_error in case of any other error
            //!
            //! \return string with the attributes name
            //!
            string name() const { return _imp.name(); }

            //-----------------------------------------------------------------
            //!
            //! \brief return parent object
            //! 
            //! This method returns the parent object of a 
            //! \throws invalid_object_error if attribute is not valid
            //! \throws type_error if parent type is unkown
            //! \throws object_error in case of any other error
            //!
            //! \return parent object
            //!
            typename nxobject_trait<IMPID>::object_type parent() const
            {
                typedef typename nxobject_trait<IMPID>::group_type group_type;
                typedef typename nxobject_trait<IMPID>::field_type field_type;
                typedef typename nximp_map<IMPID>::group_imp group_imp_type;
                typedef typename nximp_map<IMPID>::field_imp field_imp_type;

                typename nximp_map<IMPID>::object_imp p = _imp.parent();

                if(p.nxobject_type() == nxobject_type::NXFIELD)
                    return field_type(field_imp_type(std::move(p)));
                else if(p.nxobject_type() == nxobject_type::NXGROUP)
                    return group_type(group_imp_type(std::move(p)));
                else
                    throw type_error(EXCEPTION_RECORD,
                             "Cannot convert parent type");

            }

    };

//end of namespace
}
}
}
