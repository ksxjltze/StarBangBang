#include "CameraComponent.h"

StarBangBang::CameraComponent::CameraComponent(GameObject* gameObject) : Component<CameraComponent>(gameObject)
{
	position = { 0, 0 };
}

void StarBangBang::CameraComponent::Update()
{
	position = gameObject->transform.position;

	//temp
	AEGfxSetCamPosition(position.x, position.y);
}
