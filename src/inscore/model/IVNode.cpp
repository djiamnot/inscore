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

#include "IVNode.h"
#include "Updater.h"

using namespace std;

namespace inscore
{

//--------------------------------------------------------------------------
// IVNode implementation
//--------------------------------------------------------------------------
IVNode::IVNode(const std::string& name, IObject * parent) : IObject(name, parent)
{
	fTypeString = "vnode";
	fMsgHandlerMap[kset_SetMethod]		= (void*)0;	// reject the set message
	fMsgHandlerMap[klock_GetSetMethod]	= (void*)0;		// reject the lock message (can't be deleted anyway)

	fGetMsgHandlerMap[klock_GetSetMethod] = (void*)0;	// reject the lock message
	fAutoMap = false;
	localMapModified (false);
	newData (false);
}

}
