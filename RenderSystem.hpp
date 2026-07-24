#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Window.h"

namespace Rendering
{
	class RenderSystem
	{
	public:
		explicit RenderSystem(Core::Window& window);
		~RenderSystem() = default;

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		void BeginFrame();
		void EndFrame();
		void SetClearColor(float r, float g, float b, float a = 1.0f);

	private:
		Core::Window& m_Window;
		float m_ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	};
}

