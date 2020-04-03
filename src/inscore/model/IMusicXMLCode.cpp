/*

  INScore Project

  Copyright (C) 2014  Grame

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

#include "IMusicXMLCode.h"
#include "Updater.h"
#include "IMessage.h"
#include "IMessageHandlers.h"
#include "XMLImporter.h"

using namespace std;
using namespace libmapping;

namespace inscore
{

//--------------------------------------------------------------------------
const string IMusicXMLCode::kMusicXMLCodeType("musicxml");

//--------------------------------------------------------------------------
IMusicXMLCode::IMusicXMLCode( const std::string& name, IObject * parent ) : IGuidoCode (name, parent)
{
	fTypeString = kMusicXMLCodeType;
}

//--------------------------------------------------------------------------
void IMusicXMLCode::accept (Updater* u)
{
	u->updateTo (SIMusicXMLCode(this));
}

//--------------------------------------------------------------------------
bool IMusicXMLCode::convert (const string& xml, string& converted) const
{
    std::stringstream sstr;
	if ( XMLImporter::musicxmlString2Guido ( xml.c_str(), true, sstr) ) {
		converted = sstr.str();
		return true;
	}
	return false;
}

// message handling
//--------------------------------------------------------------------------
MsgHandler::msgStatus IMusicXMLCode::set ( const IMessage* msg )
{
	MsgHandler::msgStatus status = IObject::set(msg);
	if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) return status;

#ifndef EMCC
	if (!XMLImporter::musicxmlSupported()) {
		ITLErr << "MusicXML import is not available" << ITLEndl;
		return MsgHandler::kCreateFailure;
	}
	string t;
	if ((msg->size() == 2) && msg->param(1, t)) {
		if ( t != getMusicXML() ) {
			string converted;
			if ( convert ( t, converted) ) {		// first convert musicxml to guido
				setGMN( converted );
				status = MsgHandler::kProcessed;
				newData(true);
			}
			else {
				ITLErr << "Cannot convert MusicXML code" << ITLEndl;
				status = MsgHandler::kBadParameters;
			}
		}
		else status = MsgHandler::kProcessedNoChange;

	}
	else status = MsgHandler::kBadParameters;
#else
	string t;
	if ((msg->size() == 2) && msg->param(1, t))	{
		fXML = t;
		status = MsgHandler::kProcessed;
		newData(true);
	}
	else status = MsgHandler::kBadParameters;

#endif
	return status;
}

}
