#pragma once

#include "brpch.h"

// #include "Brigerad.h"
#include "Brigerad/Core/Core.h"
#include "Brigerad/Events/Event.h"

namespace Brigerad
{
struct WindowProps
{
    std::string title;
    unsigned int width;
    unsigned int height;

    WindowProps(const std::string &t = "Brigerad Engine",
                unsigned int w = 1280,
                unsigned int h = 720)
        : title(t), width(w), height(h)
    {
    }
};

// Interface representing a desktop system based window.
class BRIGERAD_API Window
{
public:
    using EventCallbackFn = std::function<void(Event &)>;

    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    // Window attributes.
    virtual void SetEventCallback(const EventCallbackFn &callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void *GetNativeWindow() const = 0;

    static Window *Create(const WindowProps &props = WindowProps());
};
} // namespace Brigerad
