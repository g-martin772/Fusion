// #pragma once
// #include "imgui.h"
//
// inline ImVec2 operator+ (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
// inline ImVec2 operator- (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
// inline ImVec2 operator* (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
// inline ImVec2 operator/ (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
//
// namespace FusionEngine { namespace ImGuiUtils {
//     
//     
//     class Style
//     {
//     public:
//         template<typename T>
//         Style(ImGuiStyleVar styleVar, T value) { ImGui::PushStyleVar(styleVar, value); }
//         ~Style() { ImGui::PopStyleVar(); }
//     };
//     
// } }
