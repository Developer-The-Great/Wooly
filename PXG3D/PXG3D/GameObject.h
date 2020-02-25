#pragma once
#include "Transform.h"
#include "Vector3.h"
#include <memory>
#include <vector>
#include "PhysicsEngine.h"
#include "RenderingEngine.h"
#include "Component.h"
#include <typeindex>


namespace PXG
{
	class MeshComponent;
	class PhysicsComponent;
	class GameObject : public std::enable_shared_from_this< GameObject>
	{
		typedef std::shared_ptr<GameObject> GOSharedPtr;
		typedef std::weak_ptr<GameObject> GOWeakPtr;

	public:

		GameObject();

		~GameObject();

		void InitializeComponentOwners();

		virtual void Start();

		virtual void Update();

		virtual void FixedUpdate(float tick);

		virtual void AddToChildren(GOSharedPtr gameObj);

		template <class T,
			typename = std::enable_if_t<std::is_base_of_v<Component, T>>>
		void AddComponent(std::shared_ptr<T> component)
		{
			components.push_back(component);
			componentTable.insert(componentTable.begin(),
				{ typeid(T),std::static_pointer_cast<Component>(component) }
			);
			component->SetOwner(shared_from_this());
		}



		template <class T,
				 typename = std::enable_if_t<std::is_base_of_v<Component,T>>>
		std::shared_ptr<T> GetComponent(std::size_t offs = 0)
		{
			if(componentTable.find(typeid(T)) != componentTable.end())
			{
				auto [start, end] = componentTable.equal_range(typeid(T));

				if (offs > std::distance(start, end))
				{
					throw std::out_of_range("component subscript out of range!");
				}
				std::advance(start, offs);
				return std::reinterpret_pointer_cast<T>(start->second);

			}
			return nullptr;
		}

		void SetParent(GOSharedPtr gameObj);

		void SetLocalPosition(Vector3 newPosition);

		int GetImmediateChildrenCount() const;

		std::shared_ptr<MeshComponent> GetMeshComponent() const;
		std::shared_ptr<PhysicsComponent> GetPhysicsComponent() const;

		//void SetWorld

		std::vector<std::shared_ptr<GameObject>> GetChildren() const;

		std::weak_ptr<World> GetWorld() const;
		void SetWorld(std::shared_ptr<World> world);

		Transform* GetTransform() ;

		//TODO make this private, and make a setter for it
		std::string name = "";

	protected:



		std::unordered_multimap<std::type_index, std::shared_ptr<Component>> componentTable;
		std::vector< std::shared_ptr<Component>> components;
		std::vector<std::shared_ptr<GameObject>> children;

		std::weak_ptr<GameObject> parent;

		Transform transform;


		std::shared_ptr<MeshComponent> meshComponent = nullptr;
		std::shared_ptr<PhysicsComponent> physicsComponent = nullptr;
		std::weak_ptr<World> world;

	};
}


