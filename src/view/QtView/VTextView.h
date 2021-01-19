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


#ifndef __VTextView__
#define __VTextView__

#include "VIntPointObjectView.h"
#include "MouseEventAble.h"

#include <QGraphicsTextItem>
#include <QList>
#include <QGraphicsRectItem>
#include <QMap>

namespace inscore
{

class IText;
class IHtmlFile;
/*!
\addtogroup ITLView
@{
*/

//--------------------------------------------------------------------------
/**
*	\brief a graphic view of a IText.
*/
class VTextView: public VIntPointObjectView
{
	typedef MouseEventAble<QGraphicsTextItem>	IQGraphicsTextItem;

	IQGraphicsTextItem* fTextItem;
	QList< QList<QRectF> > fCharRects;
	const IHtmlFile*	fHtmlFile;
//	QList< QGraphicsItem* > fCharItems;
//	QMap<std::string,int> fMappingVersion;

	// Create static map to use enum of QFont.
	static std::map<std::string, enum QFont::Style> createStyleString2Enum();
	static std::map<std::string, enum QFont::Style> sStyleString2Enum;
	static std::map<std::string, enum QFont::Weight> createWeightString2Enum();
	static std::map<std::string, enum QFont::Weight> sWeightString2Enum;

	public :
		using VIntPointObjectView::updateView;
		using VIntPointObjectView::updateLocalMapping;

				 VTextView(QGraphicsScene * scene, const IText*);
				 VTextView(QGraphicsScene * scene, const IHtmlFile*);
		virtual ~VTextView() {}
		
		virtual void updateView( IText * text );

		virtual void updateLocalMapping (IText* text);
		virtual void updateLocalMapping(IHtml* text);
		
	protected:

		class TextInterface;
		virtual void updateLocalMapping (IText* text, TextInterface& textInterface);
	
		void updateQtTextTable();

		/*!
		 * \brief updateFont Update the font of the text item from the IText object
		 * \param text
		 */
		void updateFont(const IText* text);
		GraphicSegment getGraphicSegment( const IntPointSegment& intPointSegment , const IGraphicBasedObject * object , bool& mapOk ) const;
				
		class TextInterface
		{
			protected:
				QGraphicsTextItem* fItem;
			public:
				TextInterface(QGraphicsTextItem* item) : fItem(item) {}
				virtual ~TextInterface() {}
				virtual QString			get() const = 0;
				virtual void			set(const QString& s)=0;
		};
		class HtmlTextInterface: public TextInterface
		{
			public:
				HtmlTextInterface(QGraphicsTextItem* item) : TextInterface(item) {}
				virtual QString			get() const				{ return fItem->toHtml(); }
				virtual void			set(const QString& s)	{ fItem->setHtml( s ); }
		};
		class PlainTextInterface: public TextInterface
		{
			public:
				PlainTextInterface(QGraphicsTextItem* item) : TextInterface(item) {}
				virtual QString			get() const				{ return fItem->toPlainText(); }
				virtual void			set(const QString& s)	{ fItem->setPlainText( s ); }
		};

};
typedef class libmapping::SMARTP<VTextView>	SVTextView;

/*!@} */

} // end namespoace

#endif
