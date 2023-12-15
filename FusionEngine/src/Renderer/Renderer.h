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

        static void BeginScene(const Shared<Camera>& camera);
        static void EndScene();
        
        static void DrawObjModel(Shared<ObjModel> model);
    };
    
}
