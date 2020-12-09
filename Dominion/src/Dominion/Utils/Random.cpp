#include "dmpch.h"
#include "Random.h"

#include <random>

namespace Dominion {

	std::default_random_engine Random::s_Generator;

	int8_t Random::RandomInt8(int8_t low, int8_t high)
	{
		std::uniform_int_distribution<int16_t> distribution(low, high); // int8_t not supported
		return static_cast<int8_t>(distribution(s_Generator));
	}

	uint8_t Random::RandomUint8(uint8_t low, uint8_t high)
	{
		std::uniform_int_distribution<uint16_t> distribution(low, high); // uint8_t not supported
		return static_cast<uint8_t>(distribution(s_Generator));
	}

	int16_t Random::RandomInt16(int16_t low, int16_t high)
	{
		std::uniform_int_distribution<int16_t> distribution(low, high);
		return distribution(s_Generator);
	}

	uint16_t Random::RandomUint16(uint16_t low, uint16_t high)
	{
		std::uniform_int_distribution<uint16_t> distribution(low, high);
		return distribution(s_Generator);
	}

	int32_t Random::RandomInt32(int32_t low, int32_t high)
	{
		std::uniform_int_distribution<int32_t> distribution(low, high);
		return distribution(s_Generator);
	}

	uint32_t Random::RandomUint32(uint32_t low, uint32_t high)
	{
		std::uniform_int_distribution<uint32_t> distribution(low, high);
		return distribution(s_Generator);
	}

	int64_t Random::RandomInt64(int64_t low, int64_t high)
	{
		std::uniform_int_distribution<int64_t> distribution(low, high);
		return distribution(s_Generator);
	}

	uint64_t Random::RandomUint64(uint64_t low, uint64_t high)
	{
		std::uniform_int_distribution<uint64_t> distribution(low, high);
		return distribution(s_Generator);
	}

	F32 Random::RandomFloat(F32 low, F32 high)
	{
		std::uniform_real_distribution<F32> distribution(low, high);
		return distribution(s_Generator);
	}

	double Random::RandomDouble(F64 low, F64 high)
	{
		std::uniform_real_distribution<F64> distribution(low, high);
		return distribution(s_Generator);
	}

}
