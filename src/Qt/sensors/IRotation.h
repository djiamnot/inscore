/*

  INScore Project

  Copyright (C) 2016  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 11 cours de Verdun Gensoul, 69002 Lyon - France
  research@grame.fr

*/

#ifndef __IRotation__
#define __IRotation__

#include "I3DSensor.h"
#include "IQSensor.h"

class QRotationSensor;
class QRotationReading;

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

class IRotation;
typedef class libmapping::SMARTP<IRotation>	SIRotation;
//------------------------------------------------------------------------
class IRotation : public IQSensor<QRotationSensor, I3DSensor>
{
	bool		fHasZ;				// a flag to indicate whether the sensor has z rotation
	float		fCX, fCY, fCZ;		// calibration angles
	bool		fSetFromEuler;

	void		setFromEuler (float x, float y, float z);

	public:
		static const std::string kRotationType;
		static SIRotation create(const std::string& name, IObject * parent)	{ return new IRotation(name, parent); }
	
	protected:
				 IRotation(const std::string& name, IObject * parent);
		virtual ~IRotation();

		/// \brief called by the time task, intended to read the sensor data
		virtual bool read (float& x, float& y, float& z);
		virtual void readData ();
		virtual float sigvalue (float value) const { return value; }

		virtual bool hasZ () const;
		virtual void setHandlers ();
};

/*! @} */

} // end namespoace

#endif
