#include "GameObject.h"
#include "Debug.h"
#include "MeshComponent.h"
#include "PhysicsComponent.h"


namespace PXG
{
	GameObject::GameObject()
	{
		physicsComponent = std::make_shared<PhysicsComponent>();
		meshComponent = std::make_shared<MeshComponent>();
	}
	GameObject::~GameObject()
	{
		Debug::Log(Verbosity::Info, "GameObject Destroyed");
	}
	void GameObject::Start()
	{

	}
	void GameObject::Update()
	{

	}

	void GameObject::FixedUpdate(float tick)
	{

	}

	void GameObject::AddToChildren(GOSharedPtr gameObj)
	{
		children.push_back(gameObj);
	}

	void GameObject::SetParent(GOWeakPtr gameObj)
	{
		parent = gameObj;
	}

	void GameObject::SetPosition(Vector3 newPosition)
	{
	}

	void GameObject::AddToPhysicsEngine(std::shared_ptr<PhysicsEngine> physicsEngine)
	{
		physicsEngine->AddPhysicsComponent(physicsComponent);
	}

	void GameObject::AddToRenderingEngine(std::shared_ptr<RenderingEngine> renderingEngine)
	{
		renderingEngine->AddRenderingComponent(meshComponent);
	}

}
