#pragma once

#include <memory>
#include <utils/ui/imgui/fwd.h>
#include <utils/ui/widgets/text.h>
#include <utils/ui/imgui/widget.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class text : public imgui::widget, public utils::ui::text
			{
			public:
				using base = ui::text;

				text() = default;
				text(const std::string& text) : base(text) {}

				void on_show() override;

			protected:
				WIDGET_REGISTRATOR(imgui::widget_factory, text);
			};
			using text_ptr = std::shared_ptr<text>;
		}
	}
}