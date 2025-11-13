#include "GLWrap.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace GLWrap
{

// ------------------ static variables ------------------
GLFWwindow* General::window = nullptr;
GLuint General::shader = 0;
GLuint General::VBO = 0;
GLuint General::VAO = 0;
GLuint General::mvpLoc = 0;
GLuint General::colorLoc = 0;

glm::vec3 General::cameraPos   = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 General::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 General::cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 General::front       = glm::vec3(0.0f, 0.0f, -1.0f);

float General::cameraSpeed = 0.02f;
float General::yaw = -90.0f;
float General::pitch = 0.0f;
float General::sensitivity = 0.1f;
float General::angle = 0.0f;

std::vector<float> Object::verticesStorage = {};



GLFWwindow* General::createWindow(const size_t width, const size_t height, const std::string& title)
{
    // создание нового окна
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!window) throw std::runtime_error("Window creation failed");

    glfwMakeContextCurrent(window);

    return window;
}

GLuint General::createShader()
{
    // Vertex shader with MVP matrix
    // in the vertex shader, the position of the vertex relative to the screen is calculated,
    // normals are computed (if necessary), and input data for other shaders is formed
    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec4 aColor;\n"
        "out vec4 vertexColor; // send to fragment shader\n"
        "uniform mat4 uMVP;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = uMVP * vec4(aPos, 1.0);\n"
        "    vertexColor = aColor;\n"
        "}";

    // Fragment shader
    // calculates the color of the fragment (pixel) that will be set in the current frame buffer
    const char* fragmentShaderSource =
        "#version 330 core\n"
        "in vec4 vertexColor; // get color from vertex shader\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vertexColor;\n"
        "}";

    // Создание шейдеров
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Создание шейдерной программы
    GLuint shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader;
}

void updateVertices()
{
    glBufferData(GL_ARRAY_BUFFER,
                 GLWrap::Object::getVerticesStorage().size() * sizeof(float),
                 GLWrap::Object::getVerticesStorage().data(),
                 GL_STATIC_DRAW);

    // Атрибут 0 — позиция (3 float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Атрибут 1 — цвет (4 float)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


}; // namespace GLWrap