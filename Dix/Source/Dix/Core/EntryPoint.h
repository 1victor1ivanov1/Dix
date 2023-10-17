#pragma once

#include "Dix/Core/Base.h"
#include "Dix/Core/Application.h"

extern Dix::Application* Dix::CreateApplication(ApplicationCommandLineArgs args);

#ifdef DIX_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    Dix::Log::Init();

    Dix::Application* app = Dix::CreateApplication({ argc, argv });

    app->Run();

    delete app;

    return 0;
}

#endif