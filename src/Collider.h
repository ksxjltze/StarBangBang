#pragma once

namespace TempTeam
{
	class Collider
	{
	public:
		virtual void OnEnter() = 0;
		virtual void OnStay() = 0;
		virtual void OnExit() = 0;
	};
}