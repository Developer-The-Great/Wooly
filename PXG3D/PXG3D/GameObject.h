#pragma once
#include "Transform.h"
#include "Vector3.h"
#include <memory>
#include <vector>
#include "PhysicsEngine.h"
#include "RenderingEngine.h"

class MeshComponent;
class PhysicsComponent;


namespace PXG
{

	


	class GameObject
	{
		typedef std::shared_ptr<GameObject> GOSharedPtr;
		typedef std::weak_ptr<GameObject> GOWeakPtr;

	public:

		GameObject();

		~GameObject();

		virtual void Start();

		virtual void Update();

		virtual void FixedUpdate(float tick);

		void AddToChildren(GOSharedPtr gameObj);

		void SetParent(GOWeakPtr gameObj);

		void SetPosition(Vector3 newPosition);

		void AddToPhysicsEngine(std::shared_ptr<PhysicsEngine> physicsEngine);

		void AddToRenderingEngine(std::shared_ptr<RenderingEngine> renderingEngine);

	private:

		std::vector<std::shared_ptr<GameObject>> children;
		std::weak_ptr<GameObject> parent;

		Transform transform;

		std::shared_ptr<MeshComponent> meshComponent = nullptr;
		std::shared_ptr<PhysicsComponent> physicsComponent = nullptr;


	};
}


