module;

#include <map>
#include <string>

export module color;

export namespace OGLWrap {

class RGB_color final {
public:
    unsigned char r, g, b, a;
    
    RGB_color(unsigned char r, unsigned char g, unsigned char b, 
              unsigned char a = 255) 
        : r(r), g(g), b(b), a(a) {}
    
    float rFloat() const { return r / 255.0f; }
    float gFloat() const { return g / 255.0f; }
    float bFloat() const { return b / 255.0f; }
    float aFloat() const { return a / 255.0f; }
};

class ColorFactory {
public:
    static RGB_color fromName(std::string name)
    {
        static std::map<std::string, RGB_color> colors = {
            {"black", RGB_color(0, 0, 0)},
            {"red",   RGB_color(255, 0, 0)},
            {"green", RGB_color(0, 255, 0)},
            {"blue",  RGB_color(0, 0, 255)}
        };

        return colors.at(name);
    }
};

}; // namespace OGLWrap
