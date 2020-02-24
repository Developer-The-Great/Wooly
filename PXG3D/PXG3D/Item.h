#pragma once

#ifdef _MSC_VER
#include <cstddef>
#include <cstdint>
#else
#include <cstddint>
#endif


namespace PXG
{
	using item_identifier_t = std::uint32_t;
	const item_identifier_t null_item = 0;
}