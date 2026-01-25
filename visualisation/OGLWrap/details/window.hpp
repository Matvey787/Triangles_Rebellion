#pragma once

#include "geo.h"
#include "color.hpp"
#include "render.hpp"
#include "input.hpp"
#include "camera.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "glad/glad.h"

#include <memory>
#include <exception>
#include <stdexcept>
#include <string_view>

namespace OGLWrap {

class WinConfig {
public:
    size_t      width      = 600;
    size_t      height     = 400;
    std::string title      = "Window";
    RGB_color   bgColor    = ColorFactory::fromName("black");
    size_t      start_posX = 0;
    size_t      start_posY = 0;
};

class IWin {
public:
    virtual void create() = 0;
    virtual void close () = 0;
    virtual void render() = 0;
    virtual ~IWin() = default;
};

class BaseWin : public IWin {
protected:
    WinConfig config;

public: 
    BaseWin(const WinConfig& config) : config(config) {}

    size_t getWidth() const { return config.width; }
    size_t getHeight() const { return config.height; }
    std::string_view getType() const { return config.title; }

    virtual ~BaseWin() = default;
};


class GLFWWin : public BaseWin {
private:
    static bool glfwInitialized;  // Добавляем статический флаг
    static bool gladInitialized;
    
public:
    GLFWWin(const WinConfig& config) : BaseWin(config) {
        // Инициализируем GLFW в конструкторе (только один раз)
        if (!glfwInitialized) {
            if (glfwInit() == GLFW_FALSE) {
                throw std::runtime_error("GLFW initialization failed");
            }
            glfwInitialized = true;
        }
        create();
    }
    
    ~GLFWWin() {
        close();
    }
    
protected:
    GLFWwindow* glfwWindow = nullptr;
    void create() override {
        if (glfwWindow) return;  // Окно уже создано
        
        glfwDefaultWindowHints();
        
        // OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        
        glfwWindow = glfwCreateWindow(
            static_cast<int>(config.width),  // Исправляем каст
            static_cast<int>(config.height),
            config.title.c_str(),
            nullptr,
            nullptr
        );

        if (!glfwWindow) throw std::runtime_error("Window creation failed");
        
        glfwMakeContextCurrent(glfwWindow);
        

        if (!gladInitialized) {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                throw std::runtime_error("Failed to initialize GLAD");
            }
            gladInitialized = true;
        }

        glClearColor(
            config.bgColor.rFloat(),
            config.bgColor.gFloat(),
            config.bgColor.bFloat(),
            config.bgColor.aFloat()
        );
        
        glfwSetWindowPos(glfwWindow, 
                         static_cast<int>(config.start_posX),  // Исправляем каст
                         static_cast<int>(config.start_posY));
        
        glfwShowWindow(glfwWindow);
        glfwPollEvents();
    }

    void close() override {         
        if (glfwWindow) {
            glfwDestroyWindow(glfwWindow);
            glfwWindow = nullptr;
        }
    }

    void makeWindowCurrent() const {
        if (!glfwWindow) return;
        glfwMakeContextCurrent(glfwWindow);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void redrawWindow() const {
        if (!glfwWindow) return;
        glfwSwapBuffers(glfwWindow);
    }

    void getWindowEvents() const {
        glfwPollEvents();
    }

    void render() override {
        makeWindowCurrent();
        redrawWindow();
        getWindowEvents();
    }
};

// Инициализация статического члена
bool GLFWWin::glfwInitialized = false;
bool GLFWWin::gladInitialized = false;

class Win3D : public GLFWWin {
private:
    std::unique_ptr<IRender> render_;
    std::unique_ptr<InputService> inputService_;
    std::shared_ptr<CameraController> cameraController_;
    std::shared_ptr<ICamera> camera_;
    
public:
    Win3D(WinConfig config) : GLFWWin(config) {
        camera_ = std::make_shared<XYZCamera>();
        cameraController_ = std::make_shared<CameraController>(camera_);
        inputService_ = std::make_unique<InputService>(glfwWindow);
        inputService_->addListener(cameraController_);
        
        render_ = std::make_unique<OGLRender>();

        makeWindowCurrent();
    }

    void render() override {
        
        makeWindowCurrent();
        
        render_->setViewMatrix(camera_->getViewMatrix());
        if (render_) render_->render();
        
        redrawWindow();
        getWindowEvents();

        inputService_->handleWASDkeys();
        inputService_->handleMouseEvents();
    }

    void addTriangle(const Geo::Triangle& triangle, const RGB_color& color = RGB_color{255, 255, 255}) {

        std::shared_ptr<IOGLMesh> mesh = std::make_shared<TriangleMesh>(triangle, color);
        
        addMesh(mesh);
    }

    void addMesh(const std::shared_ptr<IOGLMesh>& mesh) {
        render_->addMesh(mesh);
    }

};
}; // namespace OGLWrap