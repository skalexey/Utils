#pragma once

#include <memory>
#include <utils/ui/qt/fwd.h>
#include <utils/ui/widgets/button.h>
#include <utils/ui/qt/widget.h>

class QQuickButton;

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			class button : public qt::widget, public ui::button
			{
			public:
				using base = ui::button;

				button(ui::node* parent = nullptr); 

				void on_show() override;

				const vec2i& text_size();

				const vec2i& get_size() {
					return m_calculated_size;
				}

				void on_set_text(const std::string& text) override;
				void on_set_on_click(const on_click_t& on_click) override;

			private:
				vec2i m_calculated_size;
				WIDGET_REGISTRATOR(qt::widget_factory, qt::button);
				button_model* m_model = nullptr;
			};
			using button_ptr = std::shared_ptr<button>;
		}
	}
}
