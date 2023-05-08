#include "Core/Entrypoint.h"

class FusionForgeApplication : public Application
{
    
};

Application* CreateApplication()
{
    return new FusionForgeApplication();
}