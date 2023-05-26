#include "ui/object_hierarchy.hpp"

#include <ImGui/imgui.h>

#include "core/scene.hpp"

void ObjectHierarchy::Show(Scene& scene)
{
    ImGui::Begin("Object Hierarchy");
    ShowObject(&scene.root);
    ShowObject(&scene.camera);
    ImGui::End();

    if (!selected)
        selected = &scene.root;
}

void ObjectHierarchy::ShowObject(Object* const object)
{
    int imguiFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
    const bool empty = object->mChildren.empty();
    if (empty)
        imguiFlags |= ImGuiTreeNodeFlags_Leaf;

    if (ImGui::TreeNodeEx(object->name.c_str(), imguiFlags))
    {
        if (ImGui::IsItemClicked())
            selected = object;
        if (!empty)
            for (Object* const o : object->mChildren)
                ShowObject(o);
        ImGui::TreePop();
    }
}
