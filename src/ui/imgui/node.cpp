#include <utils/ui/imgui/app.h>
#include <utils/ui/imgui/widget_factory.h>
#include <utils/ui/imgui/node.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			imgui::node::node(ui::node* parent)
				: base(parent)
			{
				set_factory(imgui::widget_factory::instance());
			}
			
			// All the dynamic casts require a fully defined type.
			imgui::app& imgui::node::app() const
			{
				return dynamic_cast<imgui::app&>(base::app());
			}

			const imgui::app& imgui::node::get_app() const
			{
				return dynamic_cast<const imgui::app&>(base::get_app());
			}
		}
	}
}