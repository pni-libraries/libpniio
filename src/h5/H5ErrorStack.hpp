/*
 * Declaration of Nexus specific exceptions
 *
 * (c) Copyright 2011 DESY, Eugen Wintersberger <eugen.wintersberger@desy.de>
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
 * H5ErrorStack.hpp
 *
 *  Created on: Jul 28, 2011
 *      Author: eugen
 */

#pragma once

extern "C"{
#include <hdf5.h>
}

#include <iostream>
#include <vector>

#include <pni/core/types.hpp>
#include <pni/core/exceptions.hpp>

namespace pni {
namespace io {
namespace nx {
namespace h5 {

//Walker functins to read over error messages
extern "C" herr_t _error_walker(unsigned n,const H5E_error2_t *eptr,void *client_data);

using namespace pni::core;

    /*! 
    \ingroup nxh5_error_classes
    \brief HDF5 error record

    This class describes an HDF5 error record. Such a record consists basically
    of a class id (which is HDF5 for all HDF5 library functions), a description,
    a major and a minor message, and the name of the function where the error
    occurred. Along with this the name of the HDF5 source file where the
    function is defined is stored.
    */
    class H5Error
    {
        private:
            hid_t _class_id;       //!< error class
            string _class_name;    //!< name of the error class
            hid_t _major_number;   //!< major number of the error
            hid_t _minor_number;   //!< minor number of the error
            //! name of the source file where the error occurred
            string _file_name;    
            //! name of the HDF5 function where the error occurred
            string _func_name;     
            string _description;   //!< optional description of the error
            string _minor_message; //!< minor message of the error
            string _major_message; //!< major message of the error
        public:
            //-----------------------------------------------------------------
            //! default constructor
            H5Error();

            //-----------------------------------------------------------------
            //! copy constructor
            H5Error(const H5Error &e);

            //-----------------------------------------------------------------
            //! default destructor
            virtual ~H5Error();

            //-----------------------------------------------------------------
            //! assignment operator
            H5Error &operator=(const H5Error &e);
           
            //-----------------------------------------------------------------
            /*! 
            \brief get class ID
            
            Get the error class ID
            \return error class ID
            */
            hid_t class_id() const;
       
            //-----------------------------------------------------------------
            /*! 
            \brief set class ID
            
            Sets the class id from an object.  If internal buffer allocation
            fails an exception will be thrown.
            \throws MemoryAllocationError if buffer allocation fails
            \param id object ID
            */
            void class_id(hid_t id);
           
            //-----------------------------------------------------------------
            //! get major number
            hid_t major_number() const;

            //-----------------------------------------------------------------
            //! set major number
            void major_number(hid_t v);

            //-----------------------------------------------------------------
            //! get minor number
            hid_t minor_number() const;

            //-----------------------------------------------------------------
            //! set minor number
            void minor_number(hid_t v);

            //-----------------------------------------------------------------
            //! get file name
            string file_name() const;

            //-----------------------------------------------------------------
            //! get file name
            void file_name(const string &n);

            //-----------------------------------------------------------------
            //! get function name
            string func_name() const;

            //-----------------------------------------------------------------
            //! set function name
            void func_name(const string &n);

            //-----------------------------------------------------------------
            //! get description
            string description() const;

            //-----------------------------------------------------------------
            //! set description
            void description(const string &n);
           
            //-----------------------------------------------------------------
            //! get major message
            string major_message() const;

            //-----------------------------------------------------------------
            //! get minor message
            string minor_message() const;
           
            //-----------------------------------------------------------------
            //! ouput stream operator
            friend std::ostream &operator<<(std::ostream &o,const H5Error &e);
    };

    /*! 
    \ingroup nxh5_error_classes
    \brief HDF5 error stack

    Class HDF5ErrorStack provides a simple object oriented interface to the HDF5
    error stack.
    */
    class H5ErrorStack 
    {
        private:
            //! id of the error stack
            hid_t _stack_id;      
            //! vector with error records on this stack
            std::vector<H5Error> _errors; 

        public:
            //-----------------------------------------------------------------
            //! default constructor
            H5ErrorStack();

            //-----------------------------------------------------------------
            //! copy constructor
            H5ErrorStack(const H5ErrorStack &s);

            //-----------------------------------------------------------------
            //! default destructor
            virtual ~H5ErrorStack();

            //-----------------------------------------------------------------
            //! copy constructor
            H5ErrorStack &operator=(const H5ErrorStack &s);

            //-----------------------------------------------------------------
            /*! 
            \brief get number of errors

            Returns the total number of HDF5 errors in the stack.
            \return number of errors
            */
            ssize_t number_of_errors() const{ return _errors.size(); }

            //-----------------------------------------------------------------
            /*! 
            \brief fill stack

            Reads error messages from the current HDF5 error stack and add them
            to the stack.	
            */
            void fill();

            //-----------------------------------------------------------------
            /*! 
            \brief append error

            Appends a single error to the stack.
            \param e HDF5 error record
            */
            void append(const H5Error &e);

            //-----------------------------------------------------------------
            //! ostream operator
            friend std::ostream &operator<<(std::ostream &o,const H5ErrorStack &s);

            //-----------------------------------------------------------------
            /*! 
            \brief walker function

            This function is used to walk through the HDF5 error stack. 
            \param n undocumented
            \param eptr pointer to error stack
            \param client_data data passed by the client to the walker code
            \return HDF5 error code
            */
            friend herr_t _error_walker(unsigned n,const H5E_error2_t *eptr,void *client_data);
    };


//end of namespace
}
}
}
}
