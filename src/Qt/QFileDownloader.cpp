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

#include <string>

#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "QFileDownloader.h"
#include "INScore.h"
#include "ITLError.h"
#include "Tools.h"
#include "TILoader.h"

#include "sourcefetcher.h"

using namespace std;

namespace inscore
{

class NetworkAccess{
	static NetworkAccess* gNetworkAccess;

public:
	static NetworkAccess* instance();

	QNetworkAccessManager& qNetAccess() 	{ return fNetworkAccessManager; }
	QNetworkReply* 		get (const QNetworkRequest &request);
	void 				clearCache();
	QNetworkDiskCache* 	cache() const 		{ return fCache; }

protected:
			 NetworkAccess();
	virtual ~NetworkAccess();

	QNetworkDiskCache* 		fCache;
	QNetworkAccessManager 	fNetworkAccessManager;
};

//--------------------------------------------------------------------------
QFileDownloader::QFileDownloader(const char* urlprefix)
{
	if (urlprefix) fUrlPrefix = urlprefix;
	fReply = 0;
}
 
//--------------------------------------------------------------------------
QFileDownloader::~QFileDownloader()
{
	if(fReply)
		fReply->deleteLater();
}

//--------------------------------------------------------------------------
std::string	QFileDownloader::location(const char * file)
{
	if (!Tools::isurl(file))
		return TILoader::makeAbsolutePath( fUrlPrefix, file );
	else
		return file;
}

//--------------------------------------------------------------------------
void QFileDownloader::getAsync (const char* what, const char* address)
{
	fOSCAddress = address;
	QUrl url (location(what).c_str());
    QNetworkRequest request(url);
	request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferNetwork);
	fReply = NetworkAccess::instance()->get(request);
	QObject::connect(fReply, &QNetworkReply::finished, [this](){this->fileDownloaded();});
}

//--------------------------------------------------------------------------
bool QFileDownloader::get (const char* what)
{
	std::string string(what);
	std::string begin;
	begin.assign(string, 0,5);
	if(begin!="qrc:/"){
		char *buff;
		int ret = http_fetch(location(what).c_str(), &buff);
		if (ret == -1) {			// there is an error
			ITLErr << "Can't access url \"" << what << "\": " << http_strerror() << ITLEndl;
			return false;
		}
		fData.clear();
		fData.append(buff, ret);
		free (buff);
		return true;
	}

	//Return ressource file
	QString url(what);
	QFile qrc(url.mid(3));
	if(!qrc.open(QIODevice::ReadOnly))
		return false;
	fData = qrc.readAll();
	return true;
}

const char* QFileDownloader::getCachedAsync(const char *urlString, std::function<void ()> cbUpdate, std::function<void ()> cbFail)
{
	//Asynchronously update the cache
	QUrl url(urlString);
	QNetworkRequest request(url);
	fReply = NetworkAccess::instance()->get(request);
	fInitialURL = url.toString();

	//Sending an error message if download failed or callback cbUpdate if the cache was updated
	QObject::connect(fReply, &QNetworkReply::finished, [this, cbUpdate, cbFail](){
		cbCachedASync(cbUpdate, cbFail);
	});

	//Retreiving the actual value stored in the cache (before the asynchronous update)
	QNetworkDiskCache* cache = NetworkAccess::instance()->cache();

	QIODevice* data = cache->data(url);

	if(data){
		char* d = new char[data->bytesAvailable()];
		strcpy(d, data->readAll().data());
		data->deleteLater();

		return d;
	}
	return 0;
}


//--------------------------------------------------------------------------
void QFileDownloader::fileDownloaded()
{
	bool downloadSucceed;
	if(handleError(downloadSucceed, [this]{fileDownloaded();}))
		return;

	if(downloadSucceed)
    {
		QByteArray data = fReply->readAll();
		fData = data;
		updateSucceded ();
    }
    else {
		printErrors();
		updateFailed ();
	}

}

