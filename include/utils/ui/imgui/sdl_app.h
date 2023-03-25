// 
// Application UI logic controller.

#pragma once

#include <utils/ui/app.h>
#include <math/vector_2d.h>
#include <imgui.h>

struct SDL_Renderer;
struct SDL_Window;

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class sdl_app : public utils::ui::app
			{
				using base = utils::ui::app;

			public:
				bool update(float dt) override final;

				void set_resolution(const utils::vec2i& resolution) { m_resolution = resolution; }
				void set_resolution(int x, int y) { m_resolution = { x, y }; }
				const utils::vec2i& get_resolution() const { return m_resolution; }
			
			public:
				static void request_keyboard();
				
			protected:
				virtual bool on_update(float dt) {
					return true;
				}
				int on_run() override final;
				virtual int init() {
					return 0;
				};

				void set_clear_color(const ImVec4& color) { m_clear_color = color; }
				const ImVec4& get_clear_color() const { return m_clear_color; }
				
			protected:
				virtual SDL_Window* create_window();
				SDL_Window* get_window() { return m_window; }

			protected:
				ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			private:
				SDL_Renderer* m_renderer = nullptr;
				SDL_Window* m_window = nullptr;
				utils::vec2i m_resolution = { 1024, 720 };
			};
		}
	}
}
