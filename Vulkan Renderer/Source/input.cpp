// Vulkan Renderer - input.cpp
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

#include "input.h"
#include "window.h"

void Input::registerCallback(Callback* callback)
{
	_callbacks.emplace_back(callback);
}

void Input::unregisterCallback(Callback* callback)
{
	_callbacks.remove(callback);
}

bool Input::isKeyDown(uint8_t keycode) noexcept
{
    return _keystates[keycode];
}

int32_t Input::getCursorX() noexcept
{
    return _cursorX;
}

int32_t Input::getCursorY() noexcept
{
    return _cursorY;
}

LRESULT Input::processMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		for (Callback* callback : _callbacks)
			callback->onExit();
		PostQuitMessage(0);
		return 0;
	}
	case WM_KILLFOCUS:
	{
		Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		for (Callback* callback : _callbacks)
			callback->onKillFocus();
		_keystates.reset();
		return 0;
	}
	case WM_SETFOCUS:
	{
		Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		for (Callback* callback : _callbacks)
			callback->onSetFocus();
		return 0;
	}
	case WM_KEYDOWN:
	{
		uint8_t keycode = static_cast<uint8_t>(wparam);
		_keystates[keycode] = true;
		for (Callback* callback : _callbacks)
			callback->onKeyPress(keycode);
		return 0;
	}
	case WM_KEYUP:
	{
		uint8_t keycode = static_cast<uint8_t>(wparam);
		_keystates[keycode] = false;
		for (Callback* callback : _callbacks)
			callback->onKeyRelease(keycode);
		return 0;
	}
	case WM_SYSKEYDOWN:
	{
		uint8_t keycode = static_cast<uint8_t>(wparam);
		_keystates[keycode] = true;
		for (Callback* callback : _callbacks)
			callback->onKeyPress(keycode);
		return 0;
	}
	case WM_SYSKEYUP:
	{
		uint8_t keycode = static_cast<uint8_t>(wparam);
		_keystates[keycode] = false;
		for (Callback* callback : _callbacks)
			callback->onKeyRelease(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		for (Callback* callback : _callbacks)
			callback->onCharType(static_cast<char>(wparam));
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		POINTS pts = MAKEPOINTS(lparam);
		for (Callback* callback : _callbacks)
			callback->onCursorMove(pts.x - _cursorX, pts.y - _cursorY);
		_cursorX = pts.x;
		_cursorY = pts.y;
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int32_t delta = static_cast<uint32_t>(GET_WHEEL_DELTA_WPARAM(wparam));
		while (delta >= WHEEL_DELTA)
		{
			for (Callback* callback : _callbacks)
				callback->onMouseScroll(1);
			delta -= WHEEL_DELTA;
		}
		while (-delta >= WHEEL_DELTA)
		{
			for (Callback* callback : _callbacks)
				callback->onMouseScroll(-1);
			delta += WHEEL_DELTA;
		}
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		_keystates[VK_LBUTTON] = true;
		for (Callback* callback : _callbacks)
			callback->onKeyPress(VK_LBUTTON);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		_keystates[VK_LBUTTON] = false;
		for (Callback* callback : _callbacks)
			callback->onKeyRelease(VK_LBUTTON);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		_keystates[VK_RBUTTON] = true;
		for (Callback* callback : _callbacks)
			callback->onKeyPress(VK_RBUTTON);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		_keystates[VK_RBUTTON] = false;
		for (Callback* callback : _callbacks)
			callback->onKeyRelease(VK_RBUTTON);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		_keystates[VK_MBUTTON] = true;
		for (Callback* callback : _callbacks)
			callback->onKeyPress(VK_MBUTTON);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		_keystates[VK_MBUTTON] = false;
		for (Callback* callback : _callbacks)
			callback->onKeyRelease(VK_MBUTTON);
		return 0;
	}
	case WM_XBUTTONDOWN:
	{
		uint8_t keycode = VK_MBUTTON + GET_XBUTTON_WPARAM(wparam);
		_keystates[keycode] = true;
		for (Callback* callback : _callbacks)
			callback->onKeyPress(keycode);
		return 0;
	}
	case WM_XBUTTONUP:
	{
		uint8_t keycode = VK_MBUTTON + GET_XBUTTON_WPARAM(wparam);
		_keystates[keycode] = false;
		for (Callback* callback : _callbacks)
			callback->onKeyRelease(keycode);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}
