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

#ifndef __ISensor__
#define __ISensor__

#include <QObject>

#include "ISignal.h"

class QSensor;

//#define SENSORDEBUG

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

//------------------------------------------------------------------------
class ISensor : public ISignal, public QObject
{
	private:
		QSensor*	fSensor;
		int			fTimerID;
		int			fActive;		// ref count of clients activations
		float		fAlpha;			// the smoothing factor
	
	public:
		static const std::string kSensorType;
		virtual		bool activate(bool val) = 0;	// needs to handle shared sensors at higher level
		virtual		bool isSignal() const		{ return fIsSignal; }
		virtual		bool start(int val);	// val controls the hardware device start
		virtual		void stop(int val);		// val controls the hardware device stop
	
	protected:
		bool		fIsSignal;
		bool		fAutoScaling;
		float		fDefaultScale;	// the default scale

				 ISensor(const std::string& name, IObject * parent);
		virtual ~ISensor();

		void timerEvent(QTimerEvent * );
		virtual void readData () = 0;			///< called by the time task

		/// \brief sets the message handlers.
		virtual void setHandlers ();

		float smooth (float val, float prev)	{ return fAlpha*val + (1-fAlpha)*prev;}
		virtual float getSmooth () const		{ return fAlpha; }
		virtual void  setSmooth (float alpha)	{ fAlpha = std::max(0.f, std::min(alpha, 1.f)); }
		virtual void  reset ()					{ fAlpha = 1.f; setScale(fDefaultScale); }
		virtual void  autoscale (bool on)		{ fAutoScaling = on; }

	
		SIMessageList getSetMsg() const;

		/// \brief the \c 'run' message handler
		virtual MsgHandler::msgStatus run(const IMessage* msg);
		virtual bool running () const;

		/// \brief object \c 'set' message handler.
		virtual MsgHandler::msgStatus set (const IMessage* msg);
		virtual MsgHandler::msgStatus sizeMsg (const IMessage* msg);

		void		print (IMessage& out) const;
		bool		setSensor(QSensor* sensor);
		QSensor*	getSensor() const			{ return fSensor; };
};


/*! @} */

} // end namespoace

#endif
