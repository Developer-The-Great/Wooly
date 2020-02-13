#pragma once
#include "Component.h"
#include <string>
#include <unordered_map>


namespace PXG
{
	class TileMetaData : public Component
	{
	public:
		void Start() override{}
		void FixedUpdate(float tick) override { (void)tick; }

		[[nodiscard]]
		const std::unordered_map<std::string, std::string>& GetMetaData() const { return metaData; }

	private:
		friend class LevelLoader;
		std::unordered_map<std::string, std::string> metaData;
		

	};
}
