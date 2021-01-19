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


#include <iostream>
using namespace std;

#include "QStretchTilerItem.h"

#include <QPainter>
#include "VExport.h"
#include <QtDebug>

#include "VImageView.h"

#ifdef USEPHONON
#include "QGraphicsVideoItem.h"
#else
#include <QGraphicsVideoItem>
#endif

#define USEPRECISION
//#define PRECISION 1000.0f
//#define PRECISION 0.0001f
//#define PRECISION 1.f

#define kGapAdjust	1.f
//#define DONTADJUST

namespace inscore
{

//-------------------------------------------------------------------------------------------------------------------
#if 1
void QStretchTilerItem::paint ( QPainter * painter , const QStyleOptionGraphicsItem * option, QWidget * )
{
	if ( fNeedCacheUpdate )
	{
		updateCache();
        fNeedCacheUpdate = false;
	}

    chooseCache();
    
    QRectF rect = boundingRect();
    if ((rect.width() == 0) || (rect.height() == 0)) return;

    //the fCache corresponds to the "source" dimensions and the rect to the "destination"
    float xscale = fCache.width()/rect.width();
    float yscale = fCache.height()/rect.height();

    // ensure a minimum 10 x 10 size
/*    while ((rect.width() * xscale) < 20) {
        xscale += 1;
        yscale += 1;
    }
    while ((rect.height() * yscale) < 20){
        xscale += 1;
        yscale += 1;
    }
*/
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    
    VGraphicsImageItem * img = dynamic_cast<VGraphicsImageItem*>(fOriginItem);
    QGraphicsVideoItem * video = dynamic_cast<QGraphicsVideoItem*>(fOriginItem);
    float adjust = (img || video) ? kGapAdjust : 0;

    if(!fMapping.size() && !fIsStretch)
    {
        // the offset means that the top-left points of the images with and without children are not the same.
        QRectF sourceRect = QRectF(0, 0, fCache.width(), fCache.height());
        painter->drawImage(rect, fCache, sourceRect);
    }
    for ( int i = 0 ;  i < fMapping.size(); i++ )
    {
        QRectF sourceRect( fMapping[i].first.x()*xscale, fMapping[i].first.y()*yscale, fMapping[i].first.width()*xscale, fMapping[i].first.height()*yscale);
        QRectF destRect( fMapping[i].second.x(), fMapping[i].second.y(), fMapping[i].second.width() + adjust, fMapping[i].second.height());

//		QGraphicsRectItem * rect = new QGraphicsRectItem( destRect, this );
//		rect->setPen( QPen( QColor(Qt::blue) ));

#ifdef DONTADJUST
        painter->drawImage( fMapping[i].second , fCache , fMapping[i].first );
#else
        painter->drawImage( destRect , fCache , sourceRect );
#endif
    }
    for(int i = 0; i <  childItems().size(); i++)
        childItems()[i]->paint(painter, option) ;
}

#else

//-------------------------------------------------------------------------------------------------------------------
void QStretchTilerItem::paint ( QPainter * painter , const QStyleOptionGraphicsItem * , QWidget * )
{
	if ( fNeedCacheUpdate )
	{
		qDebug() << "QStretchTilerItem::paint: " << fCacheFactor;
		fCache = VExport::itemToImage( fStretchTiledItem , fCacheFactor , fCacheFactor );
		fNeedCacheUpdate = false;
	}
	
//	painter->setRenderHint( QPainter::SmoothPixmapTransform , true );
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

	// Qt pending bug workaround with QPainter::drawPixmap : truncates QRectF to QRect (float to int !).	
#ifdef USEPRECISION
	// a workaround is to scale the painter and draw to a scaled rect (destRect)
	// it improves the precision but that's not enough, 
	// the destination rect needs to be extended one pixel to the right to get correct results
	painter->scale( 1/PRECISION , 1/PRECISION );		
	for ( int i = 0 ;  i < fMapping.size(); i++ )
	{
		QRectF sourceRect( fCacheFactor * fMapping[i].first.x() , fCacheFactor * fMapping[i].first.y() ,
							fCacheFactor * fMapping[i].first.width() , fCacheFactor * fMapping[i].first.height());
		QRectF destRect( PRECISION * fMapping[i].second.x() , PRECISION * fMapping[i].second.y() , 
						 PRECISION * fMapping[i].second.width(), PRECISION * fMapping[i].second.height() );
//						 PRECISION * fMapping[i].second.width() + PRECISION, PRECISION * fMapping[i].second.height() );

#ifdef PIXMAPBASED
		painter->drawPixmap( destRect , fCache , sourceRect );
#else
		painter->drawImage( destRect , fCache , sourceRect );
#endif
	}
	painter->scale( PRECISION , PRECISION );

#else
	// an equivalent solution without scaling but it seems to be less efficient
	for ( int i = 0 ;  i < fMapping.size(); i++ )
	{
		QRectF sourceRect( fCacheFactor * fMapping[i].first.x() , fCacheFactor * fMapping[i].first.y() ,
							fCacheFactor * fMapping[i].first.width() , fCacheFactor * fMapping[i].first.height());
		QRectF destRect( fMapping[i].second.x(), fMapping[i].second.y(), fMapping[i].second.width() + 1.5, fMapping[i].second.height() );

#ifdef PIXMAPBASED
		painter->drawPixmap( destRect , fCache , sourceRect );
#else
		painter->drawImage( destRect , fCache , sourceRect );
#endif
	}
#endif			
}
#endif

//-------------------------------------------------------------------------------------------------------------------
void QStretchTilerItem::addSegment( const QRectF& sourceRect , const QRectF& destRect)
{

	// to be checked ! this is not supposed to be the case (handled by the mappings)
//	if ( !rect().contains(destRect) )
//	{
//		// if we draw outside the tiler rect, the view may not be correctly refreshed
//		qWarning() << "QStretchTilerItem::addSegment: destRect out of the bounds :" << destRect;
//		return;
//	}
    
    fMapping << QRectFPair(sourceRect,destRect);

    //#define INCREASING_CACHE_FACTOR
    #ifdef INCREASING_CACHE_FACTOR
    // Compute the cache factor so that the cache pixmap is large enough to have a nice rendering.
	
    // The cacheFactor will be the biggest destination-rect's dimension/source-rect's dimension ratio.
    float xScaleRatio = destRect.width() / sourceRect.width();
    float yScaleRatio = destRect.height() / sourceRect.height();

    float cacheFactor = qMax( xScaleRatio , yScaleRatio );
    // fCacheFactor never decreases: use the new value only if its bigger.
    if ( cacheFactor > fCacheFactor )
    {
        fCacheFactor = cacheFactor;
        // The cache now needs to be updated, to have a bigger one.
        updateCache();
    }
    #endif
    //	update();
}

}
