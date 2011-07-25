/*

  INScore Project

  Copyright (C) 2011  Grame

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


#ifndef __TLoop__
#define __TLoop__

#include <vector>
#include <string>

#include "smartpointer.h"
#include "TLoopEval.h"

namespace inscore 
{

class TLoop;
typedef SMARTP<TLoop> 	STLoop;
class TLoopEval;

//--------------------------------------------------------------------------------------------
/*!
\brief A base type for loop elements.
*/
class TLoopElement : public smartable { 
	public: 
		virtual ~TLoopElement() {} 
		virtual void accept (TLoopEval* visitor) = 0;
};
typedef SMARTP<TLoopElement> STLoopElement;

class LoopedMessage : public TLoopElement 
{
	IMessage* fMessage;
	public:
				 LoopedMessage (IMessage* msg) : fMessage(msg) {}
		virtual ~LoopedMessage();
		virtual void accept (TLoopEval* visitor);
};

class LoopedLoop : public TLoopElement 
{
	STLoop fLoop;
	public:
				 LoopedLoop (STLoop loop) : fLoop(loop) {}
		virtual ~LoopedLoop() {}
		virtual void accept (TLoopEval* visitor);
};


//--------------------------------------------------------------------------------------------
/*!
\brief A loop representation
*/
class TLoop : public smartable
{
	private:
		std::string		fIdent;
		unsigned int	fCount;
		std::vector<STLoopElement>	fLooped;
		
    protected:
				 TLoop(const std::string& id, unsigned int count) : fIdent(id), fCount(count) {}
		virtual ~TLoop() {}

	public:
        static STLoop create(const std::string& id, unsigned int count)	{ return new TLoop(id, count); }
        void add(IMessage* msg)										{ fLooped.push_back (new LoopedMessage(msg)); }
        void add(STLoop loop)										{ fLooped.push_back (new LoopedLoop(loop)); }
		void eval(TEnv* env, IMessageList* outlist);
};

} // namespace

#endif
