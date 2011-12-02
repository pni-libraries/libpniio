/*
 * Declaration of the NXfield template
 *
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
 * Declaration of the NXfield template
 *
 * Created on: Jul 3, 2011
 *     Author: Eugen Wintersberger <eugen.wintersberger@desy.de>
 */

#ifndef NXFIELD_HPP_
#define NXFIELD_HPP_

#include <pni/utils/PNITypes.hpp>
#include <pni/utils/ScalarObject.hpp>
#include <pni/utils/Scalar.hpp>
#include <pni/utils/ArrayShape.hpp>
#include <pni/utils/ArrayObject.hpp>
#include <pni/utils/Array.hpp>
#include <pni/utils/NumericObject.hpp>

#include "NXObject.hpp"
#include "NXExceptions.hpp"
#include "NXSelection.hpp"

using namespace pni::utils;

namespace pni{
namespace nx{

//need to do some trick here to get the stream operators overloaded
template<typename Imp> class NXField;
template<typename Imp> NXField<Imp> &operator<<(NXField<Imp> &o,const ScalarObject &s);
template<typename Imp> NXField<Imp> &operator<<(NXField<Imp> &o,const ArrayObject &a);
template<typename Imp> NXField<Imp> &operator<<(NXField<Imp> &o,const String &s);
template<typename Imp> NXField<Imp> &operator>>(NXField<Imp> &o,ScalarObject &s);
template<typename Imp> NXField<Imp> &operator>>(NXField<Imp> &o,ArrayObject &s);
template<typename Imp> NXField<Imp> &operator>>(NXField<Imp> &o,String &s);

//! Nexus datafield

//! NXField is the basic data holding object in a Nexus file. You cannot create
//! an instance of this object directly rather you have to use one of the
//! factory methods provided by NXGroup.
//! NXField behaves like a container for data object which for the time being
//! can be either strings, Scalars, or Array objects.
template<typename Imp>
class NXField:public NXObject<Imp> {
private:
	UInt64 _read_stream_pos;
public:
	typedef NXSelection<typename NXImpMap<Imp::IMPCODE>::SelectionImplementation > Selection;
	typedef boost::shared_ptr<NXField<Imp> > sptr;
	//! default constructor
	NXField();
	//! copy constructor
	NXField(const NXField<Imp> &);
	//! move constructor
	NXField(NXField<Imp> &&o);
	NXField(Imp &&o):NXObject<Imp>(std::move(o)){}
	virtual ~NXField();

	//! copy assignment
	NXField<Imp> &operator=(const NXField<Imp> &);

	//! move assignment
	NXField<Imp> &operator=(NXField<Imp> &&o);



	//! total field shape

	//! Return the total shape of the field. The rank if this
	//! shape is the rank of the object stored +1. The method returns
	//! a constant reference to the fields' shape. So you cannot alter
	//! this object.
	//! \return ArrayShape& reference to the shape object
	virtual const ArrayShape &getShape() const{
		return this->getImplementation().getShape();
	}

	//! shape of a single element

	//! Return the shape of a single element stored in the container.
	virtual const ArrayShape &getElementShape() const {
		return this->getImplementation().getElementShape();
	}

	//! get the type ID

	//! Return the ID of the data type stored in the field.
	//! \return data type ID
	virtual PNITypeID getTypeID() const {
		return this->getImplementation().getTypeID();
	}

	//! get the field name

	//! Return the name of the field.
	virtual String getName() const {
		return this->getImplementation().getName();
	}

	//! append an array object to the field

	//! Append data from an ArrayObject to the field. There are several
	//! conditions that must be satisfied in order to
	//! .) the array must be allocated
	//! .) the shape of the array must match the element shape of the field
	//! .) the data type must be convertible to that of the field
	//! .) the array object and the field must have the same physical units
	//! If one of this conditions is not satisfied an exception will
	//! be thrown.
	//! The data will be attached to the end of the field.
	//! \param a array object from which data will be appended
	void append(const ArrayObject &a);
	//! append a scalar object ot the field

	//! In order for this operation to succeed the following conditions
	//! must be satisfied
	//! .) the scalar object and the field must have the same physical units
	//! .) the data type of the scalar object and that of the field must be convertible
	//! If one of this conditions is not satisfied an exception will be thrown.
	void append(const ScalarObject &s);
	//! append a string obect to the field

	//! The onyl condition for this operation to succeed is that the
	//! field is of string type.
	void append(const String &s);

	//! set an array object
	void set(const UInt64 &i,const ArrayObject &s);
	//! set a scalar object
	void set(const UInt64 &i,const ScalarObject &s);
	//! set a string object
	void set(const UInt64 &i,const String &s);

	//! get an array object

	//!
	//!
	void get(const UInt64 &i,ArrayObject &a);
	//! read the entire data into a single array

	//! Reads a single scalar from the field
	void get(ArrayObject &a);
	//! get a scalar object
	void get(const UInt64 &i,ScalarObject &s);
	//! get a single scalar
	void get(ScalarObject &s);
	//alternative with move assignment
	//ScalarObject &&get();
	//! get a string object
	void get(const UInt64 &i,String &s);


