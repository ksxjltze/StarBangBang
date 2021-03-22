#include "GuardManager.h"
#include "ObjectManager.h"
#include "Sprite.h"
#include "Guard.h"
#include "Detector.h"
#include "SecurityCamera.h"
#include "Physics.h"

#include "Utils.h" // for mouseworldpos

#include "Text.h"
#include "globals.h"

using namespace StarBangBang;

GuardManager::GuardManager(GameObject* gameObject) 
	: Script(gameObject)
{

}

void GuardManager::Init(ObjectManager* objManager, Sprite* sprite, GameObject* player, GameObject* client)
{
	int id = 0;

	for (size_t i = 0; i < NUM_GUARDS; i++)
	{
		guards.push_back(objManager->NewGameObject());
		//guards[i]->SetPos({ 250, 650 });
		guards[i]->transform.scale = {0.7f, 0.7f};
		objManager->AddImage(guards[i], *sprite);
		objManager->AddComponent<Guard>(guards[i]);
		objManager->AddComponent<GuardMovement>(guards[i]);
		objManager->AddComponent<GuardVision>(guards[i]).SetPlayerAndClient(player, client);
		objManager->AddComponent<Detector>(guards[i]);
		objManager->AddComponent<Text>(guards[i]).fontID = StarBangBang::fontId;
		objManager->AddComponent<RigidBody>(guards[i]);
	}

	// temp, will change to read from file laterz
	SetGuardWaypoints(id++, { -300, -1085 }, { -420, -1085 });
	SetGuardWaypoints(id++, { -1144, -942}, { -850, -942 });
	SetGuardWaypoints(id++, { -752, -746 }, { -752, -590 });
	SetGuardWaypoints(id++, { -1020, -230 }, { -1020, -670 });
	SetGuardWaypoints(id++, { -750, 100 }, { -750, -90 });
	SetGuardWaypoints(id++, { -180, -50 }, { -20, 120 });
	SetGuardWaypoints(id++, { 350, -250 }, { -195, -250 });
	SetGuardWaypoints(id++, { 455, 190 }, { 630, -415 });
	SetGuardWaypoints(id++, { 160, -1125 }, { 370, -980 });
	SetGuardWaypoints(id++, { 795, -530 }, { 795, -530 }, true); // IDLE
	SetGuardWaypoints(id++, { 930, 830 }, { 0, 515 });
	SetGuardWaypoints(id++, { -195, 790 }, { -815, 540 });
	SetGuardWaypoints(id++, { -1120, 420 }, { -880, 880 });
}

void GuardManager::CreateSecurityCameras(ObjectManager* objManager, Sprite* sprite, GameObject* player, GameObject* client)
{
	int id = 0;

	for (size_t i = 0; i < NUM_CAMERAS; i++)
	{
		cameras.push_back(objManager->NewGameObject());
		cameras[i]->transform.scale = { 0.7f, 0.7f };
		objManager->AddImage(cameras[i], *sprite);
		objManager->AddComponent<SecurityCamera>(cameras[i]);
		objManager->AddComponent<Detector>(cameras[i]).Init(50.f, 100.f, player, client);
	}

	SetCameraPosAndViewMinMax(id++, {-1030, -790},  -250.f, -150.f);
	SetCameraPosAndViewMinMax(id++, { -250, -520 }, -270.f, -130.f);
	SetCameraPosAndViewMinMax(id++, { -620, 190 }, -200.f, -80.f);
	SetCameraPosAndViewMinMax(id++, { 470, -480 }, 0.f, 90.f);
	SetCameraPosAndViewMinMax(id++, { -645, -410 }, -90.f, 0.f);
	SetCameraPosAndViewMinMax(id++, { 1020, 350 }, 0.f, 90.f);
	SetCameraPosAndViewMinMax(id++, { -180, 340 }, 0.f, 90.f);
	SetCameraPosAndViewMinMax(id++, { -13, 1165 }, -180.f, -90.f);
	SetCameraPosAndViewMinMax(id++, { -1160, 1130 }, -180.f, -90.f);
	SetCameraPosAndViewMinMax(id++, { -690, 720 }, 0.f, 360.f);
}

void GuardManager::Update()
{
	PRINT("x: %f, y: %f\n", GetMouseWorldPos().x, GetMouseWorldPos().y);

	// upon receiving distraction event, get nearest guard to be distracted
	// ...

	if (AEInputCheckTriggered(VK_LBUTTON))
	{
		//guards[0]->GetComponent<GuardMovement>()->SetEndPos(GetMouseWorldPos());
		guards[0]->GetComponent<Guard>()->SetState(Guard::GUARD_STATE::STATE_PATROL);
	}

	if (AEInputCheckTriggered(VK_RBUTTON))
	{
		//guards[1]->GetComponent<GuardMovement>()->LookForPath(GetMouseWorldPos());
		guards[0]->GetComponent<Guard>()->SetState(Guard::GUARD_STATE::STATE_IDLE);
	}
}

GameObject* GuardManager::GetNearestGuard(AEVec2& _pos)
{
	float minDist = AEVec2SquareDistance(&_pos, &guards[0]->transform.position);
	GameObject* nearestGuard = guards[0];

	for (size_t i = 1; i < NUM_GUARDS; i++)
	{
		float dist = AEVec2SquareDistance(&_pos, &guards[i]->transform.position);

		if (dist < minDist)
		{
			minDist = dist;
			nearestGuard = guards[i];
		}
	}

	return nearestGuard;
}

void GuardManager::SetGuardWaypoints(int id, const AEVec2& start, const AEVec2& end, bool isIdle)
{
	guards[id]->GetComponent<GuardMovement>()->SetStartEndPos(start, end);

	if (isIdle)
		guards[id]->GetComponent<Guard>()->SetState(Guard::GUARD_STATE::STATE_IDLE);
}

void GuardManager::SetCameraPosAndViewMinMax(int id, const AEVec2& pos, float min, float max)
{
	cameras[id]->SetPos(pos);
	cameras[id]->GetComponent<SecurityCamera>()->SetRotationMinMax(min, max);
}