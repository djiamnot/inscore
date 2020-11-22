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


#ifndef __IModelTypes__
#define __IModelTypes__

#include "smartpointer.h"
#include "Modules.h"


namespace inscore
{

class IMessage;

class IAppl;
typedef class libmapping::SMARTP<IAppl>			SIAppl;

class IApplLog;
typedef class libmapping::SMARTP<IApplLog>		SIApplLog;

class IArc;
typedef class libmapping::SMARTP<IArc>			SIArc;

class IAudio;
typedef class libmapping::SMARTP<IAudio>		SIAudio;

class IColor;
typedef class libmapping::SMARTP<IColor>		SIColor;

class ICurve;
typedef class libmapping::SMARTP<ICurve>		SICurve;

class IEllipse;
typedef class libmapping::SMARTP<IEllipse>		SIEllipse;

class IGraphicSignal;
typedef class libmapping::SMARTP<IGraphicSignal> SIGraphicSignal;

class ISGraphicSignal;
typedef class libmapping::SMARTP<ISGraphicSignal> SISGraphicSignal;

class IRGraphicSignal;
typedef class libmapping::SMARTP<IRGraphicSignal> SIRGraphicSignal;

class IGraphicBasedObject;
typedef class libmapping::SMARTP<IGraphicBasedObject> SIGraphicBasedObject;

class IGrid;
typedef class libmapping::SMARTP<IGrid> SIGrid;

class ILayer;
typedef class libmapping::SMARTP<ILayer> SILayer;
    
class IGuidoCode;
typedef class libmapping::SMARTP<IGuidoCode>	SIGuidoCode;
    
class IGuidoPianoRoll;
typedef class libmapping::SMARTP<IGuidoPianoRoll> SIGuidoPianoRoll;

class IGuidoPianoRollStream;
typedef class libmapping::SMARTP<IGuidoPianoRollStream> SIGuidoPianoRollSream;

class IGuidoFile;
typedef class libmapping::SMARTP<IGuidoFile>	SIGuidoFile;

class IGuidoPianoRollFile;
typedef class libmapping::SMARTP<IGuidoPianoRollFile>	SIGuidoPianoRollFile;

class IMusicXMLCode;
typedef class libmapping::SMARTP<IMusicXMLCode>	SIMusicXMLCode;

class IMusicXMLFile;
typedef class libmapping::SMARTP<IMusicXMLFile>	SIMusicXMLFile;

class IHtml;
typedef class libmapping::SMARTP<IHtml>			SIHtml;

class IHtmlFile;
typedef class libmapping::SMARTP<IHtmlFile>		SIHtmlFile;

class IImage;
typedef class libmapping::SMARTP<IImage>		SIImage;

class IMemImage;
typedef class libmapping::SMARTP<IMemImage>		SIMemImage;

class IMenu;
typedef class libmapping::SMARTP<IMenu>			SIMenu;

class ILine;
typedef class libmapping::SMARTP<ILine>			SILine;

class IObject;
typedef class libmapping::SMARTP<IObject>		SIObject;

class IObjectDebug;
typedef class libmapping::SMARTP<IObjectDebug>	SIObjectDebug;

class IPDF;
typedef class libmapping::SMARTP<IPDF>			SIPDF;

class IPolygon;
typedef class libmapping::SMARTP<IPolygon>		SIPolygon;

class IRect;
typedef class libmapping::SMARTP<IRect>			SIRect;

class IScene;
typedef class libmapping::SMARTP<IScene>		SIScene;

class ISceneSync;
typedef class libmapping::SMARTP<ISceneSync>	SISceneSync;

class ISignalNode;
typedef class libmapping::SMARTP<ISignalNode>	SISignalNode;

class IShape;
typedef class libmapping::SMARTP<IShape>		SIShape;

class IShapeMap;
typedef class libmapping::SMARTP<IShapeMap>		SIShapeMap;

class IText;
typedef class libmapping::SMARTP<IText>			SIText;

class ITextFile;
typedef class libmapping::SMARTP<ITextFile>		SITextFile;

class ISVG;
typedef class libmapping::SMARTP<ISVG>			SISVG;

class ISVGFile;
typedef class libmapping::SMARTP<ISVGFile>		SISVGFile;

class IVideo;
typedef class libmapping::SMARTP<IVideo>		SIVideo;

class IUrlIntermediateObject;
typedef class libmapping::SMARTP<IUrlIntermediateObject>		SIUrlIntermediateObject;

#if INCLUDEFileWatcher
class IFileWatcher;
typedef class libmapping::SMARTP<IFileWatcher>	SIFileWatcher;
#endif

#if INCLUDEGestureFollower
class IGestureFollower;
typedef class libmapping::SMARTP<IGestureFollower> SIGestureFollower;
#endif

#if INCLUDEHttp
class IHttpd;
typedef class libmapping::SMARTP<IHttpd>		SIHttpd;
#endif

#if INCLUDEFaustWeb
class IFaustProcessor;
typedef class libmapping::SMARTP<IFaustProcessor>	SIFaustProcessor;
#endif

#if INCLUDEWebSocket
class IWebSocket;
typedef class libmapping::SMARTP<IWebSocket>		SIWebSocket;
#endif

class Master;
class ISync;

} // end namespoace

#endif
