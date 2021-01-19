/*

  INScore Project

  Copyright (C) 2016  Grame

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
#include <cmath>

#include <QOrientationSensor>
#include <QOrientationReading>

#include "IOrientation.h"
#include "ITLError.h"

using namespace std;

namespace inscore
{

const string IOrientation::kOrientationType = "orientation";
// map the Qt values so that continuous values are emittd when the phone goes
// from top up, face up, next clockwise rotation and finally top down
const float  IOrientation::fClockWiseMap[7]			= { 0, 1, 6, 3, 5, 2, 4 };
// the same but counter clockwise rotation
const float  IOrientation::fCounterClockWiseMap[7]	= { 0, 1, 6, 5, 3, 2, 4 };

static const string kClockwiseMode	("clockwise");
static const string kCClockwiseMode	("counterClockwise");

//------------------------------------------------------------------------
IOrientation::IOrientation(const std::string& name, IObject * parent)
	: IQSensor (name, parent)
{
	fTypeString = kOrientationType;
	fDefaultValue = 0.f;
	fDirection = fClockWiseMap;
	if (isSignal()) fDefaultScale = 1/3.f;
	setScale ( fDefaultScale );
}
IOrientation::~IOrientation() {}

//------------------------------------------------------------------------
float IOrientation::read ()
{
	QOrientationReading* reader = sensor()->reading();
	return reader ? fDirection[reader->orientation()] : 0;
}

//------------------------------------------------------------------------
void IOrientation::setMode (const std::string& mode)
{
	if (mode == kClockwiseMode)
		fDirection = fClockWiseMap;
	else if (mode == kCClockwiseMode)
		fDirection = fCounterClockWiseMap;
	else
		ITLErr << getOSCAddress() <<  kmode_GetSetMethod << mode << ": invalid mode." << ITLEndl;
}

//------------------------------------------------------------------------
std::string IOrientation::getMode () const
{
	return fDirection == fCounterClockWiseMap ? kCClockwiseMode : kClockwiseMode;
}

//------------------------------------------------------------------------
void IOrientation::setHandlers()
{
	I1DSensor::setHandlers();
	fGetMsgHandlerMap[kmode_GetSetMethod]	= TGetParamMethodHandler<IOrientation, string (IOrientation::*)() const>::create(this, &IOrientation::getMode);
	fMsgHandlerMap[kmode_GetSetMethod]		= TSetMethodMsgHandler<IOrientation, string>::create(this, &IOrientation::setMode);
}

} // end namespace


