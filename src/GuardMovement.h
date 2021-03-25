#pragma once
#include "ScriptComponent.h"
#include "PathFinder.h"
#include "Physics.h"
#include <vector>
#include "Listener.h"

namespace StarBangBang
{
	class GuardMovement : public Script
	{
	public:
		GuardMovement(GameObject* gameObject);

		void Start();
		void Update() {}

		void Idle();
		void Patrol();
		void Distracted();

		void LookForPath(const AEVec2& pos);
		void SetStartEndPos(const AEVec2& start, const AEVec2& end);

		inline const AEVec2& GetNextPos() const { return nextPos; }
		inline bool IsMoving() const { return isMoving; }
		inline void SetSpeed(float _speed) { speed = _speed; }
		bool IsChangingDir();

	private:
		void MoveAlongPath();
		bool MoveTo(AEVec2 pos);
		bool ReachedPos(AEVec2 pos);
		void SetWaypoints();

		bool isMoving;
		bool lookForPath;
		bool foundPath;
		bool reachedEndOfPath;
		bool changedTargetPos;

		float speed;

		unsigned int nodeIndex;

		AEVec2 targetPos;
		AEVec2 startPos;
		AEVec2 endPos;

		AEVec2 nextPos;

		// Waypoints specific to this guard
		std::vector<AEVec2> waypoints; // to use in case pathfinding causes fps drops

		std::vector<A_Node*> path;

		RigidBody* rb;
	};
}