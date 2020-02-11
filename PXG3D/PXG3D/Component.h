#pragma once
#include <memory>


namespace PXG
{

	class GameObject;
	class Transform;

	class Component 
	{
	public:

		virtual void Start() = 0;

		virtual void FixedUpdate(float tick) = 0;

		Component();

		virtual ~Component();

		virtual void SetOwner(std::shared_ptr<GameObject> owner);

		Transform* GetOwnerTransform();

		std::shared_ptr<GameObject> GetOwner();

	protected:

		std::weak_ptr<GameObject> owner;


	private:


		



	};
}



