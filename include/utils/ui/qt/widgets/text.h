#pragma once

#include <memory>
#include <utils/ui/qt/fwd.h>
#include <utils/ui/widgets/text.h>
#include <utils/ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class text : public qt::widget, public utils::ui::text
			{
			public:
				using base = ui::text;

				text(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, qt::widget(parent)
				{};

				text(ui::node* parent, const std::string& text)
					: ui::node(parent)
					, base(parent, text)
					, qt::widget(parent)
				{}

				void on_show() override;

			protected:
				WIDGET_REGISTRATOR(qt::widget_factory, text);
			};
			using text_ptr = std::shared_ptr<text>;
		}
	}
}