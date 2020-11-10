#include <Dominion.h>
#include <Dominion/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Dominion {

	class EditorApp : public Application
	{
	public:
		EditorApp()
		{
			PushLayer(new EditorLayer());
		}

		~EditorApp()
		{

		}
	};

	Application* CreateApplication()
	{
		return new EditorApp();
	}

}
