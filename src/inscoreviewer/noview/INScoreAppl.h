/*

  INScore viewer
  Copyright (C) 2020  Grame

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

#pragma once

#include <string>
#include <QApplication>

#include "INScore.h"

//_______________________________________________________________________
class INScoreAppl : public QApplication, public inscore::INScoreApplicationGlue
{
	Q_OBJECT

	int 					fRate = 0;
	int 					fTimerId = 0;
	inscore::INScoreGlue *	fGlue = 0;

	public :
				 INScoreAppl (int & argc, char ** argv ) : QApplication(argc, argv)	{}
		virtual ~INScoreAppl()	{}

		void	start (int udpinport, int udpoutport);
		void	run ();

		void	startView () override		{}
		void	stopView  () override		{}
		bool	event(QEvent *) override			{ return true; }
		void 	showMouse (bool ) override			{}
		bool 	openUrl (const char* ) override		{ return false; }
		std::string viewVersion() const override	{ return "No view version 1"; }
		std::string getIP() const override			{ return "localhost"; }

	static void open(const std::string& file);

	protected:
		void 	timerEvent(QTimerEvent *event) override;
};

