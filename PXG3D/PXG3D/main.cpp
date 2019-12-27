#include <iostream>
#include "Mathf.h"

int main()
{
	std::cout << "PXG3D is running" << std::endl;

	//initialize world

	std::cout << Mathf::LookRot(Vector3(-0.8194589, 0.2674348, 0.5069179), Vector3(-0.5534723, -0.1395520, -0.8210930)).ToString() << std::endl;

	while (true)
	{
		//track current time

		//Input sub-system update

		//while physics tick not over

			//fixed update on all game objects in currently loaded world

			//physics sub-system update

			//animation update



		//frame update on all game objects in currently loaded world


		//late update on all game objects in currenty loaded world

		//rendering sub-system update



		
	}
}