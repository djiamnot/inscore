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

#ifndef __I1DSensor__
#define __I1DSensor__


#include "ISensor.h"


namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

//------------------------------------------------------------------------
class I1DSensor : public ISensor
{
	public:
		virtual void	cleanup ();
	
	protected:
		float	fDefaultValue;
	
				 I1DSensor(const std::string& name, IObject * parent);
		virtual ~I1DSensor();

		/// \brief called by the time task, intended to read the sensor data
		virtual void	readData ();
		virtual float	read () = 0;
		virtual float	sigvalue (float value) const = 0;

		virtual	bool activate(bool val);
		virtual void setHandlers ();
};

/*! @} */

} // end namespoace

#endif
