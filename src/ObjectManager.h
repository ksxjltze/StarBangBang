#pragma once
#include "GraphicsManager.h"
#include "GameObject.h"
#include "ScriptManager.h"
#include "ImageComponent.h"
#include "DragComponent.h"
#include "Sprite.h"
#include <vector>
#include <typeinfo>

namespace StarBangBang
{
	class ObjectManager
	{
	public:
		template <class T>
		inline void AddComponent(GameObject* gameObject)
		{
			T* component = new T(gameObject);
			AddComponent(gameObject, component);
		}

		void AddComponent(GameObject* gameObject, _Component* component);
		void AddImage(GameObject* gameObject, Sprite sprite);
		void AddChildGameObject(GameObject* child, GameObject* parent);
		GameObject* NewGameObject();
		GameObject* NewGameObject(float width, float height);
		GameObject* CloneGameObject(GameObject* gameObject);
		void DestroyGameObject(GameObject* gameObject);

		void FreeObjects();
		void FreeComponents();
		void Init();
		void Draw();
		void Update();
	private:
		friend class Scene;
		std::vector<GameObject*> gameObjectList;
		std::vector<ImageComponent*> imageComponentList;
		std::vector<_Component*> componentList;
	};
}