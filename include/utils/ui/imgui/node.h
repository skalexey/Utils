// This class just adds the ability to work with nodes
// as a imgui::node* instead of a ui::node*

#pragma once

#include <utils/ui/imgui/fwd.h>
#include <utils/ui/node.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			// Hide ui::node* interface using protected inheritance
			class node : protected virtual ui::node, public std::enable_shared_from_this<imgui::node>
			{
				using base = ui::node;

			public:
				// TODO: support constructor with imgui::node* 
				node(ui::node* parent = nullptr);

				void set_parent(imgui::node* parent) {
					base::set_parent(parent);
				}

				const imgui::node* get_parent() const {
					return dynamic_cast<const imgui::node*>(base::get_parent());
				}
				
				imgui::node* parent() {
					return dynamic_cast<imgui::node*>(base::parent());
				}

				imgui::node* root() {
					return dynamic_cast<imgui::node*>(base::root());
				}
				
				// All the dynamic casts require a fully defined type.
				// So we define them in the cpp file.
				imgui::app& app();
				const imgui::app& get_app() const;
			};
			using node_ptr = std::shared_ptr<imgui::node>;
		}
	}
}