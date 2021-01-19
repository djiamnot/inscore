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

#ifndef __ICompass__
#define __ICompass__

#include "I1DSensor.h"
#include "IQSensor.h"

class QCompass;

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

class ICompass;
typedef class libmapping::SMARTP<ICompass>	SICompass;
//------------------------------------------------------------------------
class ICompass : public IQSensor<QCompass, I1DSensor>
{
	public:
		static const std::string kCompassType;
		static SICompass create(const std::string& name, IObject * parent)	{ return new ICompass(name, parent); }
	
	protected:
				 ICompass(const std::string& name, IObject * parent);
		virtual ~ICompass();

		/// \brief called by the time task, intended to read the sensor data
		virtual float read ();
		virtual float sigvalue (float value) const		{ return value; }
};

/*! @} */

} // end namespoace

#endif
