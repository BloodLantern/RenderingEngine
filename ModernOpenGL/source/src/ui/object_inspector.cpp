#include "ui/object_inspector.hpp"

#include <numbers>

#include <ImGui/imgui.h>

#include "core/object.hpp"
#include "low_renderer/camera.hpp"

void ObjectInspector::Show(Scene &scene)
{
    Object* const object = *mObject;
    ImGui::Begin("Object Inspector");
    if (object)
    {
        // Object name
        ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2 - ImGui::CalcTextSize(object->name.c_str()).x / 2);
        ImGui::Text(object->name.c_str());
        ImGui::Spacing();

        // Transform
        ImGui::Text("Transform:");

        ImGui::Text("\tPosition:  ");
        ImGui::SameLine();
        ImGui::InputFloat3("##0", &object->transform.position.x);

        ImGui::Text("\tRotation X:");
        ImGui::SameLine();
        ImGui::SliderAngle("##1", &object->transform.rotation.x);
        ImGui::Text("\tRotation Y:");
        ImGui::SameLine();
        ImGui::SliderAngle("##2", &object->transform.rotation.y);
        ImGui::Text("\tRotation Z:");
        ImGui::SameLine();
        ImGui::SliderAngle("##3", &object->transform.rotation.z);

        ImGui::Text("\tScale:     ");
        ImGui::SameLine();
        ImGui::InputFloat3("##4", &object->transform.scale.x);

        if (Camera* camera = dynamic_cast<Camera*>(object))
        {
            ImGui::Separator();
            ImGui::Text("Camera:");

            ImGui::Text("\tFOV:    ");
            ImGui::SameLine();
            ImGui::InputFloat("##5", &camera->fov);

            ImGui::Text("\tNear:   ");
            ImGui::SameLine();
            ImGui::InputFloat("##6", &camera->near);
            ImGui::Text("\tFar:    ");
            ImGui::SameLine();
            ImGui::InputFloat("##7", &camera->far);

            ImGui::Text("\tLook At:");
            ImGui::SameLine();
            ImGui::InputFloat3("##8", &camera->lookAt.x);
        }
    }
    ImGui::End();
}
