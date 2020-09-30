#pragma once

#include "Bindable.h"

#include <string>
#include <string_view>

namespace Dominion {

	class Shader : public Bindable
	{
	public:
		Shader(const std::string_view& name)
			: m_Name(name) {}
		virtual ~Shader() = default;

		virtual const std::string& GetName() const { return m_Name; }

		static Shader* Create(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath);
	protected:
		const std::string m_Name;
	};

}
