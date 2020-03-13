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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#include "HTMLUpdater.h"
#include "IObject.h"
#include "VObjectView.h"
#include "ISync.h"

#include <iostream>
using namespace std;

namespace inscore
{

//--------------------------------------------------------------------------
void HTMLUpdater::update (IObject* object)
{
	if (needupdate (object)) {
		VObjectView* view = object->getView();
		if (view) {
//cerr << "HTMLUpdater::update " << object->name() << " state: " << object->getState() << endl;
			view->updateView(object);
		}
	}
	if (object->getState() & IObject::kSubModified) {
		for (auto elt: object->elements())
			update (elt);
	}
}

//--------------------------------------------------------------------------
bool HTMLViewUpdater::needupdate (IObject* o)
{
	int state = o->getState();
	return (state & (IObject::kModified + IObject::kNewObject + IObject::kMasterModified));
}

//--------------------------------------------------------------------------
bool HTMLLocalMapUpdater::needupdate (IObject* o)
{
	return ( o->fAutoMap && o->durationModified()) || o->localMapModified() || o->newData();
}

//--------------------------------------------------------------------------
bool HTMLSlaveMapUpdater::needupdate (IObject* o)
{
	return true;

//	return o->localMapModified() || o->dateModified(); // || master->getMaster()->localMapModified() || master->modified();

//	int state = o->getState();
//	return (state & (IObject::kModified + IObject::kNewObject + IObject::kMasterModified));
}

} // end namespoace
