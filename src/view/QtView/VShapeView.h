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


#ifndef __VShapeView__
#define __VShapeView__

#include "VGraphicsItemView.h"

#include <QAbstractGraphicsShapeItem>

namespace inscore
{

/*!
\addtogroup ITLView
@{
*/

//--------------------------------------------------------------------------
/**
*	\brief a graphic view of a IShape.
*/
class VShapeView: public VGraphicsItemView
{
	QAbstractGraphicsShapeItem * fAbstractGraphicsShapeItem;

	public :
		using VGraphicsItemView::updateView;

		virtual ~VShapeView() {}
		virtual void updateView( IObject * shape );

	protected:
				VShapeView(QGraphicsScene * scene , QAbstractGraphicsShapeItem * abstractGraphicsShapeItem);
		
		virtual void drawBoundingBox (IObject* ) {}

		void	setQPenStyle(const std::string& penStyle , QPen& pen) const;
        void	setQBrushStyle(const std::string& brushStyle , QBrush& brush) const;
};

/*!@} */

} // end namespoace

#endif
