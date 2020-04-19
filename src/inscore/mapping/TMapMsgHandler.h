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


#ifndef __TMapMsgHandler__
#define __TMapMsgHandler__

#include <istream>
#include <sstream>
#include <fstream>

#include "TLocalMapping.h"
#include "IMessage.h"
#include "IObject.h"
#include "IScene.h"

#include "imapreader.h"
#include "long_to_rational_reader.h"
#include "long2D_to_rational_reader.h"
#include "float_to_rational_reader.h"
#include "float2D_to_rational_reader.h"

namespace inscore
{

class IObject;

/*!
\addtogroup ITLModel
@{
*/

//--------------------------------------------------------------------------
// type generalization of map readers
template <typename T, unsigned int D> class _imapreader {};
template <> class _imapreader<long, 1>  : public libmapping::long_to_rational_reader {};
template <> class _imapreader<long, 2>  : public libmapping::long2D_to_rational_reader {};
template <> class _imapreader<float, 1> : public libmapping::float_to_rational_reader {};
template <> class _imapreader<float, 2> : public libmapping::float2D_to_rational_reader {};


//--------------------------------------------------------------------------
/*!
	\brief a class to handle map related messages
*/
template<typename T, unsigned int D> class TMapMsgHandler
{
	static MsgHandler::msgStatus setMapping (libmapping::SMARTP<TLocalMapping<T,D> >& localMapping , IObject* object, std::istream* in, const std::string& mapName, bool add=false)
	{
		_imapreader<T,D> reader;
		imapreader r (&reader, in);
		if (r.parse()) {
			if (add)
				localMapping->addMapping( mapName , reader.mapping() );	// add the new localMapping without erasing the previous one.
			else
				localMapping->setMapping( mapName , reader.mapping() );	// Set the new localMapping, also erasing the previous local<->graphic mapping.
																		// (which is no longer up-to-date)							
			if (mapName.empty()) object->fAutoMap = false;
			return MsgHandler::kProcessed;
		}
		return MsgHandler::kBadParameters;
	}

	static bool parseMsg (const IMessage* msg , std::string& mapname,  std::string& data)
	{
		switch (msg->size()) {
			case 2:
				if (!msg->param(0, mapname)) return false;
				if (!msg->param(1, data)) return false;
				break;
			case 1:
				if (!msg->param(0, data)) return false;
				mapname.clear();
				break;
			default: return false;
		}
		return data.size() > 0;
	}

	public:
	
		/// \brief the \c 'map+' message handler
		static MsgHandler::msgStatus addMapMsg (const IMessage* msg , libmapping::SMARTP<TLocalMapping<T,D> >& localMapping , IObject* object )
		{
			std::string mapName, map;
			if (parseMsg (msg, mapName, map)) {
				std::istringstream s(map);
				return setMapping (localMapping, object, &s, mapName, true);
			}
			return MsgHandler::kBadParameters;
		}
	
		/// \brief the \c 'map' message handler
		static MsgHandler::msgStatus mapMsg (const IMessage* msg , libmapping::SMARTP<TLocalMapping<T,D> >& localMapping , IObject* object )
		{
			std::string mapName, map;
			if (parseMsg (msg, mapName, map)) {
				if ( map == kdel_SetMethod ) {
					localMapping->remove( mapName );
					object->removeMapping( mapName );
					return MsgHandler::kProcessed;
				}
				else {
					std::stringstream s(map);
					return setMapping (localMapping, object, &s, mapName);
				}
			}
			return MsgHandler::kBadParameters;
		}

		/// \brief the \c 'mapf' message handler
		static MsgHandler::msgStatus mapFileMsg (const IMessage* msg , libmapping::SMARTP<TLocalMapping<T,D> >& localMapping , IObject* object )
		{ 
			std::string mapName, file;
			if (parseMsg (msg, mapName, file)) {
				file = object->getScene()->absolutePath(file);
				std::ifstream s(file.c_str());
				return setMapping (localMapping, object, &s, mapName);
			}
			return MsgHandler::kBadParameters;
		}
		
		/// \brief Overrides IObject to handle 'get map' msg.
		static SIMessageList getMapMsgs(const libmapping::SMARTP<TLocalMapping<T,D> >& localMapping , const IObject* object )
		{ 
			SIMessageList outMsgs = IMessageList::create();
			for ( auto m: localMapping->namedMappings())
			{
				SIMessage msg = IMessage::create(object->getOSCAddress(), kmap_GetSetMethod);
				if ( m.first.size() )
					*msg << m.first;
				*msg << m.second->direct();
				outMsgs->list().push_back(msg);
			}
			return outMsgs;
		}
};

/*! @} */

} // end namespoace

#endif
