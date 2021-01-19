/*
  Copyright (c) Grame 2009

  This library is free software; you can redistribute it and modify it under
  the terms of the GNU Library General Public License as published by the
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  research@grame.fr
  
*/

#include <iostream>

#include <QApplication>
#include <QFileInfo>
#include <QDir>

#include "IAppl.h"
#include "QPlugin.h"

using namespace std;

namespace inscore
{

// ------------------------------------------------------------------------------
void QPlugin::locations (const char* library, std::vector<std::string>& list)
{
	if (fLocation.size()) {
		char ending = fLocation[fLocation.length()-1];
#ifdef WIN32
		const char* sep = (ending == '/') || (ending == '\\') ? "" : "/";
#else
		const char* sep = (ending == '/') ? "" : "/";
#endif
		string libpath = IAppl::absolutePath(fLocation)+sep+library;
		list.push_back (libpath);
	}

	list.push_back (IAppl::absolutePath (library));
	QDir dir(QApplication::applicationDirPath());
#if __APPLE__
	dir.cdUp();
#endif
	if (dir.cd("PlugIns"))
		list.push_back (dir.absoluteFilePath(library).toStdString());
	list.push_back (library);
#if __APPLE__
	QString localFramework = qgetenv("HOME") + "/Library/Frameworks/" + library;
	list.push_back (localFramework.toStdString());
#endif
}

// ------------------------------------------------------------------------------
bool QPlugin::load (const char* library)
{
	vector<string> l;
	locations (library, l);
	for (unsigned int i=0; i < l.size(); i++) {
		setFileName ( l[i].c_str() );
		if (QLibrary::load()) return true;
	}
	return false;
}


} // end namespace
