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


#ifndef __ILine__
#define __ILine__

#include "IShapeMap.h"

#include <utility>

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

class Updater;
class ILine;
typedef class libmapping::SMARTP<ILine>	SILine;

//--------------------------------------------------------------------------
/*!
*	\brief a line, with 2 control points. The 1st control point is (0,0).
*/
class ILine : public IShapeMap
{	
	public:
		/// Arrow type for end of line
		enum ArrowHeadType {
			NONE,
			TRIANGLE,
			DIAMOND,
			DISK
		};

	private:
		TFloatPoint fPoint;			// a point that describe the line, assuming that the first point is (0, 0)
		float 		fAngle = 0;
		float 		fLineWidth = 0;
		bool		fWAMode;

		// Arrow head for each side of the line
		enum ArrowHeadType fArrowLeft;
		enum ArrowHeadType fArrowRight;

		// Size of arrow head (fixed to 1.0 for the moment)
		double fArrowSizeLeft;
		double fArrowSizeRight;

		// Map for enum
		static std::map<enum ArrowHeadType, std::string> createArrowMap();
		static std::map<enum ArrowHeadType, std::string> arrowMap;

		/// \brief Get arrow enum type from string
		bool getArrowType(std::string typeString, enum ArrowHeadType &type);

	public:
		static const std::string kLineType;
		static SILine create(const std::string& name, IObject* parent)	{ return new ILine(name, parent); }

		const TFloatPoint&	getPoint() const			{ return fPoint; }
		const TFloatPoint	getWA() const				{ return TFloatPoint (fAngle, fLineWidth); }


		virtual void	print(std::ostream& out) const;
		virtual void	accept (Updater*);
		enum ArrowHeadType getArrowLeft() const { return fArrowLeft; }
		enum ArrowHeadType getArrowRight() const { return fArrowRight; }
		double getArrowSizeLeft() const { return fArrowSizeLeft; }
		double getArrowSizeRight() const { return fArrowSizeRight; }

        virtual bool date2FramePoint(const libmapping::rational& date, TFloatPoint& p) const;

	protected:
				 ILine( const std::string& name, IObject* parent );
		virtual ~ILine() {}

		virtual void setHandlers ();

		/// \brief the 'get' form without parameter
		virtual SIMessageList getSetMsg() const;

		void	setPoint(const TFloatPoint& p);
		/// \brief the \c 'set' message handler
		virtual MsgHandler::msgStatus set (const IMessage* msg);

		MsgHandler::msgStatus setArrowsMsg(const IMessage* msg);
		std::pair<std::string, std::string> getArrows() const;
};

/*! @} */

} // end namespoace

#endif
