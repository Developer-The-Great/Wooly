#pragma once
#include "Transform.h"
#include "Vector3.h"
#include <memory>
#include <vector>
#include "core/PhysicsEngine.h"
#include "core/RenderingEngine.h"
#include "Component.h"

class MeshComponent;
class PhysicsComponent;


namespace PXG
{

	class GameObject : public std::enable_shared_from_this< GameObject>
	{
		typedef std::shared_ptr<GameObject> GOSharedPtr;
		typedef std::weak_ptr<GameObject> GOWeakPtr;

	public:

		GameObject();

		~GameObject();

		virtual void Start();

		virtual void Update();

		virtual void FixedUpdate(float tick);

		virtual void AddToChildren(GOSharedPtr gameObj);

		void AddComponent(std::shared_ptr<Component> component);

		void SetParent(GOSharedPtr gameObj);

		void SetLocalPosition(Vector3 newPosition);

		int GetImmediateChildrenCount();

		std::shared_ptr<MeshComponent> GetMeshComponent();
		std::shared_ptr<PhysicsComponent> GetPhysicsComponent();

		//void SetWorld

		std::vector<std::shared_ptr<GameObject>> GetChildren();

		std::weak_ptr<World> GetWorld();
		void SetWorld(std::shared_ptr<World> world);

		Transform* GetTransform() ;

		//TODO make this private, and make a setter for it
		std::string name = "";

	protected:

		std::vector< std::shared_ptr<Component>> components;
		
		std::vector<std::shared_ptr<GameObject>> children;

		std::weak_ptr<GameObject> parent;

		Transform transform;


		std::shared_ptr<MeshComponent> meshComponent = nullptr;
		std::shared_ptr<PhysicsComponent> physicsComponent = nullptr;
		std::weak_ptr<World> world;

	};
}


