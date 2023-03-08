#pragma once

#include <memory>

namespace utils
{
	namespace ui
	{
		class widget;
		class window;
		class dialog;
		class widget_factory;
		using window_ptr = std::shared_ptr<window>;
		using widget_ptr = std::shared_ptr<widget>;
		using dialog_ptr = std::shared_ptr<dialog>;
	}
}