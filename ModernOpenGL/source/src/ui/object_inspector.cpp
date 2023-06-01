#include "ui/object_inspector.hpp"

#include <numbers>

#include <ImGui/imgui.h>

#include "core/object.hpp"
#include "core/input.hpp"
#include "low_renderer/camera.hpp"

void ObjectInspector::Show(Scene&)
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

        ImGui::Text("\tPosition:   ");
        ImGui::SameLine();
        ImGui::SliderFloat3("##0", &object->transform.position.x, -10, 10);

        ImGui::Text("\tRotation X: ");
        ImGui::SameLine();
        ImGui::SliderAngle("##1", &object->transform.rotation.x);
        ImGui::Text("\tRotation Y: ");
        ImGui::SameLine();
        ImGui::SliderAngle("##2", &object->transform.rotation.y);
        ImGui::Text("\tRotation Z: ");
        ImGui::SameLine();
        ImGui::SliderAngle("##3", &object->transform.rotation.z);

        ImGui::Text("\tScale:      ");
        ImGui::SameLine();
        ImGui::SliderFloat3("##4", &object->transform.scale.x, -5, 5);

        if (object->mesh)
        {
            ImGui::Text("\tMesh offset:");
            ImGui::SameLine();
            ImGui::SliderFloat3("##13", &object->mesh->position.x, -10, 10);
        }

        if (Camera* camera = dynamic_cast<Camera*>(object))
        {
            ImGui::Separator();
            ImGui::Text("Camera:");

            ImGui::Text("\tFOV:    ");
            ImGui::SameLine();
            ImGui::SliderAngle("##5", &camera->fov, 30, 90);
            if (Input::controllerConnected[inputs::Controller_0])
            {
                ImGui::Text("\tStick sensitivity:");
                ImGui::SameLine();
                ImGui::SliderFloat("##6", &camera->stickSensitivity, 15, 50);
            }
            ImGui::Text("\tMouse sensitivity:");
            ImGui::SameLine();
            ImGui::SliderFloat("##14", &camera->mouseSensitivity, 0.1f, 10);

            ImGui::Text("\tMovement speed: ");
            ImGui::SameLine();
            ImGui::SliderFloat("##12", &camera->moveSpeed, 0.1f, 10);

            ImGui::Text("\tNear:   ");
            ImGui::SameLine();
            ImGui::InputFloat("##7", &camera->near);
            ImGui::Text("\tFar:    ");
            ImGui::SameLine();
            ImGui::InputFloat("##8", &camera->far);

            bool displayLookAt = false, displayForward = false;

            if (Input::controllerConnected[inputs::Controller_0])
            {
                displayLookAt = true;
                displayForward = true;
            }
            else
            {
                ImGui::Text("\tIs looking at:");
                ImGui::SameLine();
                ImGui::Checkbox("##9", &camera->isLookingAt);

                if (camera->isLookingAt)
                    displayLookAt = true;
                else
                    displayForward = true;
            }

            if (displayLookAt)
            {
                ImGui::Text("\tLook at:");
                ImGui::SameLine();
                ImGui::SliderFloat3("##10", &camera->lookingAt.x, -10, 10);
            }
            if (displayForward)
            {
                Vector3 forward = camera->GetForward();
                ImGui::Text("\tForward: %.3f, %.3f, %.3f", forward.x, forward.y, forward.z);
            }

            ImGui::Text("\tFPS View:");
            ImGui::SameLine();
            ImGui::Checkbox("##11", &camera->fpsView);
        }
    }
    ImGui::End();
}
