/*

  INScore Project

  Copyright (C) 2009,2010  Grame

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


#ifndef __ISVG__
#define __ISVG__

#include <string>
#include "IGraphicBasedObject.h"
#include "IAnimated.h"

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

class Updater;
class ISVG;
typedef class libmapping::SMARTP<ISVG>	SISVG;
//--------------------------------------------------------------------------
/*!
	\brief a SVG element
*/
class ISVG :  public IGraphicBasedObject, public IAnimated
{
	std::string	fSvg;

	public:		
		static const std::string kSVGType;
		static SISVG create(const std::string& name, IObject * parent)	{ return new ISVG(name, parent); }

		virtual void		print(std::ostream& out) const;
		virtual void		accept (Updater*);		
		const std::string&	getText() const			{ return fSvg; }

	protected:
				 ISVG( const std::string& name, IObject * parent );
		virtual ~ISVG() {}

		/// \brief the \c 'set' message handler
		virtual MsgHandler::msgStatus set (const IMessage* msg);
};

/*! @} */

} // end namespoace

#endif
