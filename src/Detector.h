#pragma once
#include "ScriptComponent.h"
#include "Color.h"
#include "PlayerScript.h"

namespace StarBangBang
{
	class BoxCollider;

	class Detector : public Script
	{
	public:
		Detector(GameObject* gameObject);

		void Start();
		void Update();

		void Init(float fov, float dist, GameObject* _player, GameObject* client);
		void Rotate(float angle);

		void SpanVision(float minRot, float maxRot, float speed);

		inline void SetAngle(float angle) { rotationAngle = angle; }
		inline void SetFacingDir(const AEVec2& dir) { targetDir = dir; }
		inline float GetRotation() const { return rotationAngle; }

		bool GetDetected(AEVec2& targetPos) const;
		
		inline const Color& GetColor() const { return color; }

	private:
		void CheckForTargets(const AEVec2& _targetPos, bool checkForPlayer = true);
		void SetDetected(bool checkForPlayer, bool detected);

		float fieldOfView;
		float viewDist;
		float rotationAngle;

		bool atMaxRot;

		bool detectedTarget1, detectedTarget2;

		AEVec2 defaultForward;
		AEVec2 targetDir;

		GameObject* target1, *target2;
		BoxCollider* target1Col, *target2Col;
		BoxCollider* myCollider;

		PlayerScript* player{ nullptr };

		Color color;
	};
}