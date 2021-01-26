#pragma once
#include "ScriptComponent.h"

namespace StarBangBang
{
	class SecondaryMovementController : public Script
	{
	public:
		SecondaryMovementController(GameObject* gameObject) : Script(gameObject) {};
		Component* Clone(Component* component) { return new SecondaryMovementController(component->gameObject); };
		void Start();
		void Update();
	private:
		AEVec2 velocity { 0, 0 };
		AEVec2 targetPos { 0, 0 };
	};
}