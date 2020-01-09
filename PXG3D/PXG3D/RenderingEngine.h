#pragma once
#include <memory>
#include "MeshComponent.h"


namespace PXG
{
	class RenderingEngine
	{
	public:

		void AddRenderingComponent(std::shared_ptr<MeshComponent> meshComponent);

	private:

	};
}


