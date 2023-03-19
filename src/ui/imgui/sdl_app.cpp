// 
// Application UI logic controller.

#pragma once

#include <chrono>
#include <SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <utils/ui/imgui/sdl_app.h>

#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			int sdl_app::on_run() {

				// Setup SDL
				// (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
				// depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
				
				if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
				{
					printf("Error: %s\n", SDL_GetError());
					return -1;
				}

				SDL_DisplayMode DM;
				SDL_GetCurrentDisplayMode(0, &DM);
				auto Width = DM.w;
				auto Height = DM.h;
				SDL_Log("Display: %dx%d", Width, Height);

				m_window = create_window();

				// Setup SDL_Renderer instance
				m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
				if (m_renderer == NULL)
				{
					SDL_Log("Error creating SDL_Renderer!");
					return -2;
				}

				SDL_RenderSetLogicalSize(m_renderer, m_resolution.x, m_resolution.y);

				//SDL_RendererInfo info;
				//SDL_GetRendererInfo(renderer, &info);
				//SDL_Log("Current SDL_Renderer: %s", info.name);

				// Setup Dear ImGui context
				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO(); (void)io;
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
				//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

				// Setup Dear ImGui style
				ImGui::StyleColorsDark();
				//ImGui::StyleColorsLight();

				// Setup Platform/Renderer backends
				ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
				ImGui_ImplSDLRenderer_Init(m_renderer);

				// Load Fonts
				// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
				// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
				// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
				// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
				// - Read 'docs/FONTS.md' for more instructions and details.
				// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
				//io.Fonts->AddFontDefault();
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
				//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
#ifdef ANDROID
//  ImFontConfig font_cfg;
//         font_cfg.FontDataOwnedByAtlas = false;
//         font_cfg.MergeMode = false;
//         font_cfg.MergeGlyphCenterV = true;
//         io.Fonts->AddFontFromMemoryTTF(Roboto_TTF_Data, RobotoFont_TTF_DataSize, 18.0f, &font_cfg, io.Fonts->GetGlyphRangesCyrillic());
				// ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
#else

				ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 28.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
#endif
				//IM_ASSERT(font != NULL);
				//io.Fonts->AddFontFromFileTTF("NotoSansCJKjp-Medium.otf", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());


				// io.FontGlobalScale = 1.5f;
				// ImGuiStyle& style = ImGui::GetStyle();
				// style.ScaleAllSizes(1.5f);
				// Initial code
				auto ret = init();

				// Main loop
				if (ret == 0)
				{
					using namespace std::chrono;
					long long ns = 0;
					auto start = steady_clock::now();
					do
					{
						auto end = steady_clock::now();
						ns = duration_cast<nanoseconds>(end - start).count();
						start = end;
					} while (update(float(ns) * float(1e-9)));
				}
							
				// Cleanup
				ImGui_ImplSDLRenderer_Shutdown();
				ImGui_ImplSDL2_Shutdown();
				ImGui::DestroyContext();

				SDL_DestroyRenderer(m_renderer);
				SDL_DestroyWindow(m_window);
				SDL_Quit();

				return ret;
			}

			bool sdl_app::update(float dt) {
				assert(m_renderer);
				// Poll and handle events (inputs, window resize, etc.)
				// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
				// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
				// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
				// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
				SDL_Event event;
				bool active = true;
				while (SDL_PollEvent(&event))
				{
					ImGui_ImplSDL2_ProcessEvent(&event);
					if (event.type == SDL_QUIT)
						active = false;
					if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_window))
						active = false;
				}

				// Start the Dear ImGui frame
				ImGui_ImplSDLRenderer_NewFrame();
				ImGui_ImplSDL2_NewFrame();

				ImGui::NewFrame();

				if (active)
				{
					base::update(dt);
					active = on_update(dt);
				}

				// Rendering
				ImGui::Render();
				SDL_SetRenderDrawColor(m_renderer, (Uint8)(m_clear_color.x * 255), (Uint8)(m_clear_color.y * 255), (Uint8)(m_clear_color.z * 255), (Uint8)(m_clear_color.w * 255));
				SDL_RenderClear(m_renderer);
				ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
				SDL_RenderPresent(m_renderer);
					
				return active;
			}

			SDL_Window* ui::imgui::sdl_app::create_window()
			{
				SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
				return SDL_CreateWindow("ImGui SDL Application", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_resolution.x, m_resolution.y, window_flags);
			}
		}
	}
}