#include "Player.h"

StarBangBang::Player::Player(GameObject* gameObject) : Script(gameObject)
{

}

void StarBangBang::Player::Load()
{

}

void StarBangBang::Player::Start()
{
	movementController.Start();
	secondaryMovementController.Start();
}

void StarBangBang::Player::Update()
{
	movementController.Update();
	secondaryMovementController.Update();
}
