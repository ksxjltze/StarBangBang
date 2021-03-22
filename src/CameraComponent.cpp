#include "CameraComponent.h"
#include "GraphicsManager.h"

StarBangBang::CameraComponent::CameraComponent(GameObject* gameObject) : Component<CameraComponent>(gameObject)
{
	position = { 0, 0 };
	scale = GRAPHICS::DEFAULT_ZOOM;
	scaleFactor = GRAPHICS::CAMERA_SCALE_FACTOR;
	target = gameObject;
}

void StarBangBang::CameraComponent::LateUpdate()
{
	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
	if (AEInputCheckCurr(AEVK_MINUS))
	{
		scale -= scaleFactor * dt;
		if (scale < GRAPHICS::CAMERA_SCALE_MIN)
			scale = GRAPHICS::CAMERA_SCALE_MIN;
	}
	else if (AEInputCheckCurr(AEVK_EQUAL))
	{
		scale += scaleFactor * dt;
		if (scale > GRAPHICS::CAMERA_SCALE_MAX)
			scale = GRAPHICS::CAMERA_SCALE_MAX;
	}

	//position = gameObject->transform.position;
	position = target->transform.position;
	AEVec2Scale(&position, &position, scale);
	GRAPHICS::SetZoom(scale);

	AEVec2 screenScale = GRAPHICS::GetScreenScale();
	position.x *= screenScale.x;
	position.y *= screenScale.y;


	//temp
	//AEGfxSetCamPosition(position.x, position.y);
	GRAPHICS::SetCameraPosition(position.x, position.y);
}

void StarBangBang::CameraComponent::SetTarget(GameObject* obj)
{
	target = obj;
}

StarBangBang::GameObject* StarBangBang::CameraComponent::GetTarget()
{
	return target;
}
