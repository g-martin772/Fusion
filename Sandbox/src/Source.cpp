#include "Core/Entrypoint.h"

using namespace FusionEngine;

class FusionForgeApplication : public Application
{
    
};

Application* CreateApplication()
{
    return new FusionForgeApplication();
}