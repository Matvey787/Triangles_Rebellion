#include <iostream>
#include "OGLWrap/OGLWrap.hpp"

int main() {
    // Создаем треугольник

    // Создаем первое окно
    std::unique_ptr<OGLWrap::Win3D> w1 = OGLWrap::Win3DBuilder()
                        .setWidth(800)
                        .setHeight(600)
                        .setTitle("First Window")
                        .setBgColor(OGLWrap::ColorFactory::fromName("black"))
                        .setStart_posX(100)
                        .setStart_posY(100)
                        .build();
    
    for (size_t i = 0; i < 100000; i++) {

        Geo::Point p1(0.0f+ 0.5*i, 0.0f + 0.5*i, 0.0f + 0.5*i);
        Geo::Point p2(1.0f+ 0.5*i, 0.0f + 0.5*i, 0.0f + 0.5*i);
        Geo::Point p3(0.0f+ 0.5*i, 1.0f+ 0.5*i , 0.0f + 0.5*i);
        Geo::Triangle triangle(p1, p2, p3);
        // Добавляем треугольник в окно
        w1->addTriangle(triangle);
    }
    
    auto w2 = OGLWrap::Win3DBuilder()
                    .setWidth(400)
                    .setHeight(300)
                    .setTitle(std::string("Second Window"))
                    .setBgColor(OGLWrap::ColorFactory::fromName("blue"))
                    .setStart_posX(950)
                    .setStart_posY(100)
                    .build();

    // Главный цикл
    while (true) {
        w1->render();
        // w2->render();
    }
    
    return 0;
}
