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
 * Declaration of a general HDF5 Dataspace object.
 *
 * Created on: Jan 10, 2012
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#ifndef __H5DATASPACE_HPP__
#define __H5DATASPACE_HPP__

#include <sstream>
#include <algorithm>
#include <pni/utils/DBuffer.hpp>
#include <pni/utils/Array.hpp>

using namespace pni::utils;

#include "H5Object.hpp"

namespace pni{
namespace nx {
namespace h5 {

    /*!
    \ingroup nxh5_classes
    \brief dataspace object

    This class encapsulates an HDF5 dataspace. A dataspace describes
    the organization of dataelements within a dataset in the file
    or of data stored in memory. Thus it basically describes a 
    multidimensional array with a particular rank (number of 
    dimensions) and a particular number of elements along each 
    dimension. For each dimension there exists an actual number of 
    elements which is the number of elements now allocated and a 
    maximum number of elements. The later one gives the maximum 
    number of elements along a dimension to which a dataset (not a 
    dataspace) can grow. This number can be a finite one or 
    can be set to H5Dataspace::UNLIMITED which allows the 
    dataset to grow infinitely along this dimension. 
 
    At any time during the lifetime of a H5Dataspace object the
    dataspace it describes can be resized to any required 
    shape (see the resize() and grow() methods). 
    This makes it easy to adopt the object to your needs.
    There are many possibilities how to construct a dataspace. 
    See the users manual for details.
    */
    class H5Dataspace:public H5Object{
        private:

            DBuffer<hsize_t> _maxdims; //!< maximum number of elements dimensions
            DBuffer<hsize_t> _dims;    //!< number of elements 

            /*!
            create a dataspace from buffers

            This method will create dataspace according to the 
            values stored in the buffer objects. The resulting 
            dataspace will always be simple
            */
            void __create_dataspace();

            //! set buffers from object

            //! Use this method if a dataspace is created from an 
            //! HDF5 id. The method will fill the buffers according
            //! to the dataspace described by the ID. 
            void __set_buffers();
        public:
            //! static data member describing an unlimited dimension. 
            static const hsize_t UNLIMITED = H5S_UNLIMITED;

            //! default constructor

            //! By default a scalar dataspace is constructed. This 
            //! constructor can always be used in order to store a
            //! scalar value.
            explicit H5Dataspace();
            //! copy constructor
            H5Dataspace(const H5Dataspace &o);
            //! copy conversion constructor
            H5Dataspace(const H5Object &o);
            //! move constructor
            H5Dataspace(H5Dataspace &&o);
            //! move conversion constructor
            H5Dataspace(H5Object &&o);

            /*! 
            \brief construct form shape

            Constructor takes a container which holds the shape of the dataspace
            to create. The actual as well as the maximum number of dimensions
            will be set to these values.
            \tparam CTYPE container type for the shape
            \param s instance of CTYPE with the shape
            */
            template<typename CTYPE> explicit H5Dataspace(const CTYPE &s):
                H5Object(H5Screate(H5S_SCALAR)),
                _maxdims(s.size()),
                _dims(s.size())
            {
                std::copy(s.begin(),s.end(),_maxdims.begin());
                std::copy(s.begin(),s.end(),_dims.end());

                __create_dataspace();
            }

            /*!
            \brief construction with an initializer list

            Use an initializer list to construct a dataspace. The 
            dataspace created by this constructor is always a simple 
            constructor and has constant size. 
            \param list initializer list
            \sa H5Dataspace(const Shape &s)
            */
            H5Dataspace(const std::initializer_list<hsize_t> &list);
          
            /*! 
            \brief constructor with actual and maximum shape

            Constructor takes two containers of arbitrary type which hold the
            actual and the maximum shape of the dataspace.
           
            \tparam CTYPE1 container type for the actual shape
            \tparam CTYPE2 container type for the maximum shape
            \param s initial shape
            \param ms maximum shape
            */
            template<typename CTYPE1,typename CTYPE2>
            explicit H5Dataspace(const CTYPE1 &s,const CTYPE2 &ms):
                H5Object(H5Screate(H5S_SCALAR)),
                _maxdims(ms.size()),
                _dims(s.size())

            {
                //check if the ranks of the shapes is equal
                if(s.size() != ms.size())
                {
                    std::stringstream ss;
                    ss<<"Rank of actual shape ("<<s.size()<<") and of ";
                    ss<<"maximum shape ("<<ms.size()<<") do not match!";
                    throw ShapeMissmatchError(EXCEPTION_RECORD,ss.str());
                }

                std::copy(ms.begin(),ms.end(),_maxdims.begin());
                std::copy(s.begin(),s.end(),_dims.begin());

                //resize the dataspace to a simple one
                __create_dataspace();
            }

            /*! 
            \brief fixed size dataspace

            Create a fixed size dataspace from two initializer 
            lists. The created dataspace is a simple dataspace in 
            HDF5 terminilogy.
            */
            H5Dataspace(const std::initializer_list<hsize_t> &dlist,
                        const std::initializer_list<hsize_t> &mlist);

            /*! 
            \brief create object from HDF5 id

            Constructor creating an object directly from its HDF5
            ID. k
            */
            explicit H5Dataspace(const hid_t &id);

            /*! 
            \brief destructor

            As the destructor is not virtual this class must not 
            be used as a base class for other classes.
            */
            ~H5Dataspace();

            //================Assignment operators=====================
            //! copy assignment operator
            H5Dataspace &operator=(const H5Dataspace &o);
            //! copy conversion operator
            H5Dataspace &operator=(const H5Object &o);
            //! move assignment operator
            H5Dataspace &operator=(H5Dataspace &&o);
            //! move conversion assignment operator
            H5Dataspace &operator=(H5Object &&o);

