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
				dialog();
				
			protected:
				bool imgui_dialog_update(float dt);
				virtual bool on_imgui_dialog_update(float dt) {
					return base::on_update(dt);
				}
				
			private:
				bool on_update(float dt) override final;

			private:
				vec2i m_last_size;
				vec2i m_last_position;
			};
			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}