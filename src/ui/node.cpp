#include <utils/ui/node.h>
#include <utils/ui/app.h>

namespace utils
{
	namespace ui
	{
		// All the dynamic casts require a fully defined type.
		void ui::node::add_node(const node_ptr& child)
		{
			// This code is called from parent to avoid having a potentially called method
			child->m_parent = this;
			child->m_app = dynamic_cast<ui::app*>(child->root());
			m_children.push_back(child);
		}
	}
}
