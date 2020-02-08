#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"

typedef std::shared_ptr<PXG::GameObject> GameObj;

namespace PXG
{
	class World;

	class Game
	{


	public:

		Game();



		virtual void Start() = 0;

		virtual void Update() = 0;

		virtual void FixedUpdate(float tick) = 0;

		std::shared_ptr < World>  GetWorld() const;

		void SetCamera();

		virtual void Initialize();

		GameObj Instantiate();




	protected:


		std::shared_ptr < World> world;
		

	};


}
