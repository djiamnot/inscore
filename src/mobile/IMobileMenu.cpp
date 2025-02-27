/*

  INScore Project

  Copyright (C) 2015  Grame

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

  Grame Research Laboratory, 11 cours Verdun Gensoul, 69002 Lyon - France
  research@grame.fr

*/

#include "INScore.h"

#include "IMobileMenu.h"
#include "VMobileMenu.h"
#include "VMobileQtInit.h"


namespace inscore {

//--------------------------------------------------------------------------
IMobileMenu::~IMobileMenu()
{
	VMobileQtInit::getMainPanel()->removePanel(fMobileMenu->windowTitle(),false);
	delete fMobileMenu;
}

QWidget* IMobileMenu::window()
{
	return fMobileMenu;
}

SIMenu IMobileMenu::create(const INScoreApplicationGlue* glue, IObject * parent)
{
	IMobileMenu * menu = new IMobileMenu(parent);
	// Create the view of the menu object
	menu->setWindow(new VMobileMenu("About", glue->viewVersion().c_str(), glue->getIP().c_str()));
	return menu;
}

}
