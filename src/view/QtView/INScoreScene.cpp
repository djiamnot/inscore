/*

  Interlude Prototype
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

#include <QString>
#include <QEvent>
#include <QMimeData>
#include <QUrl>
#include <QGraphicsSceneDragDropEvent>

#include <iostream>
#include <sstream>
#include <string>
#include <ctype.h>

#include "IAppl.h"
#include "ITLparser.h"
#include "IMessage.h"
#include "INScore.h"
#include "INScoreScene.h"
#include "IScene.h"
#include "Tools.h"

using namespace std;
using namespace inscore;
using namespace libmapping;

//_______________________________________________________________________
INScoreScene::INScoreScene (const std::string& address, inscore::IScene* scene)
	: QGraphicsScene (), fScene(scene), fOscAddress(address) {}

//_______________________________________________________________________
//static string tolower (string& str)
//{
//	for (unsigned int i=0; i<str.size(); i++)
//		str[i] = tolower(str[i]);
//	return str;
//}

//_______________________________________________________________________
//const char* INScoreScene::extension (const char* fullpath) const
//{
//	string file (fullpath);
//	size_t pos = file.find_last_of (".");
//	if (pos != string::npos)
//		return &fullpath[pos+1];
//	return 0;
//}

//_______________________________________________________________________
string INScoreScene::makename (const char* fullpath) const
{
	string file (fullpath);
	size_t startpos = file.find_last_of ("/");
	size_t endpos = file.find_last_of (".");
	if (startpos == string::npos) startpos = 0;
	else startpos += 1;

	size_t length = (endpos == string::npos) ? string::npos : 
		(endpos > startpos) ? endpos - startpos : string::npos;
	return tr (file.substr(startpos, length));
}
					
//_______________________________________________________________________
string INScoreScene::tr(const string& name) const
{
	string outstr (name);
	for (unsigned int i = 0; i < outstr.size(); i++) {
		switch (outstr[i]) {
			case ' ': case '	':
				outstr[i] = '_';
				break;
			case '#': case '*': case ',': case '/': case '?':
			case '[': case ']': case '{': case '}':
				outstr[i] = '_';
				break;
		}
	}
	return outstr;
}

//_______________________________________________________________________
void INScoreScene::open (const char* fullpath)
{
//	const char* ptr = extension(fullpath);
	const char* ptr = Tools::getFileExtension(fullpath);
	string ext;
	if (ptr) ext = Tools::tolower (ptr);
	if (ext == "inscore" || ext == "inscore2" || ext == "ibundle")
		openscene (fullpath);
	else
		openfile (fullpath);
}

//_______________________________________________________________________
void INScoreScene::openscene (const char* fullpath)
{
	string file (fullpath);
	size_t pos = file.find_last_of ("/");
	string appl("/");
	appl += IAppl::kName;
	if (pos != string::npos) {
		string path = file.substr(0, pos);
		INScore::postMessage (fOscAddress.c_str(), krootPath_GetSetMethod, path.c_str());
		INScore::postMessage (appl.c_str(), krootPath_GetSetMethod, path.c_str());
	}
	INScore::postMessage (fOscAddress.c_str(), kload_SetMethod, file.c_str());
}

//_______________________________________________________________________
void INScoreScene::openfile (const char* fullpath)
{
	string file (fullpath);
	string address = fOscAddress;
	address += "/";
	address += makename (fullpath);
	INScore::MessagePtr msg = INScore::newMessage (kset_SetMethod);
	INScore::add (msg, "file");
	INScore::add (msg, file.c_str());
	INScore::postMessage (address.c_str(), msg);
}

//_______________________________________________________________________
void INScoreScene::dropEvent ( QGraphicsSceneDragDropEvent * event )
{
	if (event->mimeData()->hasUrls()) {
		QList<QUrl> urls = event->mimeData()->urls();
		for (QList<QUrl>::const_iterator i = urls.begin(); i != urls.end(); i++) {
			QString fileName = i->toLocalFile();
			open ( fileName.toLocal8Bit().data() );
		}
		event->accept();
	}
	else if (event->mimeData()->hasText()) {
		TILoader loader;
		loader.loadString(event->mimeData()->text().toStdString(), fScene, fScene->getParseVersion());
		event->accept();
	}
}

//_______________________________________________________________________
void INScoreScene::dragEnterEvent ( QGraphicsSceneDragDropEvent * event )
{
	if (event->mimeData()->hasUrls() || event->mimeData()->hasText())
         event->acceptProposedAction();
}
//_______________________________________________________________________
void INScoreScene::dragMoveEvent ( QGraphicsSceneDragDropEvent * event )
{
	if (event->mimeData()->hasUrls() || event->mimeData()->hasText())
         event->acceptProposedAction();
}
