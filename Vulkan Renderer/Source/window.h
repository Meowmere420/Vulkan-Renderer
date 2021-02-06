// Vulkan Renderer - window.h
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

#pragma once

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "windefines.h"

#include <stdint.h>

#include <memory>
#include <string_view>
#include <vector>

#include "surface.h"

class Window
{
public:
    Window(uint32_t clientWidth, uint32_t clientHeight, uint32_t posX, uint32_t posY, const std::wstring_view& title);

    ~Window();

    void resize(uint32_t clientWidth, uint32_t clientHeight);

    void setPosition(uint32_t posX, uint32_t posY);

    void setTitle(const std::wstring_view& title);

    void swapBuffers() const;

    static std::vector<const char*> getVulkanExtensions();

    const Surface& getSurface() const noexcept;

    uint32_t getPosX() const noexcept;

    uint32_t getPosY() const noexcept;

    uint32_t getWidth() const noexcept;

    uint32_t getHeight() const noexcept;
private:
    Swapchain* _swapchain;
    Surface*   _surface;
    HWND       _handle;
    HINSTANCE  _class;
    uint32_t   _posX;
    uint32_t   _posY;
    uint32_t   _clientWidth;
    uint32_t   _clientHeight;
};

#endif // !_WINDOW_H_
