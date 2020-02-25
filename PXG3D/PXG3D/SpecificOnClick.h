#pragma once
#include "AbstractOnClick.h"
#include "Debug.h"
namespace PXG
{
	class SpecificOnClick : public AbstractOnClick
	{
	public:
		~SpecificOnClick();
		virtual void Execute()override;
	};
}