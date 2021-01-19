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

#include "IMusicXMLFile.h"
#include "IScene.h"
#include "Updater.h"
#include "VObjectView.h"
#include "XMLImporter.h"

using namespace std;

namespace inscore
{
//--------------------------------------------------------------------------
const string IMusicXMLFile::kMusicXMLFileType("musicxmlf");

//--------------------------------------------------------------------------
IMusicXMLFile::IMusicXMLFile( const std::string& name, IObject * parent )
	: IMusicXMLCode (name, parent), TFile (parent->getScene())
{ 
	fTypeString = kMusicXMLFileType;
	fGetMsgHandlerMap[""] = TGetParamMsgHandler<string>::create(getFile());
}

//--------------------------------------------------------------------------
void IMusicXMLFile::accept (Updater* u)
{
	u->updateTo (SIMusicXMLFile(this));
}

//--------------------------------------------------------------------------
MsgHandler::msgStatus IMusicXMLFile::set (const IMessage* msg )
{ 
	MsgHandler::msgStatus status = IObject::set(msg);
	if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) return status; 

#ifndef EMCC
	if (!XMLImporter::musicxmlSupported()) {
		ITLErr << "MusicXML import is not available" << ITLEndl;
		return MsgHandler::kCreateFailure;
	}
#endif

	status = TFile::set( msg );
	if (status & MsgHandler::kProcessed) {
#ifndef EMCC
		if(!hasData())
        {
            if (!read(fXML))
                status = MsgHandler::kCreateFailure;
            else
            {
                string converted;
                if ( convert ( fXML, converted) ) {
                    setGMN( converted );
                    status = MsgHandler::kProcessed;
                    newData(true);
                }
                else {
                    ITLErr << "Cannot convert MusicXML file" << ITLEndl;
                    status = MsgHandler::kBadParameters;
                }
            }
		}
#else
		setPending();
		newData(true);
#endif
	}
	return status;
}

//--------------------------------------------------------------------------
void IMusicXMLFile::updateUrl()
{
	fXML = data();
	string converted;
	if ( convert ( fXML, converted) ) {
		setGMN( converted );
		newData(true);
	}
	else
		ITLErr << "updating : Cannot convert MusicXML file" << ITLEndl;

	this->getView()->updateLocalMapping(this);
}

}
