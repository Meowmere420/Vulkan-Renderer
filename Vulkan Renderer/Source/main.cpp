// Vulkan Renderer - main.cpp
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

#include <chrono>
#include <thread>
#include <iostream>

#include "callback.h"
#include "graphics.h"
#include "input.h"
#include "pipeline.h"
#include "print_device_info.h"
#include "shader.h"
#include "window.h"

int main()
{
	Graphics::Create();

	{
		Window window(800, 600, 0, 0, L"Window");

		Shader vertex_shader("Source/Shaders/vertex_shader.spv");
		Shader fragment_shader("Source/Shaders/fragment_shader.spv");

		Pipeline pipeline(vertex_shader, fragment_shader);

		while (true)
		{
			static MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					goto end;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
end:
	std::cin.get();
	Graphics::Destroy();
}
