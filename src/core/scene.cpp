#include "scene.h"
#include "utility/log.h"
#include "renderer.h"

void Scene::Render() const {
    if (!CanRender()) {
        LOG_ERROR("Scene - scene are not ready to render");
        return;
    }

    shader_->SetViewMatrix(camera_->GetViewMatrix());
    shader_->SetProjectionMatrix(camera_->GetProjectionMatrix());
    shader_->SetViewportMatrix(frame_buffer_->GetViewportMatrix());
    for (const auto& mesh_obj : mesh_objs_) {
        if (mesh_obj->mesh == nullptr) {
            LOG_ERROR("Scene - mesh object has no mesh");
            continue;
        }
        shader_->SetModelMatrix(mesh_obj->GetModelMatrix());
        shader_->NormalizeLights();
        Renderer::DrawModel(mesh_obj->mesh->model(), shader_, frame_buffer_);
    }
}

void Callbacks::OnKeyPressed(Win32Wnd *windows, const KeyCode keycode) {
    const auto scene = static_cast<Scene*>(windows->GetUserData().get());
    if (scene == nullptr) {
        LOG_WARNING("Callbacks - cannot get scene object from user data");
        return;
    }
    const auto camera = scene->GetCamera();
    switch (keycode) {
        case A:
            camera->transform.position[0] += 0.1f;
        break;
        case D:
            camera->transform.position[0] -= 0.1f;
        break;
        case W:
            camera->transform.position[2] -= 0.1f;
        break;
        case S:
            camera->transform.position[2] += 0.1f;
        break;
        case Q:
            camera->transform.position[1] += 0.1f;
        break;
        case E:
            camera->transform.position[1] -= 0.1f;
        break;
        case SPACE:
            camera->transform.position = {0, 0, 5};
        break;
        case ESC:
            windows->CloseWnd();
        break;
    }
}
