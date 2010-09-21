/*

  Interlude Prototype
  Copyright (C) 2009  Grame

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
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include <stdexcept>
#include <iostream>

#include <QMessageBox>
#include <QDir>

#include "EventsAble.h"
#include "IAppl.h"
#include "IGlue.h"
#include "interlude.h"
#include "IScene.h"
#include "ISync.h"
#include "IMessage.h"
#include "OSCStream.h"
#include "IMappingUpdater.h"
#include "ViewFactory.h"

#include "benchtools.h"

using namespace std;

namespace interlude
{

extern SIMessageStack gMsgStask;

//--------------------------------------------------------------------------
IGlue::IGlue(int udpport, int outport, int errport) : fOscThread(0), fTimerID(0), fUDP(udpport, outport, errport) {}
IGlue::~IGlue()	{ clean(); }

//--------------------------------------------------------------------------
void IGlue::clean()
{
	if (fOscThread) {
        fOscThread->terminate();
        fOscThread->wait(50);
	}
	killTimer (fTimerID);
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
void IGlue::initialize ()
{
	Master::initMap();
	EventsAble::init();

	fMsgStack = IMessageStack::create();
	gMsgStask = fMsgStack;
	fController = IController::create();
//	fController->setListener (this);

	fModel = IAppl::create(fUDP.fInPort, fUDP.fOutPort, fUDP.fErrPort);
	fModel->createVirtualNodes();
	fModel->setView (ViewFactory::create(fModel));

	SIScene	scene = IScene::create(fModel);
	scene->createVirtualNodes();
	scene->setView (ViewFactory::create(scene));
	fModel->add (scene);

	fTimeTask = scene;
	if (!OSCStream::start())
		throw("Cannot initialize output udp streams");
	oscinit (fModel, fUDP);
	if (!fMsgStack || !fController || !fModel || !fOscThread)
		throw("Memory allocation failed!");
	oscerr << OSCStart("Interlude") << "v" << Interlude::versionStr() << " listening on port " <<  fUDP.fInPort << OSCEnd();
	
	// creates a mapping updater - note that it may send error messages and thus should not be
	// set before the osc streams are ready
	setSlaveMapUpdater(new IMappingUpdater);

#ifdef RUNBENCH
	fModel->resetBench();
#endif
}

//--------------------------------------------------------------------------
bool IGlue::start (int timeInterval)
{
	try {
		initialize();
		fTimerID = startTimer(timeInterval);
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
void IGlue::localMapUpdate()
{
	if (fLocalMapUpdater)
		fLocalMapUpdater->update (fModel);
}

//--------------------------------------------------------------------------
void IGlue::slaveMapUpdate()
{
	if (fSlaveMapUpdater)
		fSlaveMapUpdater->update (fModel);
}

//--------------------------------------------------------------------------
void IGlue::viewUpdate()
{
	if (fViewUpdater)
		fViewUpdater->update (fModel);
}


//--------------------------------------------------------------------------
void IGlue::timerEvent ( QTimerEvent *)
{
#ifdef RUNBENCH
	static __uint64 time = 0;
	__uint64 t = getTime();
	if (time) bench::put ("time", t - time);
	else  bench::put ("time", 0);
	time = t;
#endif

	if (fMsgStack->size()) {
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

