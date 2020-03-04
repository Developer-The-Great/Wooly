#include "SpecificOnClick.h"

namespace PXG
{
	SpecificOnClick::~SpecificOnClick()
	{
	}
	void SpecificOnClick::Execute()
	{
		for (auto target : targetDisable)
		{
			target->GetMeshComponent()->DisableRender(true);
			for (auto component : target->GetComponents())
			{
				component->SetActive(false);
			}
		}

		for (auto target : targetEnable)
		{
			target->GetMeshComponent()->DisableRender(false);
			for (auto component : target->GetComponents())
			{
				component->SetActive(true);
			}
		}
		if(raycaster)
		{
			raycaster->SetActive(true);
		}
		Debug::Log("button executed");

	}

}
