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


#ifndef __TFile__
#define __TFile__

#include <string>
#include <ostream>

#include "IObject.h"
#include "IMessage.h"

namespace inscore
{

/*!
\addtogroup ITLModel
@{
*/

class IScene;
//--------------------------------------------------------------------------
/*!
	\brief a base class for file based elements of the model
*/
class TFile
{
	std::string	fFile;
	std::string	fFilePath;
	bool		fPathChanged;
	IScene*		fScene;

	char *		fData;				// a pointer to the file data, actually used only by url based files
	int			fDataSize;			// but it should be generalized
	
	public:
		/// \brief returns the path of the object file
		const std::string&	getFile() const						{ return fFile; }
		const std::string&	getPath() const						{ return fFilePath; }

		/// \brief set the path name \param path the new file path
		virtual void		setFile(const std::string& path)	{ fFilePath = path; fPathChanged = true; }

		/*!
			\brief reads the associated file 
			\param buff on output, the file content as a string
			\return true on success
		*/
		virtual bool		read(std::string& buff) const;
		virtual bool		read(std::ostream& out) const;

		virtual const char* data() const			{ return fData; }
		virtual int			dataSize() const		{ return fDataSize; }

		virtual bool		changed() const				{ return fPathChanged; }
		virtual void		changed(bool state)			{ fPathChanged = state; }

		virtual void		print(std::ostream& out) const;
        virtual bool        hasData() const				{return fData != 0;}

		virtual void        updateUrl()		{}
        virtual void		setData(const char* data, int size);

	protected:
				 TFile(IScene* scene, const std::string& pathname = "" );
		virtual ~TFile() { delete fData; }

		/// \brief the \c 'set' message handler
		virtual MsgHandler::msgStatus set (const IMessage* msg);
    
		static char *	readfile(const std::string& path);
		static int		getLength (std::ifstream& f);
};

/*! @} */

} // end namespoace

#endif
