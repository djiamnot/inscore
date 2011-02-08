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

#include <stdexcept>
#include <iostream>

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QDir>
#include <QMutexLocker>

#include "EventsAble.h"
#include "IAppl.h"
#include "IGlue.h"
#include "INScore.h"
#include "IScene.h"
#include "ISync.h"
#include "IMessage.h"
#include "OSCStream.h"
#include "IMappingUpdater.h"
#include "ViewFactory.h"
#include "VSceneView.h"

#include "benchtools.h"

using namespace std;

namespace inscore
{

extern SIMessageStack gMsgStack;

//--------------------------------------------------------------------------
IGlue::IGlue(int udpport, int outport, int errport) 
	: fOscThread(0), fViewListener(0), fTimerID(0), fUDP(udpport, outport, errport) {}
IGlue::~IGlue()	{ clean(); }

//--------------------------------------------------------------------------
void IGlue::clean()
{
	if (fOscThread) {
        fOscThread->terminate();
        fOscThread->wait(50);
	}
	if (fTimerID) killTimer (fTimerID);
	delete fOscThread;
	OSCStream::stop();
}

//--------------------------------------------------------------------------
// try to open the udp ports
// listening port number (which binding may fail) is allocated dynamically
// actually, tries to bind the next available port until success
// output port numbers remain as set by default
void IGlue::oscinit (SIAppl appl, udpinfo& udp)
{
	int port = udp.fErrPort;
	bool done = false;
	do {
		try {
			oscinit (udp.fInPort);
			oscinit (oscout, udp.fOutDstAddress, udp.fOutPort);
			oscinit (oscerr, udp.fErrDstAddress, udp.fErrPort);
			appl->setUDPInPort (udp.fInPort);
			done = true;
		}
		catch (std::runtime_error e) {
			if ( port - udp.fErrPort > 1000) throw ("Cannot bind udp communication ports");
			udp.fInPort = ++port;
		}
	} while (!done);
}

//--------------------------------------------------------------------------
void IGlue::oscinit (int port)
{
	if (fOscThread) {
		fOscThread->terminate();
		delete fOscThread;
	}
	fOscThread = new OscThread(fMsgStack, fUDP.fInPort=port);
	if (fOscThread) fOscThread->start();
}

//--------------------------------------------------------------------------
void IGlue::oscinit (OSCStream& osc, const std::string& address, int port)
{
	osc.setPort(port);
	osc.setAddress(address);
}

//--------------------------------------------------------------------------
bool IGlue::getSceneView(unsigned int* dest, int w, int h, bool smooth )
{ 
	QMutexLocker locker (&fTimeViewMutex);

//	QRect r = QApplication::desktop()->screenGeometry();
//	float lowestDimension = qMin( r.width(), r.height() );
//	fScene->setWidth((2*w) / lowestDimension);
//	fScene->setHeight((2*h) / lowestDimension);

	return false;
//	return getSceneView()->copy(dest, w, h, smooth );
}

//--------------------------------------------------------------------------
void IGlue::initialize (bool offscreen)
{
	Master::initMap();
	EventsAble::init();

	fMsgStack = IMessageStack::create();
	gMsgStack = fMsgStack;
	fController = IController::create();
//	fController->setListener (this);

	fModel = IAppl::create(fUDP.fInPort, fUDP.fOutPort, fUDP.fErrPort, offscreen);
	fModel->createVirtualNodes();
	fModel->setView (ViewFactory::create(fModel));

	INScore::MessagePtr msg = INScore::newMessage ("new");
	string address (fModel->getOSCAddress());
	address += "/scene";
	INScore::postMessage (address.c_str(), msg);
	

//	SIScene	scene = IScene::create("scene", fModel);
//	fModel->add (scene);
//	scene->createVirtualNodes();
//	scene->setView (new VSceneView (scene->getOSCAddress(), graphscene ));
//	fScene = scene;

//	SIScene	scene1 = IScene::create("scene1", fModel);
//	fModel->add (scene1);
//	scene1->createVirtualNodes();
//	scene1->setView (new VSceneView (scene1->getOSCAddress(), new QGraphicsScene ));

	fTimeTask = fModel;
	if (!OSCStream::start())
		throw("Cannot initialize output udp streams");
	oscinit (fModel, fUDP);
	if (!fMsgStack || !fController || !fModel || !fOscThread)
		throw("Memory allocation failed!");
	oscerr << OSCStart("INScore") << "v" << INScore::versionStr() << " listening on port " <<  fUDP.fInPort << OSCEnd();
	
	// creates a mapping updater - note that it may send error messages and thus should not be
	// set before the osc streams are ready
	setSlaveMapUpdater(new IMappingUpdater);

#ifdef RUNBENCH
	fModel->resetBench();
#endif
}

//--------------------------------------------------------------------------
bool IGlue::start (int timeInterval, bool offscreen)
{
	try {
		initialize(offscreen);
		if (timeInterval) fTimerID = startTimer(timeInterval);
	}
	catch (std::runtime_error e) {
		clean();
		cerr << "Unexpected error: " << e.what() << endl;
		QMessageBox alert (QMessageBox::Critical, "Fatal error", e.what(), QMessageBox::Ok, 0);
		alert.exec();
		return false;
	}
	catch (const char* e) {
		clean();
		cerr << "Unexpected error: " << e << endl;
		QMessageBox alert (QMessageBox::Critical, "Fatal error", e, QMessageBox::Ok, 0);
		alert.exec();
		return false;
	}
	return true;
}

//--------------------------------------------------------------------------
void IGlue::setViewUpdater(SUpdater updater)
{
	fViewUpdater = updater;
	if (fViewUpdater) {
		fViewUpdater->update (fModel);	// force view update when updater changes
									// otherwise the view won't be updated until the next incoming message
									// see timerEvent method
		fModel->cleanup();			// and clean the model
	}
}

//--------------------------------------------------------------------------
void IGlue::setSlaveMapUpdater(SUpdater updater)
{
	fSlaveMapUpdater = updater;
	if (fSlaveMapUpdater) {
		fSlaveMapUpdater->update (fModel);	// force map update when updater changes
	}
}

//--------------------------------------------------------------------------
void IGlue::setLocalMapUpdater(SUpdater updater)
{
	fLocalMapUpdater = updater;
	if (fLocalMapUpdater) {
		fLocalMapUpdater->update (fModel);	// force map update when updater changes
	}
}

//--------------------------------------------------------------------------
void IGlue::setOSCOut (int port) {	oscout.setPort(port); fUDP.fOutPort = port; }
void IGlue::setOSCOut (const std::string& a) { oscout.setAddress(a); fUDP.fOutDstAddress = a;  }
void IGlue::setOSCErr (int port) {	oscerr.setPort(port); fUDP.fErrPort = port; }
void IGlue::setOSCErr (const std::string& a) { oscerr.setAddress(a); fUDP.fErrDstAddress = a;  }

//--------------------------------------------------------------------------
void IGlue::checkUDPChange()
{
	if (fModel->getUDPInPort() != fUDP.fInPort)					// check for udp port number changes
		oscinit (fModel->getUDPInPort());
	if (fModel->getUDPOutPort() != fUDP.fOutPort)				// check for udp out port number changes
		setOSCOut(fModel->getUDPOutPort());
	if (fModel->getUDPErrPort() != fUDP.fErrPort)				// check for udp err port number changes
		setOSCErr(fModel->getUDPErrPort());
}

//--------------------------------------------------------------------------
void IGlue::modelUpdate()
{
	SIObject model(fModel);
	fController->processOn(fMsgStack, model);
}

//--------------------------------------------------------------------------
void IGlue::localMapUpdate()	{ if (fLocalMapUpdater) fLocalMapUpdater->update (fModel); }
void IGlue::slaveMapUpdate()	{ if (fSlaveMapUpdater) fSlaveMapUpdater->update (fModel); }
void IGlue::viewUpdate()		{ if (fViewUpdater) fViewUpdater->update (fModel); }

//--------------------------------------------------------------------------
void IGlue::timerEvent ( QTimerEvent *)
{
	if (fMsgStack->size()) {
		QMutexLocker locker (&fTimeViewMutex);

		timebench ("model", modelUpdate());
		if (fTimeTask) fTimeTask->ptask();
		timebench ("lmap", localMapUpdate());
		timebench ("smap", slaveMapUpdate());
		timebench ("view", viewUpdate());

		if (fModel->getState() & IObject::kModified) {
			checkUDPChange();
			if (fModel->getUDPInPort() != fUDP.fInPort)					// check for udp port number changes
				oscinit (fModel->getUDPInPort());
		}

		if (fModel->getState() & IObject::kSubModified) {
			fController->setListener (fModel->oscDebug() ? this : 0);	// check for debug flag changes
			if (fViewListener) fViewListener->update();
//cout << "timer event update " << fViewListener << endl;
		}
		
		fModel->cleanup();
	}
#ifdef RUNBENCH
	else {
		bench::put ("model", 0);
		bench::put ("lmap", 0);
		bench::put ("smap", 0);
		bench::put ("view", 0);
	}
	bench::put ("total", getTime() - time);
#endif
}

//--------------------------------------------------------------------------
void IGlue::trace( const IMessage* msg, int status )
{
	if (status == MsgHandler::kBadAddress) 
		oscerr << OSCWarn() << "incorrect OSC address: " << msg->address() << OSCEnd();
	if (status == MsgHandler::kBadParameters) {
		oscerr << OSCWarn() << "incorrect parameters: " << msg->address() << msg->message();
		msg->printArgs(oscerr);
		oscerr << OSCEnd();
	}
// cout << msg << endl;
}

//--------------------------------------------------------------------------
void IGlue::msgReceived( const IMessage* msg, int status )
{
	trace(msg, status);
}


} // end namespoace

