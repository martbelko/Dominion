#pragma once

#include <xhash>

namespace Dominion {

	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);

		operator uint64_t() const { return mUUID; }
	private:
		uint64_t mUUID; // TODO: Implement 128bit UUID
	};

}

namespace std {

	template<>
	struct hash<Dominion::UUID>
	{
		std::size_t operator()(const Dominion::UUID& uuid) const
		{
			return hash<uint64_t>()(static_cast<uint64_t>(uuid));
		}
	};

}
