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

#include "IGuidoFile.h"
#include "IScene.h"
#include "Updater.h"
#include "VObjectView.h"

using namespace std;

namespace inscore
{
//--------------------------------------------------------------------------
const string IGuidoFile::kGuidoFileType("gmnf");

//--------------------------------------------------------------------------
IGuidoFile::IGuidoFile( const std::string& name, IObject * parent )
	: IGuidoCode (name, parent), TFile (parent->getScene())
{ 
	fTypeString = kGuidoFileType;
	fGetMsgHandlerMap[""] = TGetParamMsgHandler<string>::create(getFile());
}

//--------------------------------------------------------------------------
void IGuidoFile::print (ostream& out) const
{
	IGuidoCode::print(out);
	TFile::print(out);
}

//--------------------------------------------------------------------------
void IGuidoFile::accept (Updater* u)
{
	u->updateTo (SIGuidoFile(this));
}

//--------------------------------------------------------------------------
MsgHandler::msgStatus IGuidoFile::set (const IMessage* msg )
{ 
	MsgHandler::msgStatus status = IObject::set(msg);
	if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) return status; 

	newData (true);
	
	status = TFile::set( msg );
	if (status & MsgHandler::kProcessed) {
#ifndef EMCC
		if(!hasData())
        {
            if (!read(fGMN))
                status = MsgHandler::kCreateFailure;
            else newData(true);
        }
#else
		setPending();
		newData(true);
#endif
	}
	return status;
}


//--------------------------------------------------------------------------
void IGuidoFile::updateUrl()
{
    if (dataSize())	fGMN = data();
    this->getView()->updateLocalMapping(this);
}

}
