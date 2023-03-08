#pragma once

#include <memory>
#include <functional>
#include <string>
#include <math/vector_2d.h>

namespace utils
{
	namespace ui
	{
		using vec2 = math::vector2;

		class widget
		{
			public:
				using on_show_cb = std::function<void()>;
				using on_hide_cb = std::function<void()>;

				enum alignment : int
				{
					none = 0
					, center
					, left
					, right
					, top
					, bottom
				};

				bool show() {
					m_is_visible = true;
					on_show();
					return m_is_visible;
				}

				void hide() {
					m_is_visible = false;
					if (m_on_hide)
						m_on_hide();
					on_hide();
				}

				void set_visible(bool visible) {
					if (!visible && m_is_visible)
						hide();
					else if (!m_is_visible && visible)
						// show() should be called explicitly because it is supposed to be called in every frame
						m_is_visible = true;
				}
				bool is_visible() const { return m_is_visible; }
				void set_on_show(const on_show_cb& on_show) { m_on_show = on_show; }
				void set_on_hide(const on_hide_cb& on_hide) { m_on_hide = on_hide; }

				virtual const std::string& get_title() const {
					static std::string default_title = "widget";
					return default_title;
				};

				const vec2& get_position() { return m_position; }
				const vec2& get_size() { return m_size; }
				void set_position(const vec2& pos ) { m_position = pos; }
				void set_position(const vec2& pos, const vec2& anchor ) {
					m_position.x = pos.x - m_size.x * anchor.x;
					m_position.y = pos.y - m_size.y * anchor.y;
				}
				void set_size(const vec2& size) { m_size = size; }
				void set_vertical_alignment(const alignment& align) { m_vertical_alignment = align; }
				void set_horizontal_alignment(const alignment& align) { m_horizontal_alignment = align; }
				const alignment& get_vertical_alignment() { return m_vertical_alignment; }
				const alignment& get_horizontal_alignment() { return m_horizontal_alignment; }

			protected:
				virtual void on_hide() {};
				virtual void on_show() {
					if (m_on_show)
						m_on_show();
				};

			private:
				vec2 m_position;
				vec2 m_size;
				alignment m_vertical_alignment = alignment::none;
				alignment m_horizontal_alignment = alignment::none;
				on_hide_cb m_on_hide;
				on_show_cb m_on_show;
				bool m_is_visible = true;
		};

		using widget_ptr = std::shared_ptr<widget>;
	}
}