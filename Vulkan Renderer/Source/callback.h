// Vulkan Renderer - callback.h
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

#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <stdint.h>

class Callback
{
public:
	Callback();

	virtual ~Callback();

	virtual void onKeyPress(uint8_t keycode) {}

	virtual void onKeyRelease(uint8_t keycode) {}

	virtual void onCursorMove(int32_t deltaX, int32_t deltaY) {}

	virtual void onMouseMove(int32_t deltaX, int32_t deltaY) {}

	virtual void onMouseScroll(int32_t delta) {}

	virtual void onCharType(char character) {}

	virtual void onSetFocus() {}

	virtual void onKillFocus() {}

	virtual void onExit() {}
};

#endif // !_CALLBACK_H_
