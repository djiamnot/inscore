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


#ifndef __IMessageStream__
#define __IMessageStream__

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

#include "IMessage.h"
#include "maptypes.h"
#include "smartpointer.h"

namespace inscore
{

class IEffect;

/*!
\addtogroup ITLCtrl
@{
*/

//--------------------------------------------------------------------------
// a set of '<<' operators to set a message parameters
//--------------------------------------------------------------------------
IMessage& operator <<(IMessage& msg, const libmapping::rational& val);

template <typename T, unsigned int D>
IMessage& operator <<(IMessage& msg, const libmapping::TRelation<T,D,libmapping::rational,1>& val)
{
	std::ostringstream map;
	val.print (map);
//	msg << map.str();   // looks like the << operator dont work with string
	msg.add(map.str());
	return msg;
}

template <typename T> class TSize;
template <typename T>	IMessage& operator <<(IMessage& msg, TSize<T> val) { msg << val.width() << val.height(); return msg; }
template <>	IMessage& operator <<(IMessage& msg, TSize<double> val);

inline IMessage& operator <<(IMessage& msg, int val)		{ msg.add(val); return msg; }
inline IMessage& operator <<(IMessage& msg, float val)		{ msg.add(val); return msg; }

// boolean is store as integer value
inline IMessage& operator <<(IMessage& msg, bool val)		{ msg.add(int(val)); return msg; }

// strings
IMessage& operator <<(IMessage& msg, const std::string& val);
IMessage& operator <<(IMessage& msg, const char* val);

// TFloatPoint: linearizes x and y
IMessage& operator <<(IMessage& msg, const TFloatPoint& val);

// TFloatPoint: linearizes x and y
IMessage& operator <<(IMessage& msg, const TIntSize& val);

// SIMessageList: put it as is
inline IMessage& operator <<(IMessage& msg, const SIMessageList& val)	{ if (val->list().size()) msg.add(val); return msg; }

// generic input method
//template <typename T> IMessage& operator <<(IMessage& msg, T val)		{ msg.add(val); return msg; }

// catch TUrl lists
template <typename T>	IMessage& operator <<(IMessage& msg, const std::vector<IMessage::TUrl>& urls)
						{ 
							for (unsigned int i =0; i < urls.size(); i++) msg << std::string(urls[i]);
							return msg; 
						}

// catch vectors
template <typename T>	IMessage& operator <<(IMessage& msg, const std::vector<T>& val)
						{ 
							for (unsigned int i =0; i < val.size(); i++) msg << val[i];
							return msg; 
						}
// and pairs
template <typename T>	IMessage& operator <<(IMessage& msg, const std::pair<T,T>& val)
						{ 
							msg << val.first << val.second;
							return msg; 
						}

IMessage& operator << (IMessage& out, const IMessage* m);
std::ostream& operator << (std::ostream& out, const IMessage* m);

IMessage& operator <<(IMessage& msg, const IEffect* effect);

/*!
@}
*/

} // end namespoace

#endif
