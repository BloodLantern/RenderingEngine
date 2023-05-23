#include "ui/object_hierarchy.hpp"

#include <ImGui/imgui.h>

#include "core/scene.hpp"


void ObjectHierarchy::Show(Scene& scene)
{
    ImGui::Begin("Object Hierarchy");
    ShowObject(&scene.root);
    ImGui::End();
}

void ObjectHierarchy::ShowObject(const Object *const object)
{
    int imguiFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
    for (const Object* const o : object->mChildren)
    {
        if (o->mChildren.empty())
            imguiFlags |= ImGuiTreeNodeFlags_Leaf;

        if (ImGui::TreeNodeEx(o->name.c_str(), imguiFlags))
        {
            ShowObject(o);
            ImGui::TreePop();
        }
    }
}
