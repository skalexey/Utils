#pragma once

#include <memory>
#include <utils/ui/qt/node.h>
#include <utils/ui/widget.h>

class QObject;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class widget : public virtual qt::node, public virtual ui::widget
			{
			public:
				widget(ui::node* parent = nullptr)
					: ui::node(parent)
					, qt::node(parent)
					, ui::widget(parent)
				{}
				vec2i get_screen_size() const override;

				widget* as_widget() override {
					return this;
				}
			};
			using widget_ptr = std::shared_ptr<qt::widget>;
		}
	}
}