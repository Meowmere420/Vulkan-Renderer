// Vulkan Renderer - win32_window.cpp
// Copyright (c) 2020 Meowmere
//
// https://github.com/Meowmere420/Vulkan-Renderer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "surface.h"
#include "window.h"

class WindowClass
{
public:
    WindowClass()
        : _instance(GetModuleHandle(nullptr))
    {
        WNDCLASSEX wndClass;
        ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
        wndClass.cbSize        = sizeof(WNDCLASSEX);
        wndClass.style         = CS_OWNDC;
        wndClass.lpfnWndProc   = DefWindowProc;
        wndClass.cbClsExtra    = 0;
        wndClass.cbWndExtra    = 0;
        wndClass.hInstance     = _instance;
        wndClass.hIcon         = nullptr;
        wndClass.hCursor       = nullptr;
        wndClass.lpszMenuName  = nullptr;
        wndClass.lpszClassName = _name;
        wndClass.hIconSm       = nullptr;

        RegisterClassEx(&wndClass);
    }

    ~WindowClass()
    {
        UnregisterClass(_name, _instance);
    }

    constexpr const wchar_t* getName() noexcept
    {
        return _name;
    }

    HINSTANCE getInstance() const noexcept
    {
        return _instance;
    }
private:
    static constexpr const wchar_t* _name = L"Vulkan Engine Window";
    const HINSTANCE                 _instance;
};

Window::Window(uint32_t clientWidth, uint32_t clientHeight, uint32_t posX, uint32_t posY, const std::wstring_view& title)
    : _clientWidth(clientWidth), _clientHeight(clientHeight), _posX(posX), _posY(posY)
{
    static WindowClass windowClass;

    LONG windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
    RECT clientSize = { 0, 0, LONG(clientWidth), LONG(clientHeight) };
    AdjustWindowRect(&clientSize, windowStyle, FALSE);

    _handle = CreateWindowW(
        windowClass.getName(),
        title.data(),
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        clientSize.right - clientSize.left,
        clientSize.bottom - clientSize.top,
        NULL,
        NULL,
        windowClass.getInstance(),
        this
    );

    HRESULT hresult = GetLastError();
    if (hresult != S_OK)
    {
        __debugbreak();
    }

    _surface = new Surface(_handle, windowClass.getInstance());
    _swapchain = new Swapchain(_surface->getSurface());

    ShowWindow(_handle, SW_SHOWDEFAULT);
}

Window::~Window()
{
    delete _swapchain;
    delete _surface;
    DestroyWindow(_handle);
}

void Window::resize(uint32_t clientWidth, uint32_t clientHeight)
{
    _clientWidth = clientWidth;
    _clientHeight = clientHeight;
}

void Window::setPosition(uint32_t posX, uint32_t posY)
{
    _posX = posX;
    _posY = posY;
}

void Window::setTitle(const std::wstring_view& title)
{
    SetWindowText(_handle, title.data());
}

void Window::swapBuffers() const
{
}

std::vector<const char*> Window::getVulkanExtensions()
{
    std::vector<const char*> extensions;
    extensions.push_back("VK_KHR_surface");
    extensions.push_back("VK_KHR_win32_surface");
    return extensions;
}

const Surface& Window::getSurface() const noexcept
{
    return *_surface;
}

uint32_t Window::getPosX() const noexcept
{
    return _posX;
}

uint32_t Window::getPosY() const noexcept
{
    return _posY;
}

uint32_t Window::getWidth() const noexcept
{
    return _clientWidth;
}

uint32_t Window::getHeight() const noexcept
{
    return _clientHeight;
}
