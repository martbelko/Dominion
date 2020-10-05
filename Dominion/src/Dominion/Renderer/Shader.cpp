#include "dmpch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Dominion {

	Ref<Shader> Shader::Create(const std::string_view& name, const std::string_view& vertexFilepath, const std::string_view& pixelFilepath)
	{
		return CreateRef<OpenGLShader>(name, vertexFilepath, pixelFilepath);
	}

}
