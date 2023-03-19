#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <utils/ui/window.h>
#include <utils/ui/fwd.h>

namespace utils
{
	namespace ui
	{
		class dialog : public virtual window
		{
			using base = window;

		public:
			dialog()
			// We don't pass a factory into the constructor because of virtual inheritance,
			// so it should be set using set_factory() method
			{
				// Default size
				set_size(vec2i(500, 320));
				set_vertical_alignment(alignment::center);
				set_horizontal_alignment(alignment::center);
			}

			void set_factory(widget_factory& factory) { m_factory = &factory; }
			
			const widget_factory& get_factory() const {
				assert(m_factory && "Forgot to call set_factory()?");
				return *m_factory;
			}

			const std::string& get_title() const override {
				return m_title;
			}
			void set_title(const std::string& title) { m_title = title; }
			bool is_close_button_enabled() const { return m_use_close_button; }
			bool set_close_button_enabled(bool enabled) { m_use_close_button = enabled; }

		protected:
			void on_show() override {
				if (get_horizontal_alignment() == alignment::center && get_vertical_alignment() == alignment::center) {
					set_position_relative({ 0.5f, 0.5f }, { 0.5f, 0.5f });
				}
				base::on_show();
			};

		private:
			widget_factory* m_factory = nullptr;
			std::string m_title = "Dialog";
			bool m_use_close_button = false;
		};
		// TODO: remove it as it is declared in fwd.h
		using dialog_ptr = std::shared_ptr<dialog>;
	}
}