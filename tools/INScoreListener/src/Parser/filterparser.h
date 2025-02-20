/*
  Interlude Library
  Copyright (C) 2009,2011  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __ITLparser__
#define __ITLparser__

#include <string>
#include <istream>
#include <locale.h>

#include "../OSCFilter.h"


namespace inscorelistener
{

/* \brief a class for reading ITL streams
*/
class filterparser {
		
	void initScanner();
	void destroyScanner();
	
	public:
		void*			fScanner;	// the flex scanner
		std::istream*	fStream;    // input stream
		std::string		fText;		// the current text
		int				fInt;		// the current int
		float			fFloat;		// the current float
		OSCFilterNode*		fFilterRoot;



		filterparser(std::string string);
		~filterparser();
		
		inscorelistener::OSCFilter *parse();
};

} // end namespace

#endif
