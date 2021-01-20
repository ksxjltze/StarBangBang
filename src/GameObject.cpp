#include "GameObject.h"

StarBangBang::GameObject::GameObject()
{
	active = true;
	transform.position = { 0, 0 };
	transform.rotation = 0;
	transform.scale = 1;
}

StarBangBang::GameObject::GameObject(const GameObject& prefab)
{
	transform = prefab.transform;
	parent = prefab.parent;
	width = prefab.width;
	height = prefab.height;
	active = prefab.active;
}

AEVec2 StarBangBang::GameObject::GetPos()
{
	if (parent != nullptr)
	{
		AEVec2 pos;
		AEVec2Add(&pos, &parent->transform.position, &transform.position);
		return pos;
	}
	return transform.position;
}

void StarBangBang::GameObject::SetPos(AEVec2 newPos)
{
	{
		if (parent != nullptr)
		{
			AEVec2 pos;
			AEVec2Sub(&pos, &newPos, &parent->transform.position);
			transform.position = pos;
		}
		else
			transform.position = newPos;
	}
}