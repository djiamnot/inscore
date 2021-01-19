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


#pragma once

#include "Modules.h"

#include "IAppl.h"
#include "IApplVNodes.h"
#include "IArc.h"
#include "IAudio.h"
#include "IColor.h"
#include "ICurve.h"
#include "IEllipse.h"
#include "IFileWatcher.h"
#if INCLUDEFaust
#include "IFaustProcessor.h"
#include "IFaustDSP.h"
#include "IFaustDSPFile.h"
#endif
#if INCLUDEGestureFollower
#include "IGestureFollower.h"
#endif
#include "IGraphicSignal.h"
#include "IGrid.h"
#include "IGuidoCode.h"
#include "IGuidoFile.h"
#include "IGuidoPianoRoll.h"
#include "IGuidoPianoRollFile.h"
#include "IGuidoPianoRollStream.h"
#include "IGuidoStream.h"
#include "IHtml.h"
#include "IHtmlFile.h"
#include "IImage.h"
#include "ILayer.h"
#include "ILine.h"
#include "IMemImage.h"
#include "IMusicXMLCode.h"
#include "IMusicXMLFile.h"
#include "IMenu.h"
#include "IObject.h"
#include "IObjectFactory.h"
#include "IObjectVNodes.h"
#include "IPolygon.h"
#include "IRect.h"
#include "IScene.h"
#include "ISignal.h"
#include "ISync.h"
#include "ISVG.h"
#include "ISVGFile.h"
#include "IText.h"
#include "ITextFile.h"
#include "IVideo.h"
#include "IUrlIntermediateObject.h"

#if INCLUDEHttp
#include "IHttpd.h"
#endif
#if INCLUDEWebSocket
#include "IWebSocket.h"
#endif

