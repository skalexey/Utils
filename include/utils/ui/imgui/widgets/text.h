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
			class text : public imgui::widget, public ui::text
			{
			public:
				using base = ui::text;

				text(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, imgui::widget(parent)
				{};

				text(ui::node* parent, const std::string& text)
					: ui::node(parent)
					, base(parent, text)
					, imgui::widget(parent)
				{}

				void on_show() override;

			protected:
				WIDGET_REGISTRATOR(imgui::widget_factory, text);
			};
			using text_ptr = std::shared_ptr<text>;
		}
	}
}