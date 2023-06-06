#include "ui/object_inspector.hpp"

#include <numbers>

#include <ImGui/imgui.h>

#include "core/object.hpp"
#include "core/input.hpp"
#include "low_renderer/camera.hpp"
#include "low_renderer/light.hpp"
#include "low_renderer/directional_light.hpp"
#include "low_renderer/point_light.hpp"
#include "low_renderer/spot_light.hpp"

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
        ImGui::SliderFloat3("##0", &object->transform.position.x, -2, 2);

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
                ImGui::SliderFloat3("##10", &camera->lookingAt.x, -2, 2);
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

        if (Light* light = dynamic_cast<Light*>(object))
        {
            ImGui::Separator();
            ImGui::Text("Light:");

            ImGui::Text("\tAmbient:    ");
            ImGui::SameLine();
            ImGui::SliderFloat3("##15", &light->ambient.x, 0, 1);
            ImGui::Text("\tDiffuse:    ");
            ImGui::SameLine();
            ImGui::SliderFloat3("##16", &light->diffuse.x, 0, 1);
            ImGui::Text("\tSpecular:   ");
            ImGui::SameLine();
            ImGui::SliderFloat3("##17", &light->specular.x, 0, 1);

            if (DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(light))
            {
                ImGui::Separator();
                ImGui::Text("Directional light:");

                ImGui::Text("\tDirection:   ");
                ImGui::SameLine();
                ImGui::SliderFloat3("##18", &directionalLight->direction.x, -1, 1);
            }

            else if (PointLight* pointLight = dynamic_cast<PointLight*>(light))
            {
                ImGui::Separator();
                ImGui::Text("Point light:");

                ImGui::Text("\tConstant:    ");
                ImGui::SameLine();
                ImGui::SliderFloat("##20", &pointLight->constant, 0, 1);
                ImGui::Text("\tLinear:      ");
                ImGui::SameLine();
                ImGui::SliderFloat("##21", &pointLight->linear, 0, 1);
                ImGui::Text("\tQuadratic:   ");
                ImGui::SameLine();
                ImGui::SliderFloat("##22", &pointLight->quadratic, 0, 1);
            }

            else if (SpotLight* spotLight = dynamic_cast<SpotLight*>(light))
            {
                ImGui::Separator();
                ImGui::Text("Spot light:");

                ImGui::Text("\tDirection:   ");
                ImGui::SameLine();
                ImGui::SliderFloat3("##23", &spotLight->direction.x, -1, 1);

                ImGui::Text("\tConstant:     ");
                ImGui::SameLine();
                ImGui::SliderFloat("##24", &spotLight->constant, 0, 1);
                ImGui::Text("\tLinear:       ");
                ImGui::SameLine();
                ImGui::SliderFloat("##25", &spotLight->linear, 0, 1);
                ImGui::Text("\tQuadratic:    ");
                ImGui::SameLine();
                ImGui::SliderFloat("##26", &spotLight->quadratic, 0, 1);

                ImGui::Text("\tCutoff:       ");
                ImGui::SameLine();
                ImGui::SliderAngle("##27", &spotLight->cutOff);
                ImGui::Text("\tOuter cuttoff:");
                ImGui::SameLine();
                ImGui::SliderAngle("##28", &spotLight->outerCutOff);
            }
        }
    }
    ImGui::End();
}
