#pragma once

#include <memory>
#include <utils/ui/qt/fwd.h>
#include <utils/ui/widgets/label.h>
#include <utils/ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class label : public qt::widget, public utils::ui::label
			{
			public:
				using base = ui::label;

				label(ui::node* parent = nullptr)
					: ui::node(parent)
					, base(parent)
					, qt::widget(parent)
				{}

				label(ui::node* parent, const std::string& label)
					: ui::node(parent)
					, base(parent, label)
					, qt::widget(parent)
				{}

				void on_show() override;

			protected:
				WIDGET_REGISTRATOR(qt::widget_factory, label);
			};
			
			using label_ptr = std::shared_ptr<label>;
		}
	}
}