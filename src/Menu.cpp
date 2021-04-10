/******************************************************************************/
/*!
\title		Captain Stealth
\file		Menu.cpp
\author 	Lee Jia Keat
\par    	email: l.jiakeat@digipen.edu
\date   	April 09, 2021
\brief		Menu class.
			Displays a menu overlay.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Menu.h"

StarBangBang::Menu::Menu(GameObject* gameObject, GraphicsManager& gfx) : Script(gameObject), gfxMgr{ gfx }
{

}

void StarBangBang::Menu::Start()
{
	SetStatus(false);
}

void StarBangBang::Menu::Draw()
{
	if (gameObject->active)
	{
		for (auto& btnObj : buttonList)
		{
			for (auto& component : btnObj->GetComponents())
			{
				component->Draw();
			}
		}
	}
}

/*!*************************************************************************
 * \brief 
 * Workaround hack for updating game objects when game is paused.
***************************************************************************/
void StarBangBang::Menu::ForceUpdate()
{
	if (!gameObject->active)
		return;

	for (auto& btnObj : buttonList)
	{
		for (auto& component : btnObj->GetComponents())
		{
			component->Update();
		}
	}
}

void StarBangBang::Menu::SetStatus(bool s)
{
	status = s;
	gameObject->active = status;
	for (auto& btnObj : buttonList)
	{
		btnObj->active = status;
	}
}
