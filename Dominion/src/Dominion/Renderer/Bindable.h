#pragma once

namespace Dominion {

	class Bindable
	{
	public:
		Bindable() = default;
		virtual ~Bindable() = default;

		virtual void Bind() = 0;
	};

}
