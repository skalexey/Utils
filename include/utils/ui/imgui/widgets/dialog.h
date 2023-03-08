#pragma once

#include <utils/ui/widgets/dialog.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class dialog : virtual public ui::dialog
			{
				using base = ui::dialog;

			public:
				// We don't pass the title as argument because of virtual inheritance
				dialog();
				void on_show() override;
			};

			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}