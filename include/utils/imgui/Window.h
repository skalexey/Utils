#pragma once

#include <utils/ui/Window.h>
#include "imgui.h"

namespace utils
{
	namespace imgui
	{
		class Window : public ui::Window
		{
			using base = ui::Window;
		public:
			Window() = default;
			Window(const math::vector2& position, const math::vector2& size)
				: base(position, size)
			{}
			void Show() override {
				// Set window position
				const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
				auto& s = main_viewport->Size;
				auto& p = main_viewport->Pos;
				auto& wp = main_viewport->WorkPos;
				ImGui::SetNextWindowPos(
					ImVec2(s.x * position().x, s.y * position().y)
					, ImGuiCond_FirstUseEver
				);
				ImGui::SetNextWindowSize(
					ImVec2(s.x * size().x, s.y * size().y)
					, ImGuiCond_FirstUseEver
				);

				// Create the window
				bool p_open = true;
				ImGuiWindowFlags window_flags = 0;
				if (!ImGui::Begin(Title(), &p_open, window_flags))
				{
					// Early out if the window is collapsed, as an optimization.
					ImGui::End();
					return;
				}
					
				onShow();

				// Close button 
				if (!p_open)
					Close();
					
				// End the window scope
				ImGui::End();
				return;
			}
		};

		using WindowPtr = std::unique_ptr<Window>;
	}
}