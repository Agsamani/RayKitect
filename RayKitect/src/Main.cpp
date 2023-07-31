#include <rktpch.h>

#include "core/Base.h"
#include "core/Application.h"
#include "MainLayer.h"

class RayKitectApp : public Application {
public:
	RayKitectApp(const ApplicationSpecification& spec)
		:Application(spec)
	{
		PushLayer(new MainLayer());
	}

	~RayKitectApp() {
	}
};

int main(int argc, char** argv) {

	Log::Init();

	ApplicationSpecification spec;
	auto app =  new RayKitectApp(spec);
	app->run();
	delete app;

	return 0;
}