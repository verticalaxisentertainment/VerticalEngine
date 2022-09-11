#include "pch.h"
#include "UUID.h"

#include "Math/Mathematics.h"

namespace id
{
	UUID::UUID()
		:m_UUID(Math::RandomInt(100000,9999999))
	{
	}

	UUID::UUID(uint64_t id)
		:m_UUID(id)
	{
	}
}
