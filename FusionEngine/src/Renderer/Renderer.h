#pragma once

namespace FusionEngine
{
    class ObjModel;
}

namespace FusionEngine
{
    class Camera;

    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Ref<Camera>& camera);
        static void EndScene();
        
        static void DrawObjModel(Ref<ObjModel> model);
    };
    
}
