/*
  INScore Project

  Copyright (C) 2009,2016  Grame

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

  Grame Research Laboratory, 11 cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/
#include <iostream>
#include <QDir>

#include "BundleExtractor.h"

using namespace qarchive;

namespace ibundle {


bool BundleExtractor::extract(std::string bundleFile, std::string outputFolder)
{
	QArchiveError e;
	qarchive::QArchive* archive = qarchive::QArchive::readArchiveFromFileStd(bundleFile, e);

	if(e!=ARCH_OK){
		switch (e) {
		case qarchive::FILE_NOT_FOUND:
			std::cerr<<bundleFile<<" not found."<<std::endl;
			break;
		case qarchive::FILE_CORRUPTED:
			std::cerr<<bundleFile<<": incorrect format."<<std::endl;
			break;
		case qarchive::HIGHER_BUNDLE_VERSION:
			std::cerr<<"The version of the archive "<<bundleFile<<" is higher than the extractor version, update to the latest version to read the bundle."<<std::endl;
			break;
		default:
			std::cerr<<"An error occurs during the reading of the bundle..."<<std::endl;
			break;
		}
		delete archive;
		return false;
	}

	if(!fKeepOriginalPath){
		bool r = writeArchive(archive, outputFolder, fForceOverwrite);
		delete archive;
		return r;
	}


	// --- Keep original Path ---
	ParsedData parsedData;
	if(!ScriptsParser::readArchive(archive, parsedData)){
		std::cerr<<"Impossible to restore original path for the bundle \""<<bundleFile<<"\""<<std::endl;
		delete archive;
		return false;
	}

	std::map<std::string, std::string> nameMap;
	if(!readMapName(archive, nameMap)){
		std::cerr<<"Impossible to restore original path for the bundle \""<<bundleFile<<"\""<<std::endl;
		delete archive;
		return false;
	}

	parsedData.applyFileMap(nameMap);

	if(!writeMappedArchive(archive, parsedData, nameMap, outputFolder)){
		std::cerr<<"Impossible to restore original path for the bundle \""<<bundleFile<<"\""<<std::endl;
		delete archive;
		return false;
	}

	delete archive;

	return true;
}

bool BundleExtractor::writeArchive(qarchive::QArchive* archive, std::string outputFolder, bool overwrite)
{
	QArchiveError e = archive->extractStd(outputFolder, overwrite);

	std::string r;
	switch(e){
	case qarchive::ARCH_OK:
		return true;
	case qarchive::FILE_EXIST:

		break;
	case qarchive::FILE_CORRUPTED:
		std::cerr<<"Error when uncompressing bundle: the file is corrupted!"<<std::endl;
		break;
	case qarchive::WRONG_PERMISSIONS:
		std::cerr<<"Impossible to write in "<<outputFolder<<std::endl;
		break;
	default:
		break;
	}
	return false;
}

bool BundleExtractor::writeMappedArchive(QArchive* archive, ParsedData& script, const std::map<std::string,std::string>& map, std::string outputPath)
{
	bool overwrite = fForceOverwrite;
	QString outputFolder = QString::fromStdString(outputPath);
	QDir dir;
	if(!dir.mkpath(outputFolder)){
			std::cerr<<"Impossible to write in "<<outputPath<<std::endl;
			return false;
	}
	dir.cd(outputFolder);

	treeConstIterator<int> it = archive->tree().globalIterator();
	Movement m;

	while( (m=it.next()) ){
		if(m == TREE_END)
			return true;

		if(m == ITEM){
			//Find the name in the map
			QString archiveFileName = it.currentPath()+"/"+it.name();
			archiveFileName = archiveFileName.mid(1);
			if(archiveFileName == "bundleMap.txt")
				continue;

			auto itMap = map.find(archiveFileName.toStdString());
			if(itMap==map.end()){
				std::cerr<<"Error when uncompressing bundle: the file is corrupted!"<<std::endl;
				return false;
			}
			QString filename = QString::fromStdString(itMap->second);
			int id = filename.lastIndexOf('/');
			QString filepath = "";
			if(id!=-1)
				filepath = filename.left(id);
			filename = filename.mid(id+1);

			//Ensure the path to the file exist
			QDir fileDir = dir;
			if(!filepath.isEmpty()){
				if(!fileDir.mkpath(filepath)){
					std::cerr<<"Impossible to write in "<<outputPath<<std::endl;
					return false;
				}
				fileDir.cd(filepath);
			}

			QFile f(fileDir.absolutePath()+QDir::separator()+filename);
			if(f.exists()){
				if(!overwrite){
					std::cout<<f.fileName().toStdString()<<" already exist, do you want to overwrite? [O/n]   ";
					std::string r;
					std::cin>>r;
					if(r!="n")
						overwrite = true;
					else
						return false;
				}
				f.remove();
			}
			if(!f.open(QIODevice::WriteOnly)){
				std::cerr<<"Impossible to write "<<f.fileName().toStdString()<<std::endl;
				return false;
			}

			QByteArray filedata;
			if(archiveFileName.startsWith("Ressources/"))
				archive->readFile(archiveFileName, filedata);
			else
//				filedata = QByteArray::fromStdString(script.generateScript(archiveFileName.toStdString()));
				filedata = QByteArray(script.generateScript(archiveFileName.toStdString()).c_str());
			f.write(filedata);
			f.close();
		}
	}
	return true;
}

bool BundleExtractor::readMapName(qarchive::QArchive* archive, std::map<std::string, std::string>& map)
{
	QByteArray data;
	if(!archive->readFile("bundleMap.txt", data)){
		std::cerr<<"No \bundleMap.txt\" found in the archive."<<std::endl;
		return false;
	}

//	QStringList mapping = QString::fromUtf8(data).split('\n',QString::SkipEmptyParts);
	QStringList mapping = QString::fromUtf8(data).split('\n');
	for(QStringList::const_iterator it=mapping.begin(); it!=mapping.end(); it++){
		int id = it->indexOf('\t');
		if(id==-1 || id==it->size()-1){
			std::cerr<<"The bundle's file map is not a valid file map."<<std::endl;
			return false;
		}
		map[it->left(id).toStdString()] = it->mid(id+1).toStdString();
	}
	return true;
}

} // End namespace
