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

#include "IMessageStack.h"
#include "IMessage.h"

namespace inscore
{

//--------------------------------------------------------------------------
IMessageStack::IMessageStack() : fReceivedCount(0)	{ fPendingMsgs.resize(10000); }
IMessageStack::~IMessageStack()	
{ 
	flush(); 
}
	
//--------------------------------------------------------------------------
int IMessageStack::size() 
{ 
	return fPendingMsgs.available();
} 

void IMessageStack::push(SIMessage* msg)
{ 
	fPendingMsgs.write (msg);
} 

//--------------------------------------------------------------------------
SIMessage* IMessageStack::pop()
{ 
	return fPendingMsgs.read();
}

//--------------------------------------------------------------------------
void IMessageStack::flush()
{
	SIMessage* msg = pop();
	while (msg) {
		delete msg;
		msg = pop();
	}
}

} // end namespace
