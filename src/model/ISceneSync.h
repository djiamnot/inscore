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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#ifndef __ISceneSync__
#define __ISceneSync__

#include "IVNode.h"
#include "ISync.h"

namespace inscore
{

class IMessage;

/*!
\addtogroup ITLModel
@{
*/

class ISceneSync;
typedef class libmapping::SMARTP<ISceneSync>	SISceneSync;
//--------------------------------------------------------------------------
/*! \brief the scene synchronization node
*/
class ISceneSync : public IVNode
{	
	ISync	fSync;

	MsgHandler::msgStatus syncMsg (const std::string& slave);
	MsgHandler::msgStatus syncMsg ( const std::string& slave, const std::string& slaveMap, 
									const std::string& master, const std::string& masterMap,
									Master::StretchType stretch, Master::SyncType sync, Master::VAlignType valign);
	
	public:	
		static const std::string fTypeString;
		static libmapping::SMARTP<ISceneSync> create(IObject * parent)	{ return new ISceneSync(parent); }

		virtual void	print(std::ostream& out) const;
		virtual void	accept (Updater*);

		/*! \brief gives the master of an object
			\param o the object to look for in the synchronization set
			\return the master when found
		*/
		virtual SMaster getMaster(SIObject o) const;

		/*! \brief synchronizes two objects
			\param o a slave object
			\param m the master object
		*/
		virtual void sync(const SIObject& o, SMaster m)	{ fSync.sync (o, m);}

		/*! \brief breaks a synchronization link
			\param o a slave object
		*/
		virtual void delsync(SIObject& o)						{ fSync.remove (o);}

		/*! \brief cleanup the relations set
			\see ISync::cleanup
		*/
		virtual void cleanup()	{ fSync.cleanup(); IObject::cleanup(); }

		/// \brief makes a topological sort of the scene elements according to their synchronizations set
		virtual void	sort (IObject::subnodes& nodes);

		/// \brief a periodic task to propagate modification state from masters to slaves
		virtual void ptask ();


				ISync& getSync()			{ return fSync; }
		const	ISync& getSync() const		{ return fSync; }

	protected:
				 ISceneSync(IObject * parent);
		virtual ~ISceneSync() {}

		/// \brief splits a compound name to a name and map name
		virtual bool name2mapName (const std::string& str, std::string& name, std::string& map) const;

		/// \brief sync \c 'get' message handler
		virtual SIMessageList getAllParams () const;
		/// \brief sync \c 'get' message handler
		virtual SIMessageList getMsgs (const IMessage* msg) const;
		/// \brief sync message handler
		virtual MsgHandler::msgStatus syncMsg (const IMessage*);
};

/*!
@}
*/

} // end namespoace

#endif
