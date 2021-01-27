#pragma once
#include "ScriptComponent.h"

namespace StarBangBang
{
	class SecondaryMovementController : public Script
	{
	public:
		SecondaryMovementController(GameObject* gameObject) : Script(gameObject) {};
		_Component* Clone(GameObject* obj, _Component*) { return new SecondaryMovementController(obj); }
		void Start();
		void Update();
	private:
		AEVec2 velocity { 0, 0 };
		AEVec2 targetPos { 0, 0 };
	};
}