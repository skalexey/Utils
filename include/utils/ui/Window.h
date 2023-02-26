#pragma once

#include <memory>
#include <functional>
#include <string>
#include <math/vector_2d.h>

namespace utils
{
	namespace ui
	{
		class Window
		{
			public:
				Window() : m_position(0, 0), m_size(512, 512)
				{}
				Window(const math::vector2& position, const math::vector2& size)
					: m_position(position), m_size(size)
				{}

				virtual void Show() {
					onShow();
				}

				void Close() {
					if (m_on_close)
						m_on_close();
					onClose();
				}

				void SetOnShow(const std::function<void()>& on_show) {
					m_on_show = on_show;
				}

				void SetOnClose(const std::function<void()>& on_close) {
					m_on_close = on_close;
				}

				virtual const char* Title() const {
					static std::string default_title = "Window";
					return default_title.c_str();
				};

			protected:
				virtual void onClose() {};
				virtual void onShow() {
					if (m_on_show)
						m_on_show();
				};

			protected:
				math::vector2& position() { return m_position; }
				math::vector2& size() { return m_size; }

			private:
				math::vector2 m_position;
				math::vector2 m_size;
				std::function<void()> m_on_close;
				std::function<void()> m_on_show;
		};

		using WindowPtr = std::unique_ptr<Window>;
	}
}