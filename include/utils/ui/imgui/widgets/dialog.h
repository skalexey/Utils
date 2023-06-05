#pragma once

#include <utils/ui/widgets/dialog.h>
#include <utils/ui/imgui/window.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class dialog : public imgui::window, public virtual ui::dialog
			{
				using base = ui::dialog;

			public:
				// We don't pass the title as argument because of virtual inheritance
				dialog(ui::node* parent = nullptr);
				void on_show() override;
				
			private:
				vec2i m_last_size;
				vec2i m_last_position;
			};
			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}