	//! close the field
	void close();

	//! reset read stream position

	//! Reset the stream position for reading. This can be used
	//! to re-read data using stream IO.
	void resetReadStreamPosition() {
		_read_stream_pos = 0;
	}

	friend NXField<Imp> &operator<< <> (NXField<Imp> &o,const ScalarObject &s);
	friend NXField<Imp> &operator<< <> (NXField<Imp> &o,const ArrayObject &a);
	friend NXField<Imp> &operator<< <> (NXField<Imp> &o,const String &s);

	friend NXField<Imp> &operator>> <> (NXField<Imp> &o,ScalarObject &s);
	friend NXField<Imp> &operator>> <> (NXField<Imp> &o,ArrayObject &s);
	friend NXField<Imp> &operator>> <> (NXField<Imp> &o,String &s);

};

//--------------constructors and destructors------------------------------------

template<typename Imp> NXField<Imp>::NXField():NXObject<Imp>() {
	_read_stream_pos = 0;
}

template<typename Imp> NXField<Imp>::NXField(const NXField<Imp> &o)
		:NXObject<Imp>(o){
	EXCEPTION_SETUP("template<typename Imp> NXField<Imp>::NXField(const NXField<Imp> &o)");
	try{
		//this->setImplementation(o.getImplementation());
		_read_stream_pos = 0;
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Error in copy constructor for NXField!");
		EXCEPTION_THROW();
	}
}

template<typename Imp> NXField<Imp>::NXField(NXField<Imp> &&o){
	EXCEPTION_SETUP("template<typename Imp> NXField<Imp>::NXField(NXField<Imp> &&o)");
	try{
		//express the move constructor in terms of move assignment
		 *this = std::move(o);
		_read_stream_pos = 0;
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Error in move constructor for NXField!");
		EXCEPTION_THROW();
	}
}

template<typename Imp> NXField<Imp>::~NXField() {
	// TODO Auto-generated destructor stub
	close();
	_read_stream_pos = 0;
}

//------------------------------operators---------------------------------------
template<typename Imp> NXField<Imp> &NXField<Imp>::operator=(const NXField<Imp> &o){
	EXCEPTION_SETUP("template<typename Imp> NXField<Imp> &NXField<Imp>::operator=(const NXField<Imp> &o)");

	if( this != &o){
		try{
			(NXObject<Imp> &)(*this) = (NXObject<Imp> &)o;
			//this->setImplementation(o.getImplementation());
			_read_stream_pos = 0;
		}catch(...){
			EXCEPTION_INIT(NXFieldError,"Error in copy assignment of NXField!");
			EXCEPTION_THROW();
		}
	}
	return *this;
}

template<typename Imp> NXField<Imp> &NXField<Imp>::operator=(NXField<Imp> &&o){
	EXCEPTION_SETUP("template<typename Imp> NXField<Imp> &NXField<Imp>::operator=(NXField<Imp> &&o)");

	if(this != &o){
		try{
			//move assignment of the base class
			(NXObject<Imp> &)(*this) = std::move((NXObject<Imp> &)o);
			//this->setImplementation(std::move(o.getImplementation()));
			_read_stream_pos = 0;
		}catch(...){
			EXCEPTION_INIT(NXFieldError,"Error in move assignment of NXField!");
			EXCEPTION_THROW();
		}
	}

	return *this;
}

//--------------------------write operations------------------------------------
template<typename Imp> void NXField<Imp>::append(const ArrayObject &a){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::append(const ArrayObject &a)");

	if(!a.isAllocated()){
		EXCEPTION_INIT(NXFieldError,"Array not allocated - cannot append!");
		EXCEPTION_THROW();
	}

	if(a.getShape() != getElementShape()){
		EXCEPTION_INIT(ShapeMissmatchError,"Element shape and array shape do not match!");
		EXCEPTION_THROW();
	}

	//check if the unit fits
	String unit = getAttribute("units");
	if(a.getUnit() != unit){
		EXCEPTION_INIT(NXFieldError,"Units do not match!");
		EXCEPTION_THROW();
	}

	try{
		this->getImplementation().append(a);
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Cannot append data to field ["+getName()+"]!");
		EXCEPTION_THROW();
	}
}

template<typename Imp> void NXField<Imp>::append(const ScalarObject &s){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::append(const ScalarObject &s)");

	if(!this->getImplementation().isScalar()){
		EXCEPTION_INIT(NXFieldError,"Data field is not for scalar data!");
		EXCEPTION_THROW();
	}

	//check units
	String unit = getAttribute("units");
	if(a.getUnit() != unit){
		EXCEPTION_INIT(NXFieldError,"Units do not match!");
		EXCEPTION_THROW();
	}

	try{
		this->getImplementation().append(s);
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Cannot append data to field ["+getName()+"]!");
		EXCEPTION_THROW();
	}
}

//------------------------------------------------------------------------------
template<typename Imp> void NXField<Imp>::append(const String &s){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::append(const String &s)");
	if(!this->getImplementation().isString()){
		EXCEPTION_INIT(NXFieldError,"Data field is not a string field!");
		EXCEPTION_THROW();
	}

	try{
		this->getImplementation().append(s);
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Cannot append string data to field ["+getName()+"]!");
		EXCEPTION_THROW();
	}
}

//------------------------------------------------------------------------------
template<typename Imp> void NXField<Imp>::set(const UInt64 &i,const ArrayObject &a){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::insert(const UInt64 &i,const ArrayObject &a)");

	//check if the field is of appropriate type
	if(!this->getImplementation().isArray()){
		EXCEPTION_INIT(NXFieldError,"Datafield is not an Array field!");
		EXCEPTION_THROW();
	}

	//check if the array is allocated
	if(!a.isAllocated()){
		EXCEPTION_INIT(NXFieldError,"Array not allocated - cannot write data!");
		EXCEPTION_THROW();
	}

	//check the shape
	if(a.getShape() != this->getElementShape()){
		EXCEPTION_INIT(ShapeMissmatchError,"Array shape does not match element shape!");
		EXCEPTION_THROW();
	}

	//check the unit
	String unit = this->getAttribute("units");
	if(a.getUnit() != unit){
		EXCEPTION_INIT(NXFieldError,"Units do not match!");
		EXCEPTION_THROW();
	}

	//here comes the real writing operation

}

//------------------------------------------------------------------------------
template<typename Imp> void NXField<Imp>::set(const UInt64 &i,const ScalarObject &a){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::insert(const UInt64 &i,const ScalarObject &a)");

	if(!this->getImplementation().isScalar()){
		EXCEPTION_INIT(NXFieldError,"Field is not a scalar field!");
		EXCEPTION_THROW();
	}

	//check unit
	String unit = this->getAttribute("units");
	if(a.getUnit()!=unit){
		EXCEPTION_INIT(NXFieldError,"Units do not match!");
		EXCEPTION_THROW();
	}

	//here comes the real writing operation

}

template<typename Imp> void NXField<Imp>::set(const UInt64 &i,const String &s){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::insert(const UInt64 &i,const String &s)");

}

template<typename Imp> void NXField<Imp>::get(const UInt64 &i,ArrayObject &a){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::get(const UInt64 &i,ArrayObject &a)");

	if(!this->getImplementation().isArray()){
		EXCEPTION_INIT(NXFieldError,"Data field is not an array!");
		EXCEPTION_THROW();
	}

	try{
		this->getImplementation().get(i,a);
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Cannot read array data from field ["+getName()+"]!");
		EXCEPTION_THROW();
	}
}

template<typename Imp> void NXField<Imp>::get(const UInt64 &i,ScalarObject &a){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::get(const UInt64 &i,ScalarObject &a)");
	if(!this->getImplementation().isScalar()){
		EXCEPTION_INIT(NXFieldError,"Data field is not scalar!");
		EXCEPTION_THROW();
	}

	try{
		this->getImplementation().get(i,a);
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Cannot read scalar data from field ["+getName()+"]!");
		EXCEPTION_THROW();
	}
}

template<typename Imp> void NXField<Imp>::get(const UInt64 &i,String &s){
	EXCEPTION_SETUP("template<typename Imp> void NXField<Imp>::get(const UInt64 &i,String &s)");

	if(!this->getImplementation().isString()){
		EXCEPTION_INIT(NXFieldError,"Data field is not a string array!");
		EXCEPTION_THROW();
	}

	try{
		this->getImplementation().get(i,s);
	}catch(...){
		EXCEPTION_INIT(NXFieldError,"Cannot fetch string data from field ["+getName()+"]!");
		EXCEPTION_THROW();
	}

}


//----------------------stream operators---------------------------------------
template<typename Imp>
NXField<Imp> &operator<<(NXField<Imp> &o,const ScalarObject &s){
	o.append(s);
	return o;
}

template<typename Imp>
NXField<Imp> &operator<<(NXField<Imp> &o,const ArrayObject &a){
	o.append(a);
	return o;
}

template<typename Imp>
NXField<Imp> &operator<<(NXField<Imp> &o,const String &s){
	o.append(s);
	return o;
}

template<typename Imp>
NXField<Imp> &operator>>(NXField<Imp> &o,ScalarObject &s){
	o.get(o._read_stream_pos,s);
	o._read_stream_pos++;
	return o;
}

template<typename Imp>
NXField<Imp> &operator>>(NXField<Imp> &o,ArrayObject &a){
	o.get(o._read_stream_pos,a);
	o._read_stream_pos++;
	return o;
}

template<typename Imp>
NXField<Imp> &operator>>(NXField<Imp> &o,String &s){
	o.get(o._read_stream_pos,s);
	o._read_stream_pos++;
	return o;
}




//------------------convienance methods-----------------------------------------
template<typename Imp> void NXField<Imp>::close(){
	this->getImplementation().close();
}



//end of namespace
}
}

#endif /* NXFIELD_HPP_ */
