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
				set_size({ 300, 200 });
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

		protected:
			void on_show() override {
				if (get_horizontal_alignment() == alignment::center && get_vertical_alignment() == alignment::center) {
					set_position({0.5f, 0.5f}, {0.5f, 0.5f});
				}
				base::on_show();
			};

		private:
			widget_factory* m_factory = nullptr;
			std::string m_title = "Dialog";

		};
		// TODO: remove it as it is declared in fwd.h
		using dialog_ptr = std::shared_ptr<dialog>;
	}
}