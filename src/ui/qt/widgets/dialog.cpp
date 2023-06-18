#include <QtGlobal>
#include <QQuickWindow>
#include <QUrl>
#include <QString>
#include <utils/string_utils.h>
#include <utils/ui/qt/widgets/dialog.h>
#include <utils/ui/qt/app.h>
#include <utils/Log.h>
#ifdef LOG_ON
#include <QDebug>
#endif
#include "QtGUI/include/app.h"

LOG_POSTFIX("\n");
LOG_PREFIX("[qt::dialog]: ");

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			qt::dialog::dialog(ui::node* parent)
				: ui::node(parent)
				, ui::dialog(parent)
				, qt::window(parent)
			{
				set_title("qt Dialog");
				init();
			}

			int qt::dialog::init()
			{
				return app().do_in_main_thread([self = this] {
					const QUrl url(u"qrc:QtGUI/Dialog.qml"_qs);
					QVariantMap initialProperties;
					initialProperties["title"] = QString(self->get_title().c_str());
					auto r = self->qt::node::init(url, initialProperties);
					Q_ASSERT(self->qobject());
					if (r != 0)
						return r;

					self->m_content = self->qobject()->findChild<QObject*>("content");

					QVariant result = self->qobject()->property("show");
					if (result.canConvert<QJSValue>()) {
						QJSValue jsFunction = result.value<QJSValue>();
						if (jsFunction.isCallable()) {
							QJSValueList args;
							QJSValue returnValue = jsFunction.call(args);
						}
					}

					return 0;
				});
			}

			void qt::dialog::on_set_title()
			{
 				if (auto object = qobject())
					object->setProperty("title", QString(get_title().c_str()));
			}

			QObject* qt::dialog::content_qobject()
			{
				return m_content;
			}

			void qt::dialog::on_before_show()
			{
				qt::window::on_before_show();
			}

			void qt::dialog::on_show()
			{
				if (!qobject())
					Q_ASSERT(init() == 0);

				if (!is_open())
					return;

				// Qt-dialog-specific stuff
				// if (is_auto_resize())
				// 	window_flags |= qtWindowFlags_AlwaysAutoResize;

				// auto label = utils::format_str("%s##", get_title().c_str());
				// bool p_open = true;
				// auto p_open_ptr = is_close_button_enabled() ? &p_open : nullptr;
				if (is_modal())
				{
				}
				else
				{
				}

				base::on_show();

				// Close button
				// if (!p_open)
				// 	close();
			}
		}
	}
}