// #include "EditorLayer.h"
//
// #include <imgui.h>
//
// #include "UI/ImGuiUtils.h"
//
// void EditorLayer::DrawSceneHierarchyPanel()
// {
//     {
//         ImGuiUtils::Style style(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
//         ImGui::Begin("Scene Hierarchy");
//     }
//
//     ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(5.0f, 5.0f));
//     ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 5.0f);
//
//     ImGui::Spacing();
//
//     // Use entity handle instead of id?
//     static UUID selected;
//     static UUID lastSelected = selected;
//
//     const auto view = m_Scene->View<std::string_view, UUID>();
//     
//     for (auto [entity, name, id] : view.each())
//     {
//         ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow
//             | ImGuiTreeNodeFlags_OpenOnDoubleClick
//             | ImGuiTreeNodeFlags_SpanAvailWidth
//             | ImGuiTreeNodeFlags_Leaf
//             | ImGuiTreeNodeFlags_NoTreePushOnOpen;
//         
//         if (selected == id)
//             nodeFlags |= ImGuiTreeNodeFlags_Selected;
//         
//         ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(id.Low)), nodeFlags, "%s", name.data());
//         
//         if (ImGui::IsItemClicked())
//             selected = id;
//     }
//
//     if(selected != lastSelected)
//         m_SelectedEntity = m_Scene->GetEntityByUUID(selected);
//     
//     ImGui::End();
// }
