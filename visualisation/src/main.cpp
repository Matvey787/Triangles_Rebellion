#include "GLWrap.hpp"

#include <iostream>


int main() {

    GLWrap::General::init(800, 600, "Triangles Intersection Visualization");

    GLWrap::General::enableMouseCameraMovement();
    GLWrap::General::enableKeyboardSceneMovement();

    // glClearColor(0, 1, 1, 1);

    GLWrap::Triangle triangle1{{-0.8f, -0.1f, -1.0f,
                                -0.6f, -0.1f, 0.0f,
                                -0.7f,  0.1f, 0.0f}};
    triangle1.setRGBColor(255, 0, 0); // set color to red

    GLWrap::Triangle triangle2{{0.6f, -0.1f, 0.0f,
                                0.8f, -0.1f, 0.0f,
                                0.7f,  0.1f, 0.0f}};

    GLWrap::updateVertices();


    while (!glfwWindowShouldClose(GLWrap::General::window))
    {

        // Create MVP matrix: model (rotation), view (camera), projection (perspective)
        glm::mat4 mvp = GLWrap::General::generateMVPMatrix();

        GLWrap::General::redrawWindow(mvp);

        GLWrap::General::updateEvent();
    }

    GLWrap::General::clean();

    return 0;
}

