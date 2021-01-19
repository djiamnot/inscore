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


#ifndef __TComposition__
#define __TComposition__

#include "TMapping.h"

namespace libmapping
{

/*!
\addtogroup Relations
@{
*/

//--------------------------------------------------------------------------
/*!
	\brief relations composition
*/
template<typename T1, unsigned int D1, typename T2, unsigned int D2, typename T3, unsigned int D3> 
class TComposition : public TMapping<T1,D1,T3,D3>
{
	protected:
		typedef typename TRelation<T1,D1,T2,D2>::const_iterator	const_iterator;

				TComposition(const TRelation<T1,D1,T2,D2>& r1,  const TRelation<T2,D2,T3,D3>& r2) {
					for (const_iterator i = r1.begin(); i != r1.end(); i++) {
						this->add ( i->first, r2.get(i->second));
					}
				}
		virtual ~TComposition() {}
	
	public:
		/// creates a new composition
		static SMARTP<TMapping<T1,D1,T3,D3> >create(const TRelation<T1,D1,T2,D2>& r1,  const TRelation<T2,D2,T3,D3>& r2)		
			{ return new TComposition<T1,D1,T2,D2,T3,D3>(r1, r2); }
};

/*! @} */

} // end namespoace

#endif
