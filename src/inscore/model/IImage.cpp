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

#include "IImage.h"
#include "IMessage.h"
#include "Updater.h"
#include "VObjectView.h"

using namespace std;

namespace inscore
{
//--------------------------------------------------------------------------
const string IImage::kImageType("img");

//--------------------------------------------------------------------------
IImage::IImage( const std::string& name, IObject * parent ) 
	: IGraphicBasedObject (name, parent), TFile (parent->getScene())
{	
	fTypeString = kImageType;
	fGetMsgHandlerMap[""] = TGetParamMsgHandler<string>::create(getFile());
	setPending();
}

//--------------------------------------------------------------------------
void IImage::accept (Updater* u)
{
	u->updateTo (SIImage(this));
}

//--------------------------------------------------------------------------
MsgHandler::msgStatus IImage::set (const IMessage* msg )
{ 
	MsgHandler::msgStatus status = IObject::set(msg);
	if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) return status; 

	status = TFile::set( msg );
	if (status & MsgHandler::kProcessed)
		newData(true);
	setCalled();
	getView()->initView (this);
	return status;
}

//--------------------------------------------------------------------------
void IImage::updateUrl()
{
	this->getView()->setImage((const unsigned char*)data(), dataSize());
	this->getView()->updateLocalMapping(this);
}

}
