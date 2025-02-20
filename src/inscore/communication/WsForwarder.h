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

  Grame Research Laboratory, 11 cours de Verdun Gensoul, 69002 Lyon - France
  research@grame.fr

*/

#pragma once

#include <vector>

#include <QString>
#include <QWebSocket>
#include <QWebSocketServer>

#include "Forwarder.h"
#include "IApplVNodes.h"
#include "IMessage.h"

namespace inscore
{

//--------------------------------------------------------------------------
// Web Socket Forwarder
//--------------------------------------------------------------------------
class WSForwarder : public QWebSocketServer, public ForwardEndPoint
{
	Q_OBJECT

	int			fID = 1;
	std::vector<QWebSocket *> fClients;		///< the clients list

	void clear ();
	void send (QWebSocket *s, const QString& msg);

	public:
				 WSForwarder (const IMessage::TUrl& url, IApplLog* log);
		virtual ~WSForwarder ();

		void 	send (const IMessage * imsg);
		size_t  getClients () const 	{ return fClients.size(); }

	private Q_SLOTS:
		void accept ();
		void disconnect ();
};


} //
