#include <Dix.h>
#include <Dix/Core/EntryPoint.h>

class Sandbox : public Dix::Application
{
public:
	Sandbox(const Dix::ApplicationSpecification& specification)
		: Dix::Application(specification)
	{
	}

	~Sandbox()
	{
	}
};

Dix::Application* Dix::CreateApplication(Dix::ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox Application";
	spec.Args = args;

	return new Sandbox(spec);
}