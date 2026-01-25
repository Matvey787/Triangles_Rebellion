#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
    
#include "glad/glad.h"

#include <unordered_map>
#include <vector>
#include <memory>
#include <algorithm>
#include <variant>

namespace OGLWrap {

class KeyEvent {
public:
    int key;

    KeyEvent(int k) : key(k) {}
};

class IKeyboardHandler {
public:
    virtual ~IKeyboardHandler() = default;
    virtual bool isKeyPressed(int key) const = 0;
    
};

class MouseEvent {
public:
    float x;
    float y;
    float xOffset;
    float yOffset;
};

class IMouseHandler {
public:
    virtual ~IMouseHandler() = default;
    virtual const MouseEvent& getMouseEvent() const = 0;
};

using InputEvent = std::variant<KeyEvent, MouseEvent>;

class IInputListener {
public:
    virtual ~IInputListener() = default;
    virtual void onInputEvent(const InputEvent& event) = 0;
};

class InputDispatcher {
    std::vector<std::weak_ptr<IInputListener>> listeners_;

public:
    void dispatch(const InputEvent& event) {
        for (auto& weakListener : listeners_) {
            if (auto listener = weakListener.lock()) {
                std::cerr << "Dispatching event to listener\n";
                listener->onInputEvent(event);
            }
        }
    }
    
    void addListener(std::weak_ptr<IInputListener> newListener) {
        listeners_.push_back(std::move(newListener));
    }

    void removeDiedListeners() {
        listeners_.erase(
            std::remove_if (
                listeners_.begin(), 
                listeners_.end(),
                [](const auto& weakListener) { return weakListener.expired(); }
            ),
            listeners_.end()
        );
    }
};

// Общий класс для управления всеми обработчиками ввода
class InputManager {
private:
    struct InputState {
        std::unordered_map<int, bool> keyStates;
        MouseEvent mouseEvent;
        float lastMouseX = 0.0f;
        float lastMouseY = 0.0f;
        bool firstMouse = true;
    };
    
    InputState state;
    
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (manager) {
            if (action == GLFW_PRESS) {
                manager->state.keyStates[key] = true;
            } else if (action == GLFW_RELEASE) {
                manager->state.keyStates[key] = false;
            }
        }
    }
    
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        auto* manager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
        if (manager) {
            auto& state = manager->state;
            
            if (state.firstMouse) {
                state.lastMouseX = static_cast<float>(xpos);
                state.lastMouseY = static_cast<float>(ypos);
                state.firstMouse = false;
            }
            
            state.mouseEvent.x = static_cast<float>(xpos);
            state.mouseEvent.y = static_cast<float>(ypos);
            state.mouseEvent.xOffset = static_cast<float>(xpos) - state.lastMouseX;
            state.mouseEvent.yOffset = state.lastMouseY - static_cast<float>(ypos); // Обратный Y
            
            state.lastMouseX = static_cast<float>(xpos);
            state.lastMouseY = static_cast<float>(ypos);
        }
    }
    
public:
    InputManager(GLFWwindow* win) {
        glfwSetWindowUserPointer(win, this);
        glfwSetKeyCallback(win, keyCallback);
        glfwSetCursorPosCallback(win, mouseCallback);
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    bool isKeyPressed(int key) const {
        auto it = state.keyStates.find(key);
        return it != state.keyStates.end() && it->second;
    }
    
    const MouseEvent& getMouseEvent() const {
        return state.mouseEvent;
    }

    void resetMouseOffset() {
        state.mouseEvent.xOffset = 0.0f;
        state.mouseEvent.yOffset = 0.0f;
    }
};

class InputService {
    InputDispatcher inputDispatcher_;
    std::unique_ptr<InputManager> inputManager_;

public:
    InputService() = delete;

    explicit InputService(GLFWwindow* window) : 
        inputManager_(std::make_unique<InputManager>(window)) {}

    void addListener(const std::shared_ptr<IInputListener>& newListener) {
        inputDispatcher_.addListener(std::weak_ptr<IInputListener>(newListener));
    }

    void handleWASDkeys() {
        if (inputManager_->isKeyPressed(GLFW_KEY_W)) {
            std::cerr << "W key is pressed\n";
            inputDispatcher_.dispatch(KeyEvent(GLFW_KEY_W));
        }
        if (inputManager_->isKeyPressed(GLFW_KEY_A)) {
            inputDispatcher_.dispatch(KeyEvent{GLFW_KEY_A});
        }
        if (inputManager_->isKeyPressed(GLFW_KEY_S)) {
            inputDispatcher_.dispatch(KeyEvent(GLFW_KEY_S));
        }
        if (inputManager_->isKeyPressed(GLFW_KEY_D)) {
            inputDispatcher_.dispatch(KeyEvent(GLFW_KEY_D));
        }
    }

    void handleMouseEvents() {
        const auto& mouseEvent = inputManager_->getMouseEvent();
        if ( std::fabs(mouseEvent.xOffset) > 0.000001f || std::fabs(mouseEvent.yOffset) > 0.000001f) {
            inputDispatcher_.dispatch(mouseEvent);
            inputManager_->resetMouseOffset();
        }
    }

    void update() {
        inputDispatcher_.removeDiedListeners();
    }
};

}; // namespace OGLWrap