//--------------------------------------------------------------------------
bool QFileDownloader::handleError( bool &success, const std::function<void ()>& cbFinished)
{

	success = true;

	//  -----   Redirection handling    -------
	QUrl newUrl = fReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
	if(!newUrl.isEmpty() && fReply->rawHeaderList().contains("Location"))
		newUrl = QUrl(fReply->header(QNetworkRequest::LocationHeader).toString());

	if(!newUrl.isEmpty()){
			//Redirection needed
		if(newUrl == fReply->url()){
			ITLErr<< "URL redirection failed: "<<newUrl.toString().toStdString()<<ITLEndl;
			return false;
		}

		fReply->deleteLater();

		fReply = NetworkAccess::instance()->get(QNetworkRequest(newUrl));
		QObject::connect(fReply, &QNetworkReply::finished, [cbFinished]{cbFinished();});
		return true;
	}

	//  -----   Error handling   -----
	if(fReply->error()){
		//If network error try to load from cache
		if(!fReply->property("loadedFromCache").isValid() || !fReply->property("loadedFromCache").toBool() ){
			QNetworkRequest request = fReply->request();
			fReply->deleteLater();
			request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysCache);

			fReply = NetworkAccess::instance()->get(request);
			fReply->setProperty("loadedFromCache", QVariant::fromValue(true));
			QObject::connect(fReply, &QNetworkReply::finished, [cbFinished]{cbFinished();});
			ITLErr << "Url not accessible, trying to load ressource from cache..." << ITLEndl;
			return true;
		}

		// error could not be handled
		success = false;
	}

	return false;
}

//--------------------------------------------------------------------------
void QFileDownloader::updateSucceded()
{
//	emit downloaded();
	INScore::MessagePtr msg = INScore::newMessage ("success");
	INScore::postMessage (fOSCAddress.c_str(), msg);
	fReply->deleteLater();
	fReply = 0;
}

//--------------------------------------------------------------------------
void QFileDownloader::updateFailed()
{
//	emit failed(reply);
    INScore::MessagePtr msg = INScore::newMessage ("error");
	INScore::add (msg, fReply->errorString().toStdString().c_str());
	INScore::postMessage (fOSCAddress.c_str(), msg);
	fReply->deleteLater();
	fReply = 0;
}

//--------------------------------------------------------------------------
void QFileDownloader::cbCachedASync(const std::function<void()>& cbUpdate, const std::function<void()>& cbFail)
{
	bool downloadSucceed;

	if(handleError( downloadSucceed, [this, cbUpdate, cbFail](){cbCachedASync(cbUpdate, cbFail);} ))
		return;

	if(!downloadSucceed){
		//URL download failed
		printErrors();
		cbFail();
	}else if(!fReply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool() || fReply->url().toString() != fInitialURL){
			//The cache have been updated

			if(fReply->url().toString() != fInitialURL){
				//In case of redirection we cheat: we put the redirected data inside the initial url
				QNetworkCacheMetaData meta = NetworkAccess::instance()->cache()->metaData(fReply->url());
				meta.setUrl(QUrl(fInitialURL));
				QIODevice* data = NetworkAccess::instance()->cache()->prepare(meta);
				data->write(fReply->readAll());
				NetworkAccess::instance()->cache()->insert(data);
			}
			cbUpdate();
	}

	fReply->deleteLater();
	fReply = 0;
}

void QFileDownloader::printErrors() const
{
	ITLErr << "Can't access url: " << fReply->errorString().toStdString();
	if(fReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt())
		ITLErr << "[Error " << fReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << "]";
	ITLErr << ITLEndl;
}





/***********************************************************
 *                    NetworkAccess                        *
 **********************************************************/

NetworkAccess* NetworkAccess::gNetworkAccess = 0;

NetworkAccess *NetworkAccess::instance()
{
	if(!gNetworkAccess)
		gNetworkAccess = new NetworkAccess();

	return gNetworkAccess;
}

QNetworkReply *NetworkAccess::get(const QNetworkRequest &request)
{
	QNetworkRequest r = request;
	r.setRawHeader("User-Agent", "INScore");
	return fNetworkAccessManager.get(r);
}


//__________________________________________________

NetworkAccess::NetworkAccess()
{
	fCache = new QNetworkDiskCache(&fNetworkAccessManager);

	//Create temporary folders
	bool tmpCreated = true;
	QDir tmp = QDir::temp();
	if(!tmp.exists("INScore"))
		if(!tmp.mkdir("INScore")){
			//Impossible to create temporary folder
			ITLErr<<"Creation of temporary folder is impossible, URL cache won't be available."<<ITLEndl;
			tmpCreated = false;
		}
	if(tmpCreated){
		tmp.cd("INScore");
		fCache->setCacheDirectory(tmp.absolutePath());
		fNetworkAccessManager.setCache(fCache);
	}
}

NetworkAccess::~NetworkAccess()
{
	delete fCache;
}

void NetworkAccess::clearCache()
{
	fCache->clear();
}




};  //end namespace
