#pragma once

//#include <xhash>

#include "Core.h"

namespace id
{
	class _API UUID
	{
	public:
		UUID();
		UUID(uint64_t id);

		UUID(const UUID&) = default;

		const uint64_t id() const { return m_UUID; }
	private:
		uint64_t m_UUID;
	};
}