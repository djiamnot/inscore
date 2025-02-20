/*

  INScore Project

  Copyright (C) 2015  Grame

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

  Grame Research Laboratory, 11 cours Verdun Gensoul, 69002 Lyon - France
  research@grame.fr

*/
#ifndef VMOBILESCENEVIEW_H
#define VMOBILESCENEVIEW_H

#include "VSceneView.h"

namespace inscore
{
/*!
\addtogroup ITLMobileView
@{
*/

/*!
 * \brief The MobileVSceneView class extend the standard VSceneView class for mobile.
 *
 * The scene view gesture are grabbed to zoom and move inside the scene.
 * The Swipe gesture is gradb to change scene tab.
 */
class VMobileSceneView : public VSceneView
{
	public:
		VMobileSceneView() : VSceneView() {}
		virtual ~VMobileSceneView();

		/*!
		 * \brief foreground Select tab of the scene to put it on front side.
		 */
		void				foreground();

	protected:
		/*!
		 * \brief createGraphicsView Create the graphics view and add it in the tab widget.
		 * \param scene
		 * \param address
		 * \return
		 */
		ZoomingGraphicsView* createGraphicsView(QGraphicsScene * scene, const char * address);
};

/*! @} */

//class MobileZoomingGraphicsView: public ZoomingGraphicsView
//{
////	Q_OBJECT

//	private:
//		bool fFocus = false;
//		bool fDoubleTap = false;

//		qreal fViewScale = 1;
//		qreal fIniViewScale = 1;
//		QPointF fViewTranslate = QPointF(0,0);
//		QPointF fIniViewTranslate = QPointF(0,0);

//	public:
//		MobileZoomingGraphicsView(QGraphicsScene * s);
//		virtual ~MobileZoomingGraphicsView(){}

//		void resetViewZoomTranslate();
//		virtual void doZoomTranslate();

//	protected:
//		/*!
//		 * \brief viewportEvent Gesture event are fired on viewport.
//		 * \param event the event
//		 * \return
//		 */
//		virtual bool viewportEvent(QEvent *event);
//		/*!
//		 * \brief gestureEvent process gesture event
//		 * \param event
//		 * \return
//		 */
//		virtual bool gestureEvent(QGestureEvent *event);
//		/*!
//		 * \brief pinchTriggered process pinch gesture (with two fingers) to zoom and move in the scene.
//		 * \param event
//		 */
//		void pinchTriggered(QPinchGesture *event);
//};

} // end namespoace
#endif // VMOBILESCENEVIEW_H
