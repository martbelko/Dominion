#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Log.h"
#include "Dominion/Utils/PlatformUtils.h"
#include "Dominion/Core/Application.h"
#include "Dominion/Events/Event.h"
#include "Dominion/Events/KeyEvent.h"
#include "Dominion/Events/MouseEvent.h"
#include "Dominion/Events/WindowEvent.h"
#include "Dominion/Core/Window.h"
#include "Dominion/Core/Layer.h"
#include "Dominion/Core/LayerStack.h"
#include "Dominion/Core/Input.h"
#include "Dominion/Core/Filesystem.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/Pipeline.h"
#include "Dominion/Renderer/Texture.h"
#include "Dominion/Renderer/Framebuffer.h"
#include "Dominion/Renderer/Renderer.h"
#include "Dominion/Renderer/OrthographicCamera.h"
#include "Dominion/Renderer/OrthographicCameraController.h"
#include "Dominion/Renderer/PerspectiveCamera.h"
#include "Dominion/Renderer/PerspectiveCameraController.h"
#include "Dominion/Renderer/Mesh.h"
#include "Dominion/Renderer/Material.h"
#include "Dominion/Renderer/Model.h"
#include "Dominion/Core/Timestep.h"
#include "Dominion/Scene/Scene.h"
#include "Dominion/Scene/Entity.h"
#include "Dominion/Scene/ScriptableEntity.h"
#include "Dominion/Scene/Components.h"
#include "Dominion/Scene/SceneSerializer.h"
#include "Dominion/Debug/Instrumentor.h"

#if defined(new)
	#undef new
	#include "../imgui/imgui.h"
	#include "../imgui/imgui_internal.h"
	#define new DEBUG_NEW
#else
	#include "../imgui/imgui.h"
	#include "../imgui/imgui_internal.h"
#endif
