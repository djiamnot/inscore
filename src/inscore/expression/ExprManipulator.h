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
#ifndef EXPRMANIPULATOR_H
#define EXPRMANIPULATOR_H

#include <string>
#include <unordered_set>
#include <stack>
#include <map>
#include <utility>

#include "IExpression.h"


namespace inscore {

class IObject;
typedef std::unordered_set<const IObject*> WatchersList;
typedef std::map<std::string, SIExprArg> ReplaceMap;


//____________________________________________________________________
// ------------------------------------------------------------------
/*!
 * \brief ExprCompositor are in charge of expression's expansion before the first evaluation (every arguments prefixed with ~ shall be expanded)
 */
class ExprCompositor: public evaluator{
	const IObject* fContextObject;
	ReplaceMap fReplacedNodes;

	bool succeed;

public:

	static bool compose(SIExprArg& expr, const IObject* contextObject);


	bool apply(SIExprArg& exprTree);

	virtual const std::string eval(IExprOperator *arg,		IExprArgBase *exprArg);
	virtual const std::string eval(const std::string&,		IExprArgBase*){return "";}
	virtual const std::string eval(const filepath&,			IExprArgBase*){return "";}
	virtual const std::string eval(const itladdress &arg,	IExprArgBase *exprArg);
	virtual const std::string eval(const IObject* arg,		IExprArgBase *exprArg);
	virtual const std::string eval(const iexpression &,		IExprArgBase *){return "";}

protected:
	ExprCompositor(const IObject* contextObject, const char* manipulatorName="ExprManipulator");
	std::string replaceBy(SIExprArg newNode, std::string key="");
	void continueExploration(IExprOperator* arg, IExprArgBase *exprArg);

	inline void manipulationFailed(){succeed = false;}

	const char* fManipulatorName;
private:
	inline void replaceNode(SIExprArg &node, std::string key);

};


//____________________________________________________________________
// ------------------------------------------------------------------
/*!
 * \brief ExprSimplificator are in charge of expression's simplification after the first evaluation
 * (every static branch are simplified into IExprArg<iexpression> containing the value computed during the first evaluation and a string definition of the branch)
 */
class ExprSimplificator: public evaluator{

	SIExprArg fSimplifiedNode;

public:
	static void simplify(SIExpression &expression);
	static void simplify(SIExprArg &rootNode);

	virtual const std::string eval(IExprOperator *arg,	IExprArgBase *exprArg);
	virtual const std::string eval(const std::string &,	IExprArgBase *){return "";}
	virtual const std::string eval(const filepath &,	IExprArgBase *){return "";}
	virtual const std::string eval(const itladdress &,	IExprArgBase *){return "";}
	virtual const std::string eval(const iexpression &,	IExprArgBase *){return "";}

protected:
	ExprSimplificator(): evaluator() {}

	bool simplifyNode(SIExprArg &node);

};


//____________________________________________________________________
// ------------------------------------------------------------------
/*!
 * \brief ExprSmartCopy allows to make a copy of an expression tree keeping any memory-shared link between similar branches
 * (when an expression is expanded using an object defintion more than once the expanded branch are memory-shared).
 */
class ExprSmartCopy: public constEvaluator{

	std::map<const IExprArgBase*, SIExprArg> fCopyMap;
	SIExprArg fCurrentNode;

public:
	static SIExpression copy(const SIExpression expression);
	static SIExprArg copy(const SIExprArg rootNode);

	virtual const std::string eval(const IExprOperator *,	const IExprArgBase *exprArg);
	virtual const std::string eval(const std::string &,		const IExprArgBase *exprArg);
	virtual const std::string eval(const filepath &,		const IExprArgBase *exprArg);
	virtual const std::string eval(const itladdress &,		const IExprArgBase *exprArg);
	virtual const std::string eval(const iexpression &,		const IExprArgBase *exprArg);

protected:
	ExprSmartCopy(): constEvaluator() {}
	SIExprArg fCopyTree;

	SIExprArg smartCopy(const SIExprArg node);

};


}//End namespace


#endif // EXPRMANIPULATOR_H
