#include <cassert>
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
				const QUrl url(u"qrc:QtGUI/Dialog.qml"_qs);
				QVariantMap initialProperties;
				initialProperties["title"] = QString(get_title().c_str());
				auto r = qt::node::init(url, initialProperties);
				assert(qobject());
				if (r != 0)
					return r;

				QVariant result = qobject()->property("show");
				if (result.canConvert<QJSValue>()) {
					QJSValue jsFunction = result.value<QJSValue>();
					if (jsFunction.isCallable()) {
						QJSValueList args;
						QJSValue returnValue = jsFunction.call(args);
					}
				}

				return 0;
			}

			QObject* qt::dialog::content_qobject()
			{
				QVariant contentItemVariant = qobject()->property("contentItem");
        		QObject *contentItem = contentItemVariant.value<QObject*>();
				return contentItem;
			}

			void qt::dialog::on_before_show()
			{
				qt::window::on_before_show();
				if (!is_open())
					set_title("test not open");
				else
					set_title("open open");
			}

			void qt::dialog::on_show()
			{
				if (!qobject())
					assert(init() == 0);

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