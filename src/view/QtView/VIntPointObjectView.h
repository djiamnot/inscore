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


#ifndef __VIntPointObjectView__
#define __VIntPointObjectView__

#include "VGraphicsItemView.h"
class QGraphicsScene;

namespace inscore
{

/*!
\addtogroup ITLView
@{
*/

//--------------------------------------------------------------------------
/**
*	\brief a graphic view of a IGraphicBasedObject.
*/
class VIntPointObjectView: public VGraphicsItemView
{
	public :
		using VGraphicsItemView::updateLocalMapping;
		
		virtual void updateLocalMapping (IGraphicBasedObject * object);

	protected:

		VIntPointObjectView(QGraphicsScene * scene, QGraphicsItem * item): VGraphicsItemView(scene, item) {}
		virtual ~VIntPointObjectView() {}

		virtual GraphicSegment getGraphicSegment( const IntPointSegment& intPointSegment , const IGraphicBasedObject * object , bool& mapOk ) const = 0;		
};

/*!@} */

} // end namespoace

#endif
