#include <utils/ui/node.h>
#include <utils/ui/app.h>

namespace utils
{
	namespace ui
	{
		// All the dynamic casts require a fully defined type.
		ui::node::node(node* parent)
			: m_parent(parent)
		{
			m_app = dynamic_cast<ui::app*>(root());
		}
	}
}
