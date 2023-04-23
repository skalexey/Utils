#include <utils/ui/app.h>
#include <utils/ui/window.h>
#include <utils/ui/window_controller.h>

namespace utils
{
	namespace ui
	{
		bool window_controller::show() {
			assert(m_view);
			return m_view->show();
		}

		const widget_factory& window_controller::get_factory() const {
			return m_app.get_factory();
		}
	}
}