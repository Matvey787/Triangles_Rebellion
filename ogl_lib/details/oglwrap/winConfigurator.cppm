module;

#include <unistd.h>
#include <memory>
#include <string>

import window;
import color;

export module winConfigurator;

export namespace OGLWrap {

class IBuilder {
public:
    virtual IBuilder& setWidth(size_t width) = 0;
    virtual IBuilder& setHeight(size_t height) = 0;
    virtual IBuilder& setTitle(std::string& title) = 0;
    virtual IBuilder& setBgColor(RGB_color color) = 0;
    virtual IBuilder& setStart_posX(size_t x) = 0;
    virtual IBuilder& setStart_posY(size_t y) = 0;
    virtual ~IBuilder() = default;
};

class Win3DBuilder : public IBuilder {
    WinConfig config_;

public:
    Win3DBuilder& setWidth(size_t width) override {
        config_.width = width;
        return *this;
    }
    Win3DBuilder& setHeight(size_t height) override {
        config_.height = height;
        return *this;
    }
    Win3DBuilder& setTitle(std::string& title) override {
        config_.title = title;
        return *this;
    }
    Win3DBuilder& setTitle(std::string&& title) {
        config_.title = std::move(title);
        return *this;
    }
    Win3DBuilder& setBgColor(RGB_color color) override {
        config_.bgColor = color;
        return *this;
    }
    Win3DBuilder& setStart_posX(size_t x) override {
        config_.start_posX = x;
        return *this;
    }
    Win3DBuilder& setStart_posY(size_t y) override {
        config_.start_posY = y;
        return *this;
    }
    std::unique_ptr<Win3D> build() {
        return std::make_unique<Win3D>(config_);
    }

};


}; // namespace OGLWrap