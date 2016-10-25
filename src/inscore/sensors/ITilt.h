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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __ITilt__
#define __ITilt__

#include "I3DSensor.h"
#include "IQSensor.h"

class QTiltSensor;

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

class ITilt;
typedef class libmapping::SMARTP<ITilt>	SITilt;
//------------------------------------------------------------------------
class ITilt : public IQSensor<QTiltSensor, I3DSensor>
{
	public:
		static const std::string kTiltType;
		static SITilt create(const std::string& name, IObject * parent)	{ return new ITilt(name, parent); }
	
	protected:
				 ITilt(const std::string& name, IObject * parent);
		virtual ~ITilt();

		virtual void calibrate ();
		/// \brief called by the time task, intended to read the sensor data
		virtual bool read (float& x, float& y, float& z);
		/// \brief sets the message handlers.
		virtual void setHandlers ();
};

/*! @} */

} // end namespoace

#endif
