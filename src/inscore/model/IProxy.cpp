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

#include "Modules.h"

#include "IAppl.h"
#include "IScene.h"
#include "IMessage.h"
#include "IObjectFactory.h"
#include "IProxy.h"
#include "ISignalNode.h"
#include "ISignal.h"
#include "Tools.h"
#include "IUrlIntermediateObject.h"
#include "IMusicXMLFile.h"
#include "IGuidoFile.h"
#include "IHtmlFile.h"
#include "ISVGFile.h"
#include "IImage.h"
#include "ITextFile.h"

#if INCLUDEFaust
#include "IFaustProcessor.h"
#include "IFaustDSP.h"
#include "IFaustDSPFile.h"
#endif

#if INCLUDESensors
#include "SensorsModel.h"
#endif

using namespace std;

namespace inscore
{

//--------------------------------------------------------------------------
static string name2type (const std::string& name)
{
#if INCLUDESensors
	if  ((name == IAccelerometer::kAccelerometerType)	||
		(name == IGyroscope::kGyroscopeType)			||
		(name == IRotation::kRotationType)				||
		(name == IAmbientLight::kAmbientLightType)		||
		(name == ILight::kLightType)					||
		(name == ICompass::kCompassType)				||
		(name == IProximity::kProximityType)			||
		(name == IOrientation::kOrientationType)		||
		(name == ITilt::kTiltType)						||
		(name == IMagnetometer::kMagnetometerType))
		return name;
#endif
	return ISignal::kSignalType;
}

//--------------------------------------------------------------------------
int IProxy::signal (const IMessage* msg, const std::string& objName, SIObject parent)
{
	if (msg->message() == kget_SetMethod) return MsgHandler::kBadAddress;
	if (!msg->size())			return MsgHandler::kBadParameters;

	string objType = msg->param(0)->value<string>("");
	SIObject obj;
#if INCLUDEFaust
	if (objType == IFaustProcessor::kFaustProcessorType || objType == IFaustDSP::kFaustDSPType || objType == IFaustDSPFile::kFaustDSPFileType)
		obj = IObjectFactory::create(objName, objType, parent);
	else {
		string sigtype = name2type (objName);
        obj = IObjectFactory::create(objName, sigtype, parent);
	}
#else
	obj = IObjectFactory::create(objName, name2type (objName), parent);
#endif
    if (obj) {
		int status = obj->execute(msg);
		if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) parent->add(obj);
		return MsgHandler::kProcessed;
	}
	return MsgHandler::kCreateFailure;
}

//--------------------------------------------------------------------------
int IProxy::execute (const IMessage* msg, const std::string& objName, SIObject parent, IObject** newobj, IObject* previousObj)
{
	if (parent && (parent->name() == ISignalNode::kName))
		return signal (msg, objName, parent);

	if (Tools::regexp(objName))
        return MsgHandler::kBadAddress;

	string objType;
	if (parent && (parent->getTypeString() == IAppl::kApplType)) {
		if (msg->message() != knew_SetMethod)
            return MsgHandler::kBadAddress;
		if (msg->size())
            return MsgHandler::kBadParameters;
		objType = IScene::kSceneType;
	}
	else {
		string tmp = msg->message();
		if (msg->message() != kset_SetMethod)
            return MsgHandler::kBadAddress;
		if (msg->size() == 0)
            return MsgHandler::kBadParameters;
		if (parent->getTypeString() == "vnode")
            return MsgHandler::kCreateFailure;		// creating objects in vnodes in not allowed
		objType = msg->param(0)->value<string>("");
	}


    SIMessage newmsg;
    if(objType == ITextFile::kTextFileType || objType == IImage::kImageType || objType == IGuidoFile::kGuidoFileType || objType == IHtmlFile::kHtmlFileType || objType == IMusicXMLFile::kMusicXMLFileType || objType == ISVGFile::kSVGFileType)
    {
#ifndef EMCC
        std::string path;
        if (!msg->param(1, path)) return MsgHandler::kBadParameters;

        if (Tools::isurl(path) || Tools::isurl(parent->getScene()->getRootPath()))
        {
            newmsg = IMessage::create(msg->address(), msg->message());
            newmsg->add(IUrlIntermediateObject::kUrlIntermediateType);
            newmsg->add(objType);
            newmsg->add(path);
            objType = IUrlIntermediateObject::kUrlIntermediateType;
        }
        else
#endif
            newmsg = IMessage::create(*msg);
    }
    else
        newmsg = IMessage::create(*msg);
    
	if (parent->getDeleted())return MsgHandler::kProcessedNoChange;
	SIObject obj = IObjectFactory::create(objName, objType, parent);
	if (obj) {
		if(previousObj) previousObj->transferAttributes(obj);
		int status = obj->execute(newmsg);
		if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) {
			parent->add(obj);
			if (newobj) *newobj = obj;
			return MsgHandler::kProcessed;
		}
		return status;
	}
	return MsgHandler::kCreateFailure;
}


}
