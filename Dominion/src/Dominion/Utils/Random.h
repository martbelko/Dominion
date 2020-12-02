#pragma once

#include <cstdint>
#include <random>

namespace Dominion {

	class Random
	{
	public:
		static int8_t RandomInt8(int8_t low, int8_t high);
		static uint8_t RandomUint8(uint8_t low, uint8_t high);

		static int16_t RandomInt16(int16_t low, int16_t high);
		static uint16_t RandomUint16(uint16_t low, uint16_t high);

		static int32_t RandomInt32(int32_t low, int32_t high);
		static uint32_t RandomUint32(uint32_t low, uint32_t high);

		static int64_t RandomInt64(int64_t low, int64_t high);
		static uint64_t RandomUint64(uint64_t low, uint64_t high);

		static float RandomFloat(float low, float high);
		static double RandomDouble(double low, double high);
	private:
		static std::default_random_engine s_Generator;
	};

}
