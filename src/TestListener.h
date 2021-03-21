#pragma once
#include "Listener.h"

namespace StarBangBang
{
	/// <summary>
	/// Inherit (publicly) from Listener abstract class (interface) and implement onNotify()
	/// MessageBus (Event system) will call onNotify when a new message is received
	/// See DetectionListener.h for an example using ListenerComponent instead.
	/// </summary>
	class TestListener : public Listener
	{
		void onNotify(Event e);

	};
}