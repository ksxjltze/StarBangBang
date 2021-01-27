#include "DragComponent.h"
#include "Utils.h"
#include <iostream>

StarBangBang::DragComponent::DragComponent(GameObject* gameObject) : _Component(gameObject)
{
	enabled = false;
}

StarBangBang::_Component* StarBangBang::DragComponent::Clone(GameObject* obj, _Component*)
{
	return new DragComponent(obj);
}

void StarBangBang::DragComponent::Update()
{
	AEVec2 mousePos = GetMouseWorldPos();
	float mouseX = mousePos.x, mouseY = mousePos.y;
	AEVec2 pos = gameObject->GetPos();

	//Should probably change
	float width = gameObject->width, height = gameObject->height;

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		if (mouseX > pos.x - width / 2
			&& mouseX < pos.x + width / 2
			&& mouseY > pos.y - height / 2
			&& mouseY < pos.y + height / 2)
		{
			enabled = true;
		}

	}
	else if (AEInputCheckReleased(AEVK_LBUTTON))
	{
		enabled = false;
	}

	if (enabled)
	{
		pos.x = mouseX;
		pos.y = mouseY;
		gameObject->SetPos(pos);
	}
}
