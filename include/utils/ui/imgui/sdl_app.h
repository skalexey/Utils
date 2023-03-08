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
				using vec2 = math::vector_2d<int>;

			public:
				bool update(float dt) override final;

			protected:
				virtual bool on_update(float dt) {
					return true;
				}
				int on_run() override final;
				virtual int init() {
					return 0;
				};

			protected:
				virtual SDL_Window* create_window();

			protected:
				ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			private:
				SDL_Renderer* m_renderer = nullptr;
				SDL_Window* m_window = nullptr;
			};
		}
	}
}
