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

#include "VLineView.h"
#include "ILine.h"
#include <QDebug>

namespace inscore
{

//----------------------------------------------------------------------
VLineView::VLineView(QGraphicsScene * scene, const ILine* h) 
	: VMappedShapeView( scene , new IQGraphicsPathItem(h))
    {}

//----------------------------------------------------------------------
/*
void VLineView::updateView( ILine * line )
{
    line->cleanupSync();
    QPainterPath myPath;
	myPath.moveTo(0,0);
	myPath.lineTo( relative2SceneWidth(line->getPoint().x()) , relative2SceneHeight(line->getPoint().y()) );

	if ( myPath != item()->path() )
	{
		item()->setPath( myPath );
		itemChanged();
	}
	VShapeView::updateView( line );
}
*/


//----------------------------------------------------------------------
void VLineView::updateView( ILine * line )
{
    line->cleanupSync();

	double x = relative2SceneX(line->getPoint().x());
	double y = relative2SceneY(line->getPoint().y());
	double xo = 0;
	double yo = 0;

	if (fParent) {	// a parent is a QGraphicsItem that differs from a scene regarding the reference rect:
		QRectF r = referenceRect(0);	// a scene origin (the point in 0,0) is centered on the scene
		x -= r.width()/2;				// while a QGraphicsItem origin is at the top left corner
		y -= r.height()/2;				// this part is intended to compensate this difference
	}

	if ((x < 0) && (y < 0)) {
		x = -x;
		y = -y;
	}
	else if (x < 0) {
		yo = y;
		x = -x;
		y = 0;
	}
	else if (y < 0) {
		yo = -y;
		y = 0;
	}

	QPainterPath myPath;
	myPath.moveTo( xo,yo);
	myPath.lineTo( x, y );
	if ( myPath != item()->path() )
	{
		item()->setPath( myPath );
		itemChanged();
	}
	VShapeView::updateView( line );
}



//----------------------------------------------------------------------
void VLineView::updateObjectSize(IObject* o)
{
    ILine * p = dynamic_cast<ILine*>(o);
    if(p)
        updateView(p);
    VGraphicsItemView::updateObjectSize(o);
}

} // end namespoace
