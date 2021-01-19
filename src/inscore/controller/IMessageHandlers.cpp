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

#include "IMessageHandlers.h"
#include "IObject.h"

namespace inscore
{

MsgHandler::msgStatus TSetMethodMsgHandler<Master, void (Master::*)(float)>::operator ()(const IMessage* msg)
{ 
	if ( msg->size() != 1 ) return kBadParameters;
	float val;
	if ( !msg->cast_param(0, val) ) return kBadParameters;

	(fObject->*fMethod)( val );
	fSlave->modify();
	return kProcessed;
}


} // end namespoace
