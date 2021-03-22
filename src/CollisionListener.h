#pragma once

#include "ListenerComponent.h"

namespace StarBangBang
{
	class CollisionListener : public ListenerComponent
	{
	public:
		CollisionListener(GameObject* obj);
		void onNotify(Event e);
	};
}