module;

#include "geo.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
    
#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <typeindex>

import color;
import camera;
import input;

export module render;

export namespace OGLWrap {

class IOGLMesh {
public:
    virtual ~IOGLMesh() = default;

    virtual std::vector<float> getVertexes() = 0;
    virtual size_t getVertexCount() = 0;
    virtual OGLWrap::RGB_color getColor() = 0;
    virtual bool isDynamic() = 0;
};

class TriangleMesh final : public IOGLMesh {
    Geo::Triangle triangle_;
    OGLWrap::RGB_color color_ = OGLWrap::RGB_color{255, 255, 255};
    bool isDynamic_ = false;
    
public:
    TriangleMesh(const Geo::Triangle& triangle, 
                 const OGLWrap::RGB_color& color = OGLWrap::RGB_color{255, 255, 255},
                 bool isDynamic = false) 
        : triangle_(triangle), color_(color), isDynamic_(isDynamic) {}

    std::vector<float> getVertexes() override {
        std::vector<float> vertices;
        for (int i = 0; i < 3; ++i) {
            auto point = triangle_.getPoint(i);
            vertices.push_back(point.x_);
            vertices.push_back(point.y_);
            vertices.push_back(point.z_);
        }
        return vertices;
    }

    OGLWrap::RGB_color getColor() override {
        return color_;
    }

    bool isDynamic() override {
        return isDynamic_;
    }

    size_t getVertexCount() override {
        return 3;
    }
};

class MeshManager final {

    class BucketData final {
    public:
        std::type_index type_;
        GLuint VBO_ = 0;
        GLuint VAO_ = 0;
        size_t vertexDataSize = 0;
        
        std::vector<std::weak_ptr<IOGLMesh>> meshes_;
        std::vector<float> draftVBO;

        BucketData() : type_(typeid(void)) {}
    };

    std::vector<BucketData> storage_;

    void addToBucket(std::vector<BucketData>::iterator bucketIter, 
                     const std::shared_ptr<IOGLMesh>& mesh) {
        
        auto vertices = mesh->getVertexes();
        size_t vertexCount = mesh->getVertexCount();
        auto color = mesh->getColor();

        // Убедимся, что vertices содержит правильное количество элементов
        if (vertices.size() < vertexCount * 3) {
            return; // Недостаточно данных
        }

        // Добавляем вершины и цвет в draftVBO
        for (size_t i = 0; i < vertices.size(); i += 3) {
            // Позиция вершины
            bucketIter->draftVBO.push_back(vertices[i]);
            bucketIter->draftVBO.push_back(vertices[i + 1]);
            bucketIter->draftVBO.push_back(vertices[i + 2]);

            // Цвет вершины
            bucketIter->draftVBO.push_back(color.rFloat());
            bucketIter->draftVBO.push_back(color.gFloat());
            bucketIter->draftVBO.push_back(color.bFloat());
            bucketIter->draftVBO.push_back(color.aFloat());
        }

        // Обновляем данные в GPU
        glBindBuffer(GL_ARRAY_BUFFER, bucketIter->VBO_);
        glBufferData(GL_ARRAY_BUFFER, 
                     bucketIter->draftVBO.size() * sizeof(float), 
                     bucketIter->draftVBO.data(), 
                     mesh->isDynamic() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        
        bucketIter->vertexDataSize = bucketIter->draftVBO.size() / 7; // 7 элементов на вершину
        bucketIter->meshes_.push_back(mesh);
    }

    void setupBucket(std::vector<BucketData>::iterator bucketIter) {
        glGenVertexArrays(1, &bucketIter->VAO_);
        glGenBuffers(1, &bucketIter->VBO_);
        glBindVertexArray(bucketIter->VAO_);
        glBindBuffer(GL_ARRAY_BUFFER, bucketIter->VBO_);

        // Позиции (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                            7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Цвета (location = 1)
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 
                            7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
    }

public:
    void addMesh(const std::shared_ptr<IOGLMesh>& mesh) {
        if (!mesh) return;

        auto foundedBucketIter = std::find_if(
            storage_.begin(), 
            storage_.end(),
            [&mesh](const BucketData& bucket) {
                return bucket.type_ == typeid(*mesh);
            }
        );

        if (foundedBucketIter != storage_.end()) {
            addToBucket(foundedBucketIter, mesh);
        } else {
            BucketData newBucket;
            newBucket.type_ = std::type_index(typeid(*mesh));
            storage_.push_back(std::move(newBucket));
            
            auto newBucketIter = storage_.end() - 1;
            setupBucket(newBucketIter);
            addToBucket(newBucketIter, mesh);
        }
    }

    void render() const {
        for (const auto& bucket : storage_) {
            if (bucket.vertexDataSize == 0) continue;
            
            glBindVertexArray(bucket.VAO_);
            glDrawArrays(GL_TRIANGLES, 0, bucket.vertexDataSize);
            glBindVertexArray(0);
        }
    }

    ~MeshManager() {
        for (auto& bucket : storage_) {
            if (bucket.VBO_) glDeleteBuffers(1, &bucket.VBO_);
            if (bucket.VAO_) glDeleteVertexArrays(1, &bucket.VAO_);
        }
    }
};

class Shader final {
private:
    GLuint shader_ = 0;

    const char* vertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec4 aColor;\n"
        "out vec4 vertexColor;\n"
        "uniform mat4 uMVP;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = uMVP * vec4(aPos, 1.0);\n"
        "    vertexColor = aColor;\n"
        "}";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "in vec4 vertexColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vertexColor;\n"
        "}";

public:
    Shader() {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shader_ = glCreateProgram();
        glAttachShader(shader_, vertexShader);
        glAttachShader(shader_, fragmentShader);
        glLinkProgram(shader_);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader() {
        if (shader_) {
            glDeleteProgram(shader_);
        }
    }

    void use() const {
        glUseProgram(shader_);
    }

    void setMVP(const glm::mat4& mvp) const {
        GLint mvpLoc = glGetUniformLocation(shader_, "uMVP");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);
    }
};

class IRender {
public:
    virtual void render() = 0;
    virtual void addMesh(const std::shared_ptr<IOGLMesh>& mesh) = 0;
    virtual void setViewMatrix(const glm::mat4& viewMatrix) = 0;
    virtual void setModelMatrix(const glm::mat4& modelMatrix) = 0;
    virtual void setProjectionMatrix(const glm::mat4& projectionMatrix) = 0;
    virtual ~IRender() = default;
};

class OGLRender final : public IRender {
private:
    Shader shader_;
    MeshManager meshManager_;
    
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        800.0f/600.0f,
        0.1f,
        100.0f
    );
    
public:
    OGLRender() = default;
    
    void addMesh(const std::shared_ptr<IOGLMesh>& mesh) override {
        meshManager_.addMesh(mesh);
    }

    void setViewMatrix(const glm::mat4& viewMatrix) override {
        view = viewMatrix;
    }

    void setModelMatrix(const glm::mat4& modelMatrix) override {
        model = modelMatrix;
    }

    void setProjectionMatrix(const glm::mat4& projectionMatrix) override {
        projection = projectionMatrix;
    }

    void render() override {
        auto mvp = projection * view * model;

        shader_.use();
        shader_.setMVP(mvp);
        meshManager_.render();
    }
};

}; // namespace OGLWrap