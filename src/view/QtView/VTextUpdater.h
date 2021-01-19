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


#ifndef __VTextUpdater__
#define __VTextUpdater__

#include <string>
#include <ostream>

#include "IController.h"
#include "IModelTypes.h"
#include "Updater.h"

namespace inscore
{

/*!
\addtogroup ITLCtrl
@{
*/

class IObject;
typedef class libmapping::SMARTP<IObject>	SIObject;

//--------------------------------------------------------------------------
/// \brief a textual view of the model state
class inscore_export VTextUpdater : public ViewUpdater
{
	std::ostream&	fOutStream;
	
	public :
		using ViewUpdater::updateTo;

		static libmapping::SMARTP<VTextUpdater> create(std::ostream& out)	{ return new VTextUpdater(out); }

		virtual void updateTo (IObject* model, const Master* master);
		virtual void updateTo (IAppl*, const Master* master);
		virtual void updateTo (IScene*, const Master* master);
		virtual void updateTo (IText* text, const Master* master);
		virtual void updateTo (ITextFile* text, const Master* master);
		virtual void updateTo (IGuidoCode* guidoCode, const Master* master);
		virtual void updateTo (IGuidoFile* guidoFile, const Master* master);
		virtual void updateTo (IImage* img, const Master* master);
		virtual void updateTo (IVideo* v, const Master* master);
		virtual void updateTo (IGraphicSignal* graph, const Master* master);
		virtual void updateTo (IRect* rect, const Master* master);
		virtual void updateTo (IEllipse* ellipse, const Master* master);
		virtual void updateTo (IPolygon* polygon, const Master* master);
		virtual void updateTo (ICurve*, const Master* master=0);
		virtual void updateTo (ILine*, const Master* master=0);
		
	protected :
				 VTextUpdater(std::ostream& out) : fOutStream(out) {};
		virtual ~VTextUpdater() {}

		virtual void print (IObject* model, const Master* master);
};
typedef class libmapping::SMARTP<VTextUpdater>	SVTextUpdater;

/*!@} */

} // end namespoace

#endif
