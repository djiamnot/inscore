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


#ifndef __VSceneView__
#define __VSceneView__

#include <QGraphicsView>
#include <QStack>
#include <QPair>

#include <string>
#include "VDummyObjectView.h"
#include "VEditBox.h"

class QGraphicsScene;
class QImage;
class QGestureEvent;
class QPinchGesture;
class QSwipeGesture;
class QPanGesture;
class QParallelAnimationGroup;
class QRect;

namespace inscore
{

/*!
\addtogroup ITLView
@{
*/
class IObject;
class IScene;
class Master;
class ResizeMoveEventFilter;
class TouchEventFilter;
class ZoomingGraphicsView;

//--------------------------------------------------------------------------
/**
*	\brief a graphic view of a IScene.
*/
class VSceneView : public VDummyObjectView
{
	QGraphicsScene *	fScene;
	QImage *			fImage;
	ResizeMoveEventFilter * fResizeMoveEventFilter;
	TouchEventFilter *	fTouchEventFilter;
	Qt::WindowFlags		fDefaultFlags;

	// Data of a screenshot
	QByteArray			fDataScreenShot;

	// Size of the data of the screen shot
	int					fDataScreenShotSize;

	// Ask for a screenshot
	bool				fUpdateScreenShot;
	
	// a stack to handle events from other threads
	QStack<QPair<QGraphicsItem*, QEvent::Type> > fEvents;

	// Version of the screen updated on each redraw of the screen.
	unsigned long		fNewVersion;

	std::string			fScreenshotFormat;

	// Version of the screen updated on each redraw of the screen.
	VEditBox*			fEditBox;
	

	void				updateOnScreen( IScene * scene );
	void				updateOffScreen( IScene * scene );

	bool				isScreenShotReady() { return fDataScreenShotSize != 0; }

	/*!
	 * \brief setUpdateScreenShot Ask for a update of the screenshot of the scene.
	 * \param format
	 */
	void				setUpdateScreenShot(const char *format);

	QPoint				scenePos(const IScene * scene) const;

	public :
		using VDummyObjectView::updateView;

				 VSceneView();
		virtual ~VSceneView();
		/*!
		 * \brief initialize Initialize scene view  with an address and a scene
		 * \param address the scene OSC address
		 * \param scene the QGraphicsScene or null for offscreen rendering.
		 */
        void				initializeView(const std::string& address, QGraphicsScene * scene);

		bool				copy(unsigned int* dest, int w, int h, bool smooth=false );
		void				setSceneRect(int w, int h)	{ fScene->setSceneRect(0, 0, w, h); }
		void				updateView( IScene * scene );
	
		// this is to handle events posted from another thread
		void				postEvent( QGraphicsItem* item, QEvent::Type e );
		void				execEvents();

		/*!
		 * \brief foreground Set the scene in front of other window.
		 */
		virtual void				foreground();
		QGraphicsScene *	scene() const;
		QGraphicsView *		view()			{ return (QGraphicsView*)fGraphicsView; }

		/*!
		 * \brief updateSreenShot. Update athe screenshot if a request as been send with setUpdateScreenShot.
		 * If no request the previous scrrenshot if invalidated.
		 */
		void				updateSreenShot(bool newVersion);

		/*!
		 * \brief getScreenShot get Screenshot data.
		 * \param format Image format of the data. This is the same format as a QT format (see QImage)
		 * \return
		 */
		const AbstractData		getImage(const char *format);

		/*!
		 * \brief provides the score current version
		 * \return the current score version
		 */
		unsigned long		getVersion() const  { return fNewVersion; }
	
		/*!
		 * \brief gives the scene level edit box
		 * \return the scene edit box
		 */
		void				edit(IObject* o);


	protected:
		ZoomingGraphicsView *	fGraphicsView;
		bool					fOpenGlRendering;

		/*!
		 * \brief createGraphicsView  create a QGraphicsView object for the GraphicsScene. The method is used in intialize method.
		 * \param scene
		 * \param address
		 * \return	a QGraphicsView
		 */
		virtual ZoomingGraphicsView* createGraphicsView(QGraphicsScene * scene, const char * address);

		/*!
		 * \brief change the ZoomingGraphicsView view port.
		 * \param opengl used to switch to/from opengl rendering
		 */
		void swapViewPort(bool opengl);
};

//--------------------------------------------------------------------------
/**
*	\brief the actual view on Qt side
*/
class ZoomingGraphicsView : public QGraphicsView
{
	std::string		fSceneAddress;
	IScene*	fScene;

	public :
				 ZoomingGraphicsView(QGraphicsScene * s);
		virtual ~ZoomingGraphicsView() {}

		void setSceneAddress(const std::string& name)	{ fSceneAddress = name; }
		void setScene		(IScene* scene)		{ fScene = scene; }
		IScene* getScene	()					{ return fScene; }

		/*!
         * \brief should be called on fullscreen change triggered by the UI.
		 */
		void stateChange( bool fullscreen );

		/*!
         * \brief doZoom zoom on the scene and translate the scene in the view. It use scene scale as zoom factor and xorigin / yorigin to translate the scene.
		 */
		virtual void doZoomTranslate();

		/*!
		 * \brief resetViewZoomTranslate Reset the zoom and translation of the view to their default value.
		 */
		void resetViewZoomTranslate();

		/*!
		 * \brief getScaleFactor Get the scale factor of the scene. This value is updated after a zoom with touch gesture.
		 * \return
		 */
		qreal getScaleFactor() { return fScaleFactor; }

		/*!
		 * \brief getXOrigin Get the XOrigin of the scene. This value is updated after a zoom with touch gesture.
		 * \return
		 */
		qreal getXOrigin();

		/*!
		 * \brief getYOrigin Get the YOrigin of the scene. This value is updated after a zoom with touch gesture.
		 * \return
		 */
		qreal getYOrigin();

		/*!
		 * \brief getSceneAddress Get the address or name of the scene.
		 * \return
		 */
		std::string getSceneAddress() const;

	protected:
		// Scale factor used during a zoom
		qreal fScaleFactor;
		// Scale factor used as reference during a zoom. It's the model scale factor.
		qreal fTotalScaleFactor;

		// Offset to translate the scene in the view.
		qreal fHorizontalOffset;
		qreal fVerticalOffset;

		QRect fSceneRect;

		// Touch Event flags
		bool fFocus;


		virtual void	closeEvent	(QCloseEvent *);
		virtual void	paintEvent  (QPaintEvent * );

		/*!
		 * \brief viewportEvent Gesture event are fired on viewport.
		 * \param event the event
		 * \return
		 */
		virtual bool viewportEvent(QEvent *event);
		/*!
		 * \brief gestureEvent process gesture event
		 * \param event
		 * \return
		 */
		virtual bool gestureEvent(QGestureEvent *event);
		/*!
		 * \brief pinchTriggered process pinch gesture (with two fingers) to zoom and move in the scene.
		 * \param event
		 */
		void pinchTriggered(QPinchGesture *event);


		void resizeEvent ( QResizeEvent * );

#ifndef __MOBILE__
		virtual void changeEvent(QEvent *e);
#endif
};


/*!@} */

} // end namespoace

#endif
