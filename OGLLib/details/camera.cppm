module;

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


#include <iostream>
#include <memory>

import input;

export module camera;

export namespace OGLWrap {

class ICamera {
public:
    virtual void moveForward (float speed) = 0;
    virtual void moveBackward(float speed) = 0;
    virtual void moveLeft    (float speed) = 0;
    virtual void moveRight   (float speed) = 0;
    virtual void rotate      (float xOffset, float yOffset) = 0;

    virtual void setCameraPos(const glm::vec3& cameraPosVec) = 0;
    virtual void setTargetPos(const glm::vec3& targetPosVec) = 0;
    virtual void setCameraUp (const glm::vec3& cameraUpVec)  = 0;
    virtual void setYaw      (float yaw)   = 0;
    virtual void setPitch    (float pitch) = 0;

    virtual float getYaw  () = 0;
    virtual float getPitch() = 0;
    virtual glm::vec3 getCameraPos () = 0;
    virtual glm::vec3 getTargetPos () = 0;
    virtual glm::vec3 getCameraUp  () = 0;
    virtual glm::mat4 getViewMatrix() = 0;
    

    virtual ~ICamera() = default;
};

class XYZCamera : public ICamera {
private:
    float yaw = -90.0f;   // Начинаем смотрящим вдоль отрицательной оси Z
    float pitch = 0.0f;
    glm::vec3 cameraPosVec_;
    glm::vec3 targetPosVec_;
    glm::vec3 cameraUpVec_;

public:
    XYZCamera()
        : cameraPosVec_(glm::vec3(10.0f, 10.0f, 10.0f)),
          targetPosVec_(glm::vec3(0.0f, 0.0f, 0.0f)),
          cameraUpVec_(glm::vec3(0.0f, 1.0f, 0.0f)) {}

    void setCameraPos(const glm::vec3& cameraPosVec) override {
        cameraPosVec_ = cameraPosVec;
    }

    void setTargetPos(const glm::vec3& targetPosVec) override {
        targetPosVec_ = targetPosVec;
    }

    void setCameraUp( const glm::vec3& cameraUpVec) override {
        cameraUpVec_ = cameraUpVec;
    }

    glm::vec3 getCameraPos() override {
        return cameraPosVec_;
    }
    glm::vec3 getTargetPos() override {
        return targetPosVec_;
    }
    glm::vec3 getCameraUp() override {
        return cameraUpVec_;
    }

    void moveForward(float speed) override {
        glm::vec3 forward = glm::normalize(targetPosVec_ - cameraPosVec_);
        cameraPosVec_ += forward * speed;
        targetPosVec_ += forward * speed;
    }

    void moveBackward(float speed) override {
        glm::vec3 backward = glm::normalize(cameraPosVec_ - targetPosVec_);
        cameraPosVec_ += backward * speed;
        targetPosVec_ += backward * speed;
    }

    void moveLeft(float speed) override {
        glm::vec3 forward = glm::normalize(targetPosVec_ - cameraPosVec_);
        glm::vec3 left = glm::normalize(glm::cross(cameraUpVec_, forward));
        cameraPosVec_ += left * speed;
        targetPosVec_ += left * speed;
    }

    void moveRight(float speed) override {
        glm::vec3 forward = glm::normalize(targetPosVec_ - cameraPosVec_);
        glm::vec3 right = glm::normalize(glm::cross(forward, cameraUpVec_));
        cameraPosVec_ += right * speed;
        targetPosVec_ += right * speed;
    }

    void rotate(float xOffset, float yOffset) override {
        float sensitivity = 0.1f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        targetPosVec_ = cameraPosVec_ + glm::normalize(direction);
    }

    glm::mat4 getViewMatrix() override {
        return glm::lookAt(cameraPosVec_, targetPosVec_, cameraUpVec_);
    }

    void setYaw(float yaw) override {
        this->yaw = yaw;
    }

    void setPitch(float pitch) override {
        this->pitch = pitch;
    }

    float getYaw() override {
        return yaw;
    }

    float getPitch() override {
        return pitch;
    }
};

class ICameraCommand {
public:
    virtual ~ICameraCommand() = default;
    virtual void execute() = 0;
};

class CameraCommand : public ICameraCommand {
protected:
    std::shared_ptr<ICamera> camera_;
    float value_;
    
public:
    CameraCommand(std::shared_ptr<ICamera> camera, float value = 1.0f)
        : camera_(std::move(camera)), value_(value) {}
    
    virtual ~CameraCommand() = default;
};

class MoveForwardCommand : public CameraCommand {
public:
    using CameraCommand::CameraCommand;
    
    void execute() override {
        if (camera_) camera_->moveForward(value_);
    }
};

class MoveBackwardCommand : public CameraCommand {
public:
    using CameraCommand::CameraCommand;
    
    void execute() override {
        if (camera_) camera_->moveBackward(value_);
    }
};

class MoveLeftCommand : public CameraCommand {
public:
    using CameraCommand::CameraCommand;
    
    void execute() override {
        if (camera_) camera_->moveLeft(value_);
    }
};

class MoveRightCommand : public CameraCommand {
public:
    using CameraCommand::CameraCommand;
    
    void execute() override {
        if (camera_) camera_->moveRight(value_);
    }
};

// class RotateCommand : public CameraCommand {
// public:
//     using CameraCommand::CameraCommand;
    
//     void execute() override {

//         camera_->setPitch()
//         yaw += xOffset * sensitivity;
//         pitch += yOffset * sensitivity;
        
//         if (pitch > 89.0f) pitch = 89.0f;
//         if (pitch < -89.0f) pitch = -89.0f;
        
//         glm::vec3 direction;
//         direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//         direction.y = sin(glm::radians(pitch));
//         direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

//         camera_->setTargetPos(camera_->getCameraPos() + glm::normalize(direction));
//     }
// };


class CameraController : public IInputListener {
    std::shared_ptr<ICamera> camera_;
    std::unordered_map<int, std::shared_ptr<ICameraCommand>> keyBindings_;

public:
    CameraController() = delete;
    
    CameraController(std::shared_ptr<ICamera> camera) : camera_(camera) {
        keyBindings_[GLFW_KEY_W] = std::make_shared<MoveForwardCommand> (camera_, 0.5f);
        keyBindings_[GLFW_KEY_S] = std::make_shared<MoveBackwardCommand>(camera_, 0.5f);
        keyBindings_[GLFW_KEY_A] = std::make_shared<MoveLeftCommand>    (camera_, 0.5f);
        keyBindings_[GLFW_KEY_D] = std::make_shared<MoveRightCommand>   (camera_, 0.5f);
    }
    
    void bindKey(int key, std::shared_ptr<ICameraCommand> command) {
        keyBindings_[key] = std::move(command);
    }

    void onInputEvent(const InputEvent& event) override {
        std::cerr << "CameraController received an event\n";
        std::visit([this](const auto& e) { handleEvent(e); }, event);
        std::cerr << "CameraController handled the event\n";
    }

private:
    void handleEvent(const KeyEvent& event) {
        std::cerr << "Handling WASD keys\n";
        int key = event.key;
        auto it = keyBindings_.find(key);
        if (it != keyBindings_.end()) {
            it->second->execute();
        }
    }

    void handleEvent(const MouseEvent& event) {
        camera_->rotate(event.xOffset, event.yOffset);
    }
};

}; // namespace OGLWrap