            //===============convenience  methods======================
            /*! \brief obtain current shape
            
            This method returns the actual shape (number of elements
            along each dimension) of the dataspace.
            If the dataspace is scalar an empty shape is returned.
            \return shape object */
            
            const DBuffer<hsize_t> &shape() const { return _dims; }

            template<typename CTYPE> CTYPE shape() const
            {
                CTYPE c(_dims.size());
                std::copy(_dims.begin(),_dims.end(),c.begin());
                return c;
            }

            /*! \brief obtain the maximum shape

            Returns the maximum shape of the dataspace. 
            \return shape object */
            const DBuffer<hsize_t> &maxshape() const
            {
                return _maxdims;
            }

            template<typename CTYPE> CTYPE maxshape() const
            {
                CTYPE c(_maxdims.size());
                std::copy(_maxdims.begin(),_maxdims.end(),c.begin());
                return c;
            }

            /*! \brief rank of dataset

            Returns the rank (the number of dimensions) of the dataset. 
            For a scalar dataspace this method returns 0.
            \return number of dimension */
            size_t rank() const;

            /*! \brief number of elements

            Returns the number of elements along dimension i. If the 
            dataspace is scalar 0 is returned independent of the value 
            of i.
            \throws IndexError if i exceeds the dataspace rank
            \param i dimension index
            \return number of elements along i */
            size_t dim(size_t i) const;

            /*! \brief max.elements along dimension

            Returns the maximum number of dimension along dimension i. 
            If the dataspace is scalar 0 is returned independent of 
            the value of i.
            \throws IndexError if i exceeds dataspace rank
            \param i index of dimension
            \return maximum number of elements along dimension i */
            size_t max_dim(size_t i) const;

            /*! \brief total number of elements

            Returns the total number of elemenets that can be stored 
            in the dataspace.  For a scalar dataspace this method 
            returns 1.
            \return total number of elements */
            size_t size() const;

            /*! \brief number of elements
            
            Returns the number of elements along dimension i.
            \throws IndexError if i exceeds dataspace rank
            \param i dimension index
            \return number of elements along i */
            size_t operator[](size_t i) const;

            /*! \brief true if scalar dataspace

            Returns true if the dataspace is scalar. 
            \return true if dataspace is scalar */
            bool is_scalar() const;

            /*! \brief resize dataspace

            Resizes the dataspace to a new shape determined by s. If 
            the dataspace was originally scalar the new dataspace 
            becomes simple. In this case the maximum number of 
            elements along each dimension will be determined by s too. 
            Using this datspace for the construction of a dataset
            will lead to an dataset of constant size.
            \param s new dataspace shape */
            template<typename CTYPE> void resize(const CTYPE &s)
            {
                _dims.allocate(s.size());
                _maxdims.allocate(s.size());
                
                std::copy(s.begin(),s.end(),_dims.begin());
                std::copy(s.begin(),s.end(),_maxdims.begin());

                __create_dataspace();
            }

            /*! \brief resize dataspace

            This method does basically what resize(const Shape &s) is
            doing. However, an initializer list is used instead 
            of a shape object. This method exists for convenience 
            as it makes typing code easier.
            \param list initializer list */
            void resize(const std::initializer_list<hsize_t> &list);

            /*! \brief resize dataspace

            With this version of the resize method the actual and 
            maximum number of elements along each dimension can be 
            configured individually. Using such a dataspace for dataset
            construction will lead to an resizeable dataset.
            \throws ShapeMissmatchError if the ranks of the two shapes do not match
            \param s initial shape of the dataspace
            \param ms maximum shape of the dataspace */
            template<typename CTYPE1,typename CTYPE2>
            void resize(const CTYPE1 &s,const CTYPE2 &ms)
            {
                if(s.size() != ms.size())
                {
                    std::stringstream ss;
                    ss<<"Rank of actual shape ("<<s.size()<<") and of ";
                    ss<<"maximum shape ("<<ms.size()<<") do not match!";
                    throw ShapeMissmatchError(EXCEPTION_RECORD,ss.str());
                }

                _dims.allocate(s.rank());
                _maxdims.allocate(ms.rank());

                std::copy(s.begin(),s.end(),_dims.begin());
                std::copy(ms.begin(),ms.end(),_maxdims.begin());

                __create_dataspace();
            }

            /*! \brief resize dataspace

            Like resize(const Shape&, const Shape &) but using 
            initializer lists instead of shape objects. This is 
            again a convenience method to reduce typing work.
            The rank of the resized dataspace is determined by the 
            size of the tow lists. Both lists must be of equal size.
            \throws ShapeMissmatchError if list sizes are not equal
            \param dlist list with actual number of elements
            \param mlist list with maximum number of elements */
            void resize(const std::initializer_list<hsize_t> &dlist,
                        const std::initializer_list<hsize_t> &mlist);

            /*! \brief grow dataspace

            Grow the dataspace along dimension dim by ext elements.
            This is a very simple method to enlarge a dataspace along
            a particular dimension. If dim is larger than the rank 
            of the dataspace an exception will be thrown.
            \throws IndexError if dim exteeds rank of dataspace
            \param dim dimension along which to grow
            \param ext extend by which to grow */
            void grow(size_t dim=0,size_t ext=1) ;

            //! close the dataspace
            virtual void close();

            //! output operator
            friend std::ostream &operator<<(std::ostream &o,
                    const H5Dataspace &s);

        };

//end of namespace
}
}
}



#endif

