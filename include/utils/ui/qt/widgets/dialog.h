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
				dialog();
				QObject* content_qobject() override;
				
			protected:
				void on_before_show() override;
				void on_set_title() override;
				int init() override;
				void on_set_modal() override;
				bool qt_dialog_update(float dt);
				
				virtual bool on_qt_dialog_update(float dt) {
					return true;
				}

			private:
				bool on_update(float dt) override final;

			private:
				vec2i m_last_size;
				vec2i m_last_position;
				QObject* m_content = nullptr;
			};
			using dialog_ptr = std::shared_ptr<dialog>;
		}
	}
}