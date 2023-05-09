#include "fepch.h"
#include "Application.h"

void Application::Init()
{
    printf("startup\n");
}

void Application::Run()
{
    while(m_Running)
    {
        printf("running\n");
    }
}

void Application::Shutdown()
{
    printf("end\n");
}
