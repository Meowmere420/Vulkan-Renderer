// Vulkan Renderer - vkassert.h
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

#ifndef _VKASSERT_H_
#define _VKASSERT_H_

#include <cstdlib>

// I fucking despise this warning, why is this shit enabled on /W3?
#pragma warning(disable: 26812)

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>

#ifdef _DEBUG
#define CHECK_VKRESULT(x) \
	if (x != VK_SUCCESS) \
	{ \
		__debugbreak(); \
	}
#else
#define CHECK_VKRESULT(x) \
	if (x != VK_SUCCESS) \
	{ \
		std::exit(-1); \
	}
#endif

#endif // !_VKASSERT_H_
