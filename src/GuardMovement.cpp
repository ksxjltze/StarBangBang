#include "GuardMovement.h"
#include "Guard.h"
#include <iostream>

#include "BasicMeshShape.h"
#include "globals.h"

using namespace StarBangBang;

GuardMovement::GuardMovement(GameObject* gameObject)
	: Script(gameObject)
	, speed(GUARD::GUARD_SPEED)
	, nodeIndex(0)
	, lookForPath(false)
	, foundPath(false)
	, isMoving(false)
	, changedTargetPos(false)
{
	SetWaypoints();
	//std::cout << waypoints.size() << "\n";

	//std::string text = "Guard ID: " + std::to_string(gameObject->GetComponent<Guard>()->GetID()) + "\n";
	//PRINT(text.c_str());
	//targetPos = waypoints.front();
}

void GuardMovement::Start()
{
	rb = gameObject->GetComponent<RigidBody>();
	rb->drag = 0.2f;
}

void GuardMovement::Idle()
{
	//std::cout << "GUARD: IDLE" << "\n";

	// rotate between left, front, right, back sprites
	// ...

	isMoving = false;
}

void GuardMovement::Patrol()
{
	//std::cout << "GUARD: PATROL" << "\n";

	/*AEVec2 target = { 0, 0 }, dir = { 0, 0 };
	AEVec2 test = { 100, 10 };

	AEVec2Sub(&target, &test, &gameObject->transform.position);
	AEVec2Scale(&dir, &waypoints.front(), dt);
	AEVec2Normalize(&dir, &dir);
	AEVec2Add(&gameObject->transform.position, &gameObject->transform.position, &dir);*/

	//MoveTo(waypoints.front());

	if (!changedTargetPos)
	{
		targetPos = endPos;
	}
	else
	{
		targetPos = startPos;
	}

	if (!foundPath)
	{
		LookForPath(targetPos);
		return;
	}

	MoveAlongPath();
}

void GuardMovement::Distracted()
{
	//Distraction test
	gameObject->transform.scale = { 2, 2 };

	if (!foundPath)
		return;

	MoveAlongPath();
}

void GuardMovement::MoveAlongPath()
{
	if (turning)
		return;

	/*for (const A_Node* n : path)
	{
		DrawCircle(20.0f, n->nodePos);
	}*/

	if (nodeIndex < path.size())
	{
		isMoving = true;
		nextPos = path[nodeIndex]->nodePos;

		float timer = 0.f;
		if (MoveTo(path[nodeIndex]->nodePos))
		{
			timer += g_dt;
			if (timer >= 0.01f)
			{
				path[nodeIndex]->occupied = false;
				++nodeIndex;
				timer = 0.f;
			}
		}
	}
	else
	{
		// deactivate interactable object
		// ...

		reachedEndOfPath = true;
		path.clear();
		nodeIndex = 0;
		foundPath = false;
		isMoving = false;
		changedTargetPos = !changedTargetPos;
	}
}

bool GuardMovement::MoveTo(AEVec2 pos)
{
	if (ReachedPos(pos))
	{
		//gameObject->transform.position = pos; // this snapping to position causes the obj to vanish
		return true;
	}

	AEVec2 dir = { 0, 0 };

	AEVec2Sub(&dir, &pos, &gameObject->transform.position);
	AEVec2Normalize(&dir, &dir);
	AEVec2Scale(&dir, &dir, speed);

	rb->AddVelocity(dir, 1.f);

	return false;
}

bool GuardMovement::ReachedPos(AEVec2 pos)
{
	float minDistToTarget = 10.f;
	return (AEVec2SquareDistance(&pos, &gameObject->transform.position) <= minDistToTarget * minDistToTarget);
}

void GuardMovement::SetWaypoints()
{
	//std::cout << "GUARD: SETTING WAYPOINTS\n";
	// Test
	waypoints.push_back({ 100, 10 });


	// ***********************************

	// Read from file
	// ...

	// Set waypoints specfic to this guard
	// ...
}

bool GuardMovement::IsChangingDir()
{
	AEVec2 currNode, prevNode;
	
	prevNode = path[nodeIndex]->nodePos;
	size_t index = static_cast<size_t>(nodeIndex) + static_cast<size_t>(1);

	if (index < path.size())
		currNode = path[index]->nodePos;
	else
		currNode = prevNode;

	AEVec2Normalize(&currNode, &currNode);
	AEVec2Normalize(&prevNode, &prevNode);
	float dpResult = AEVec2DotProduct(&currNode, &prevNode);

	return (dpResult < 0.f);

	/*if ((int)currNode.x == (int)prevNode.x)
		if ((int)currNode.y != (int)prevNode.y)
			return true;

	if ((int)currNode.y == (int)prevNode.y)
		if ((int)currNode.x != (int)prevNode.x)
			return true;

	return false;*/
	//return (currNode.x != prevNode.x || currNode.y != prevNode.y);
}

void GuardMovement::LookForPath(const AEVec2& pos)
{
	foundPath = false;
	turning = false;
	lookForPath = true;

	if (lookForPath)
	{
		if (foundPath) // changing path midway into moving along the path
		{
			nodeIndex = 0;
		}

		path = PathFinder::SearchForPath(gameObject->transform.position, pos);
		foundPath = (path.size() > 0);
		lookForPath = false;
		reachedEndOfPath = false;
	}
}

void GuardMovement::SetStartEndPos(const AEVec2& start, const AEVec2& end)
{
	gameObject->SetPos(start);
	startPos = start;
	endPos = end;
}