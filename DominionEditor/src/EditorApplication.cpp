#include <Dominion.h>
#include <Dominion/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Dominion {

	class Editor : public Application
	{
	public:
		Editor(ApplicationCommandLineArgs args)
			: Application("Dominion Editor", args)
		{
			PushLayer(new EditorLayer());
		}

		~Editor()
		{
		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		return new Editor(args);
	}

}
