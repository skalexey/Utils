#pragma once

#include <memory>
#include <string>
#include <utils/ui/imgui/fwd.h>
#include <utils/ui/widgets/label.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class label : public imgui::widget, public ui::label
			{
			public:
				using base = ui::label;

				void set_text(const std::string& text) {
					m_text = text;
				}

				const std::string& get_text() const {
					return m_text;
				}

			protected:
				bool imgui_label_update(float dt);
				virtual bool on_imgui_label_update(float dt) {
					return true;
				}
				
			private:
				bool on_update(float dt) override final;
				WIDGET_REGISTRATOR(imgui::widget_factory, label);

			private:
				std::string m_text;
			};
			
			using label_ptr = std::shared_ptr<label>;
		}
	}
}