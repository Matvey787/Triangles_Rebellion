#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stdexcept>

namespace GLWrap
{

enum class obj_t
{
    WIDGET,
    TRIANGLE
};

struct color
{
    unsigned char r; // red channel
    unsigned char g; // green channel
    unsigned char b; // blue channel
    unsigned char a; // alpha channel
};


class Object {
protected:
    static std::vector<float> verticesStorage;
    obj_t objType;
    color objColor;
    size_t vertexCount;
    std::vector<float>::iterator offset;

public:

    static const std::vector<float>& getVerticesStorage()
    {
        return verticesStorage;
    }

    void setRGBColor(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255)
    {
        float r_percentage = r / 255.0f;
        float g_percentage = g / 255.0f;
        float b_percentage = b / 255.0f;
        objColor = {r, g, b, 1};

        for (size_t v = 0; v < vertexCount; ++v) {
            auto it = offset + v * 7;
            it[3] = r_percentage;
            it[4] = g_percentage;
            it[5] = b_percentage;
            it[6] = 1.0f;
        }
    }

    virtual obj_t getType() const = 0;

    virtual ~Object() = default;
};

class Triangle : public Object {
public:
    Triangle()
    {
        objType = obj_t::TRIANGLE;
    }   

    Triangle(std::vector<float>&& vertices)
    {
        if (vertices.size() != 9)
            throw std::invalid_argument("3 vertices * 3 dimensions = vector of 9 elements");

        objType = obj_t::TRIANGLE;
        vertexCount = 3;

        // vertex = 3 coordinates + 4 colors = 7 floats
        size_t oldSize = verticesStorage.size();
        verticesStorage.resize(oldSize + 3 * 7); 
        offset = verticesStorage.begin() + oldSize;

        auto it = offset;
        for (size_t i = 0; i < vertices.size(); i += 3)
        {
            *it++ = vertices[i];     // x
            *it++ = vertices[i + 1]; // y
            *it++ = vertices[i + 2]; // z

            // временно выставляем белый цвет, потом setRGBColor заменит
            *it++ = 1.0f; // r
            *it++ = 1.0f; // g
            *it++ = 1.0f; // b
            *it++ = 1.0f; // a

        }

        setRGBColor(); // set color
    }


    virtual obj_t getType() const override
    {
        return objType;
    }
};

void updateVertices();


class General {
public:
    static GLFWwindow* window;
    static GLuint shader;

    // VBO (Vertex Buffer Object)
    // Хранит данные вершин (координаты, цвета, текстурные координаты и т.д.) в памяти GPU
    static GLuint VBO;

    // VAO (Vertex Array Object)
    // Хранит информацию о том, как данные из VBO следует интерпретировать и использовать.
    // VAO содержит указатели на VBO и информацию о разметке данных. 
    static GLuint VAO;

    static GLuint mvpLoc;
    static GLuint colorLoc;
    static glm::vec3 cameraPos; // положение камеры
    static glm::vec3 cameraFront; // куда смотрим
    static glm::vec3 cameraUp; // вверх камеры
    static glm::vec3 front;
    static float cameraSpeed; // скорость движения камеры

    static float yaw;   // вращение вокруг оси Y (влево/вправо)
    static float pitch;   // вращение вокруг оси X (вверх/вниз)
    static float sensitivity; // чувствительность мыши
    static float angle; // угол вращения

    static void init(const size_t width, const size_t height, const std::string& title)
    {
        // инициализация GLFW
        if (glfwInit() == GLFW_FALSE) throw std::runtime_error("GLFW initialization failed");

        // устанавливаем версию OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        // современный профиль
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        
        window = createWindow(width, height, title);

        // С помощью GLAD подключаем все нужные функции OpenGL (glfw3) по адресу в драйвере видеокарты
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            throw std::runtime_error("GLAD initialization failed");

        shader = createShader();
        mvpLoc = glGetUniformLocation(shader, "uMVP");
        colorLoc = glGetUniformLocation(shader, "uColor");

        // Привязка VAO и VBO к OpenGL
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // Привязываем VAO
        glBindVertexArray(VAO);

        // Привязываем VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        cameraPos   = glm::vec3(0.0f, 0.0f, 2.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
        front       = glm::normalize(cameraFront);
        cameraSpeed = 0.02f;
        yaw         = -90.0f;
        pitch       = 0.0f;
        sensitivity = 0.1f;
        angle       = 0.0f;
    }

    static glm::mat4 generateMVPMatrix()
    {
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0,1,0));
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

        glm::mat4 mvp = proj * view * model;
        return mvp;
    }

    static void redrawWindow(glm::mat4 mvp)
    {
        // очищаем цветовой буфер
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // передаем матрицу в шейдер
        glUseProgram(shader);
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, GLWrap::Object::getVerticesStorage().size() / 7);

        glfwSwapBuffers(GLWrap::General::window); // показать нарисованное
    }


    static void updateEvent()
    {
        glfwPollEvents(); // обработка событий
    }

    static void clean()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    static void enableMouseCameraMovement(){
        glfwSetCursorPosCallback(GLWrap::General::window, mouse_callback);
        glfwSetInputMode(GLWrap::General::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    };
    static void enableKeyboardSceneMovement()
    {
        glfwSetKeyCallback(GLWrap::General::window, key_callback);
    };
private:
    General() = delete;
    ~General() = delete;
    General(const General&) = delete;
    General& operator=(const General&) = delete;

    static GLuint createShader();
    static GLFWwindow* createWindow(const size_t width, const size_t height, const std::string& title);
    
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if(action == GLFW_PRESS || action == GLFW_REPEAT) {
            if(key == GLFW_KEY_W)
                GLWrap::General::cameraPos += GLWrap::General::cameraSpeed * 
                                            GLWrap::General::cameraFront;
            if(key == GLFW_KEY_S)
                GLWrap::General::cameraPos -= GLWrap::General::cameraSpeed *                    
                                            GLWrap::General::cameraFront;

            if(key == GLFW_KEY_A)
                GLWrap::General::cameraPos -= glm::normalize(glm::cross(GLWrap::General::cameraFront, GLWrap::General::cameraUp)) * GLWrap::General::cameraSpeed;

            if(key == GLFW_KEY_D)
                GLWrap::General::cameraPos += glm::normalize(glm::cross(GLWrap::General::cameraFront, GLWrap::General::cameraUp)) * GLWrap::General::cameraSpeed;
        }

        // выход
        if (key == GLFW_KEY_Q && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
    }


    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        static float lastX = 400, lastY = 300;
        static bool firstMouse = true;
        if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // обратный порядок, т.к. координаты окна сверху вниз
        lastX = xpos;
        lastY = ypos;

        xoffset *= GLWrap::General::sensitivity;
        yoffset *= GLWrap::General::sensitivity;

        GLWrap::General::yaw += xoffset;
        GLWrap::General::pitch += yoffset;

        // ограничиваем вертикальный угол, чтобы не перевернуть камеру
        if(GLWrap::General::pitch > 89.0f) GLWrap::General::pitch = 89.0f;
        if(GLWrap::General::pitch < -89.0f) GLWrap::General::pitch = -89.0f;

        // обновляем направление камеры
        glm::vec3 front;
        front.x = cos(glm::radians(GLWrap::General::yaw)) * cos(glm::radians(GLWrap::General::pitch));
        front.y = sin(glm::radians(GLWrap::General::pitch));
        front.z = sin(glm::radians(GLWrap::General::yaw)) * cos(glm::radians(GLWrap::General::pitch));
        GLWrap::General::cameraFront = glm::normalize(front);
    }
};

}; // namespace GLWrap

