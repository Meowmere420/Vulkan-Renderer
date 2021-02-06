// Vulkan Renderer - input.h
//
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

#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdint.h>

#include <bitset>
#include <list>

#include "callback.h"
#include "windefines.h"

class Input
{
    friend class Window;
public:
    static void registerCallback(Callback* callback);

    static void unregisterCallback(Callback* callback);
    
    static bool isKeyDown(uint8_t keycode) noexcept;

    static int32_t getCursorX() noexcept;

    static int32_t getCursorY() noexcept;
private:
    static LRESULT CALLBACK processMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
    static inline std::list<Callback*> _callbacks;
    static inline std::bitset<256>     _keystates;
    static inline int32_t              _cursorX;
    static inline int32_t              _cursorY;
};

#endif // !_INPUT_H_
