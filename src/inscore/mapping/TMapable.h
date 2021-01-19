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


#ifndef __TMapable__
#define __TMapable__

#include <map>
#include <string>

//#include "libmapping/maptypes.h"
#include "maptypes.h"
#include "TMapping.h"

namespace inscore {

/*!
\addtogroup ITLMapping
@{
*/

/*!
	\brief a map-able object is an object that provides a mapping from relative time
	to graphic space
*/
class TMapable
{
	public:
		typedef std::map<std::string, SRelativeTime2GraphicMapping> namedMapping;	
		typedef std::map<std::string, SRelativeTime2GraphicMapping>::const_iterator	const_iterator;
		typedef std::map<std::string, SRelativeTime2GraphicMapping>::iterator		iterator;
		typedef std::map<std::string, SGraphic2GraphicMapping> masterMapping; //the string corresponds to the master name : master->getMaster()->name()
		typedef std::map<std::string, SGraphic2GraphicMapping>::const_iterator	m_const_iterator;
		typedef std::map<std::string, SGraphic2GraphicMapping>::iterator		m_iterator;
    
        typedef std::map<std::string, bool> useMappingBooleans;
		typedef std::map<std::string, bool>::iterator		b_iterator;

	private:
	
	const SRelativeTime2GraphicMapping find (const std::string& name) const;
		  SRelativeTime2GraphicMapping find (const std::string& name);
	const SGraphic2GraphicMapping findS2M (const std::string& name) const;
		  SGraphic2GraphicMapping findS2M (const std::string& name);

	namedMapping					fMappings;					// intended to support different named mappings
	useMappingBooleans				fUseMapping;
	bool							fLocalMapModified;	
	masterMapping                   fSlave2MasterMappings;
	
	public:
		bool fAutoMap;											// a flag to indicate mappings automatically build from object duration

				 TMapable() : fLocalMapModified(false), fAutoMap(false) {}
		virtual ~TMapable() {}
		
		const SRelativeTime2GraphicMapping  getMapping(const std::string& name) const	{ return find (name); }
		const namedMapping& namedMappings() const								{ return fMappings; }

		const SGraphic2GraphicMapping		getSlave2MasterMapping(const std::string& name) const		{ return findS2M(name); }
        const masterMapping& masterMappings() const								{ return fSlave2MasterMappings; }

		bool removeMapping(const std::string& name);			// Returns true if the specified 'name' has been found and removed.
		
        void setMapping				(const std::string& name, SRelativeTime2GraphicMapping m)	{ fMappings[name] = m; }
		void setSlave2MasterMapping (const std::string& name, const SGraphic2GraphicMapping map); 
		
		void	UseGraphic2GraphicMapping (bool val, std::string masterName = "");
    
		bool	UseGraphic2GraphicMapping (std::string masterName = "") const			{ return fUseMapping.find(masterName)->second; }
     
    
		void	localMapModified (bool val)					{ fLocalMapModified = val; }
		bool	localMapModified () const					{ return fLocalMapModified; }
};

/*! @} */

}

#endif
