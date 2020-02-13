#pragma once 
#include "SpecificOnClick.h"

namespace PXG
{
	SpecificOnClick::SpecificOnClick() : AbstractOnClick()
	{
	}

	SpecificOnClick::~SpecificOnClick()  
	{
	}

	void SpecificOnClick::Execute()
	{
		Debug::Log("button executed");
	}



}