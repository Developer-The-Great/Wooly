#pragma once
#include "Component.h"
#include "GameObject.h"
#include <array>
#include "Tile.h"
#include "TileMetaData.h"

namespace PXG
{
	class SurroundingObjectEnumeratorComponent : public Component
	{
	public:
		void Start() override{}
		void FixedUpdate(float tick) override{}


		std::array<std::shared_ptr<GameObject>,6> Enumerate(GameObject* map)
		{

			std::array<std::shared_ptr<GameObject>, 6> accumulator;
			auto i = accumulator.begin();
			
			for(const auto& child : map->GetChildren())
			{
				if (i == accumulator.end())
				{
					break;
				}
				
				auto* tc = child->GetTransform();
				auto delta = tc->GetPosition() - GetOwnerTransform()->GetPosition();

				const float epsilon = Tile::SIZE / 20.0f;

				if(delta.Length() < Tile::SIZE + epsilon && delta.Length() > Tile::SIZE - epsilon)
				{
					if(child->GetComponent<TileMetaData>() != nullptr)
					{
						
						*i = child;
						++i;
						
					}
				}	
			}

			return accumulator;
		}

		
	};
}
