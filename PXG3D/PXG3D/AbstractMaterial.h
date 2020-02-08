#pragma once
#include <memory>

namespace PXG
{
	class World;
	struct Mat4;

	class AbstractMaterial
	{

	public:
		AbstractMaterial();
		virtual ~AbstractMaterial();

		virtual void SendUniforms(std::weak_ptr<World> _world,Mat4 model,Mat4 view,Mat4 projection) = 0;


	private:

	};


}
