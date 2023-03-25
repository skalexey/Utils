#pragma once

#include <memory>
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
				using base = utils::ui::text;

			public:
				text() = default;
				text(const std::string& text) : base(text) {}

				void on_show() override;
			};
			using text_ptr = std::unique_ptr<text>;
		}
	}
}