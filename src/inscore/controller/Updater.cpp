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

#include <iostream>

#include "Updater.h"
#include "IObject.h"
#include "ISync.h"
#include "IGraphicSignal.h"
#include "ISignalNode.h"


using namespace std;

namespace inscore
{

//--------------------------------------------------------------------------
void Updater::update (IObject* object)
{
	if (needupdate (object)) {
		object->accept(this);
    }
	if (object->getState() & IObject::kSubModified) {
        ViewUpdater * vu = dynamic_cast<ViewUpdater*>(this);
        IObject::subnodes elements = vu ? object->invertedSort() : object->sort();
		IObject::subnodes::const_iterator i = elements.begin();
		while (i != elements.end()) {
			update (*i);
			i++;
		}
	}
}

//--------------------------------------------------------------------------
//void ViewUpdater::update (IObject* object)
//{
//	cout << "ViewUpdater::update " << object->name() << " " << needupdate (object) << endl;
//	Updater::update(object);
//}

//--------------------------------------------------------------------------
bool ViewUpdater::needupdate (IObject* o)
{
	if (o->getDeleted()) return false;
	int state = o->getState();
	return (state & (IObject::kModified + IObject::kNewObject + IObject::kMasterModified));
}

//--------------------------------------------------------------------------
bool LocalMapUpdater::needupdate (IObject* o)
{
	if (o->getDeleted()) return false;
	bool need = ( o->fAutoMap && o->durationModified()) || o->localMapModified() || o->newData();
	return need;
}

//--------------------------------------------------------------------------
bool SlaveMapUpdater::needupdate (IObject* o)
{
	if (o->getDeleted() || !o->mapable()) return false;
	int state = o->getState();
	bool need = o->localMapModified() || o->dateModified() || (state & (IObject::kModified + IObject::kNewObject + IObject::kMasterModified));
	//if (need) cout << "SlaveMapUpdater::needupdate: " << o->name() << endl;
	return need;
}

//--------------------------------------------------------------------------
// modification state propagation
//--------------------------------------------------------------------------
void SigModified::updateTo (IGraphicSignal* gs)
{
	if (gs->getSignal()->getState()) {
		gs->setModified();
	}
}

void SigModified::updateTo (ISignalNode* signode)
{
	if (signode->getState()) {
		vector<SISignalConnection>& cnx = signode->getConnections();		// propagate signal modification to connected objects
		for (size_t i=0; i<cnx.size(); i++) {
			cnx[i]->getObject()->setModified();
		}
	}
}

} // end namespoace
