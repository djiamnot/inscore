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

#include <math.h>

#include "ILine.h"
#include "ITLError.h"
#include "IMessage.h"
#include "Updater.h"

using namespace std;

namespace inscore
{

const string ILine::kLineType("line");

// Initialize map to convert enum array head to string
std::map<enum ILine::ArrowHeadType, std::string> ILine::arrowMap = ILine::createArrowMap();

//--------------------------------------------------------------------------
ILine::ILine( const std::string& name, IObject* parent ) : IShapeMap(name, parent), fWAMode(false),
	fArrowLeft(NONE), fArrowRight(NONE), fArrowSizeLeft(1.0), fArrowSizeRight(1.0)
{ 
	fTypeString = kLineType;
	fGetMsgHandlerMap[""] = TGetParamMsgHandler< TFloatPoint >::create( getPoint() );

	// Arrow specific method
	fGetMsgHandlerMap[karrows_GetSetMethod]	= TGetParamMethodHandler<ILine, pair<string, string> (ILine::*)() const>::create(this, &ILine::getArrows);
	fMsgHandlerMap[karrows_GetSetMethod]	= TMethodMsgHandler<ILine>::create(this, &ILine::setArrowsMsg);

	fPenWidth = 1.0f;
}

//--------------------------------------------------------------------------
void ILine::setHandlers ()
{
	IObject::setHandlers();
	fMsgHandlerMap[kcolor_GetSetMethod]		= TMethodMsgHandler<IColor>::create(&fPenColor, &IColor::set);
}

//--------------------------------------------------------------------------
void ILine::accept (Updater* u)
{
	u->updateTo (SILine(this));
}

//--------------------------------------------------------------------------
void ILine::print (ostream& out) const
{
	IShapeMap::print (out);
	out << fPoint << endl;
}


//--------------------------------------------------------------------------
// the 'get' form without parameter
SIMessageList ILine::getSetMsg() const
{
	SIMessageList outmsgs = IMessageList::create();
	SIMessage msg = IMessage::create(getOSCAddress(), kset_SetMethod);
	*msg << kLineType << "xy" << fPoint;
	outmsgs->list().push_back (msg);
	return outmsgs;
}

//--------------------------------------------------------------------------
bool ILine::date2FramePoint(const libmapping::rational& date, TFloatPoint& p) const
{
	const libmapping::rational dur = getDuration();
	if ((date < 0.) || (date > dur))	return false;
	
	TFloatPoint o(0,0);
	float len = fPoint.distance (o);
	float datelen = len * float(date) / float(dur);  // convert the date into a length
	float ratio = datelen / len;					 // this is the relative position on the line
	p.fX = fPoint.x() * ratio / getWidth();
	p.fY = fPoint.y() * ratio / getHeight();
	if (fPoint.x() < 0) p.fX += 1;
	if (fPoint.y() < 0) p.fY += 1;
	return true;
}

//--------------------------------------------------------------------------
void ILine::setPoint(const TFloatPoint& p)
{
	fPoint = p;
	fAngle = atan(p.y() / p.x()) * 180 / M_PI;
	float a = fAngle * M_PI / 180;
	fLineWidth = a ? p.y() / sin(a) : p.x() / cos(a);
//	cerr << "ILine::setPoint computed " << p << " aw: " << fAngle << " " << fLineWidth << endl;
}

//--------------------------------------------------------------------------
MsgHandler::msgStatus ILine::set (const IMessage* msg)	
{
	MsgHandler::msgStatus status = IObject::set(msg);
	if (status & (MsgHandler::kProcessed + MsgHandler::kProcessedNoChange)) return status;
	if (msg->size() == 3) {
		float x, y;
		if ((!msg->param(1, x)) || (!msg->param(2, y)))
			return MsgHandler::kBadParameters;
		setPoint( TFloatPoint(x,y) );
		status = MsgHandler::kProcessed;
		newData(true);
		ITLErr << "set line without mode is deprecated" << ITLEndl;
	}
	else if (msg->size() == 4) {
		string mode; float a, b; // int ai, bi;
		if (!msg->param(1, mode))
			return MsgHandler::kBadParameters;
		if (!msg->cast_param(2, a)) return MsgHandler::kBadParameters;
//			if (msg->param(2, ai)) a = float(ai);
//			else return MsgHandler::kBadParameters;
//		}
		if (!msg->cast_param(3, b)) return MsgHandler::kBadParameters;
//			if (msg->param(3, bi)) b = float(bi);
//			else return MsgHandler::kBadParameters;
//		}
		if (mode == "xy") {
			setPoint( TFloatPoint(a, b) );
		}
		else if (mode == "wa") {
			fWAMode = true;
			double x = a * cos(M_PI * b / 180);
			double y = a * sin(M_PI * b / 180);
//cerr << "ILine::set wa " << b << " " << a << " computed angle: " /*<< acos(x / a) * 180 / M_PI << " "*/ << asin(y / a) * 180 / M_PI << endl;
			setPoint( TFloatPoint(x, y) );
		}
		else return MsgHandler::kBadParameters;
		newData(true);
		status = MsgHandler::kProcessed;
	}
	else status = MsgHandler::kBadParameters;
	return status;
}

//--------------------------------------------------------------------------
bool ILine::getArrowType(string typeString, enum ArrowHeadType & type)
{
	if(typeString == "none") {
		type = NONE;
		return true;
	}
	if(typeString == "triangle") {
		type = TRIANGLE;
		return true;
	}
	if(typeString == "diamond") {
		type = DIAMOND;
		return true;
	}
	if(typeString == "disk") {
		type = DISK;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
map<enum ILine::ArrowHeadType, string> ILine::createArrowMap()
{
	map<enum ArrowHeadType, string> myMap;
	myMap[NONE] = "none";
	myMap[TRIANGLE] = "triangle";
	myMap[DIAMOND] = "diamond";
	myMap[DISK] = "disk";
	return myMap;
}

//--------------------------------------------------------------------------
MsgHandler::msgStatus ILine::setArrowsMsg(const IMessage* msg)
{
	if(msg->size() != 2)
		return MsgHandler::kBadParameters;
	string type;
	enum ArrowHeadType left, right;
	if (msg->param(0, type)) {
		if(!getArrowType(type, left))
			return MsgHandler::kBadParameters;
	}
	if (msg->param(1, type)) {
		if(!getArrowType(type, right))
			return MsgHandler::kBadParameters;
	}
	fArrowLeft = left;
	fArrowRight = right;
	newData(true);
	return MsgHandler::kProcessed;
}

//--------------------------------------------------------------------------
pair<string, string> ILine::getArrows() const
{
	pair<string, string> heads;
	heads.first = arrowMap[fArrowLeft];
	heads.second = arrowMap[fArrowRight];

	return heads;
}

}
