#pragma once

#include "Dominion/Core/Base.h"

namespace Dominion {

	class Bindable
	{
	public:
		Bindable() = default;
		virtual ~Bindable() = default;

		virtual void Bind() = 0;
	};

}
