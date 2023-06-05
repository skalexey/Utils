#pragma once

#include <utils/ui/widgets/dialog.h>
#include <utils/ui/qt/window.h>

class QObject;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class dialog : public qt::window, public virtual ui::dialog
			{
				using base = ui::dialog;

			public:
				// We don't pass the title as argument because of virtual inheritance
				dialog(ui::node* parent = nullptr);
				void on_before_show() override;
				void on_show() override;
				QObject* content_qobject() override;
				
			private:
				int init();

			private:
				vec2i m_last_size;
				vec2i m_last_position;
			};
			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}