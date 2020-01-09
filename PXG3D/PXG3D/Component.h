#pragma once



namespace PXG
{
	class GameObject;

	class Component
	{
	public:

		Component(GameObject * owner);



	private:


		GameObject * owner;



	};
}



