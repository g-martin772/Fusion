#include "../../RenderModule.h"
#include "VulkanModuleData.h"

namespace FusionEngine
{
    static RenderModuleData* s_Data;
    
    RenderModuleData* RenderModule::Init()
    {
        RenderModuleData* data = new RenderModuleData();
        s_Data = data;
        return data;
    }

    void RenderModule::Reload(RenderModuleData* data)
    {
        data->Reset();
        s_Data = data;
    }

    void RenderModule::Shutdown(RenderModuleData* data)
    {
        delete s_Data;
    }

    RenderModuleData* RenderModule::GetDataPointer()
    {
        return s_Data;
    }
}
