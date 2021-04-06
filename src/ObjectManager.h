#pragma once
#include "GraphicsManager.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "CollisionManager.h"
#include "Sprite.h"
#include <vector>
#include <typeinfo>
#include <map>

namespace StarBangBang
{
	class ObjectManager
	{
	public:
		template <class T, class ...Args>
		T& AddComponent(GameObject* gameObject, Args&&... args)
		{
			T* component = new T(gameObject, args...);
			AddComponent(gameObject, component);
			component->objMgr = this;
			return *component;
		}

		template <class T, class U>
		T& AddComponent(GameObject* gameObject)
		{
			T* component = new T<U>(gameObject);
			AddComponent(gameObject, component);
			component->objMgr = this;
			return *component;
		}

		GameObject* Find(const std::string& name);

		BoxCollider& AddCollider(GameObject* gameObject, bool isStatic);

		void AddComponent(GameObject* gameObject, _Component* component);
		StarBangBang::ImageComponent* AddImage(GameObject* gameObject, Sprite sprite);
		void AddChildGameObject(GameObject* child, GameObject* parent);

		GameObject* NewGameObject();
		GameObject* NewGameObject(GameObject* parent);

		GameObject* CloneGameObject(GameObject* gameObject);
		void DestroyGameObject(GameObject* gameObject);
		void SaveGameObject(GameObject& gameObject); //Serialize
		GameObject& ReadGameObject(const char* path); //Serialize

		void FreeObjects();
		void FreeComponents();
		void Init();
		void Draw();
		void Update();
		void LateUpdate();
	private:
		friend class Scene;
		std::vector<GameObject*> gameObjectList;
		std::vector<_Component*> componentList;

		std::map<int, std::vector<_Component*>> layerMap;
	};
}