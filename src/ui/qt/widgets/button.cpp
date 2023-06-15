#include <cmath>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>
#include <QQmlEngine>
#include <QVariant>
#include <QObject>
#include <QString>
#include <QUrl>
#include <utils/ui/qt/app.h>
#include <utils/ui/qt/widgets/button.h>
#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/button_model.h>

#include <utils/Log.h>

LOG_PREFIX("[qt::button] ");
LOG_POSTFIX("\n");

namespace utils
{
	namespace ui
	{
		namespace qt
        {
			REGISTER_WIDGET(qt::button);

			int qt::button::init()
			{
				const QUrl url(u"qrc:QtGUI/Button.qml"_qs);
				QVariantMap initialProperties;
				initialProperties["text"] = QString(get_text().c_str());
				m_model = new button_model();
				initialProperties["model"] = QVariant::fromValue(m_model);
				auto r = qt::node::init(url, initialProperties);
				if (r != 0)
					return r;
				m_model->setParent(qobject());
				return 0;

				// QQuickItem* parentLayout = rootItem->findChild<QQuickItem*>("yourGridLayoutId");
				// if (parentLayout) {
				// 	// Set button position using GridLayout
				// 	QQmlEngine::setObjectOwnership(buttonItem, QQmlEngine::CppOwnership);
				// 	parentLayout->setProperty("layoutDirection", "LeftToRight"); // Set the layout direction if needed
				// } else {
				// 	// Handle the case where the layout is not found
				// }
			}

			void button::on_show()
			{
				// if (qt::Button(get_text().c_str(), sz))
				// 	on_click(true);
				qt::widget::on_show();
			}

			const vec2i& button::text_size()
			{
				// auto s = qt::CalcTextSize(get_text().c_str());
				// return m_calculated_size = { (int)s.x, (int)s.y };
				return m_calculated_size;
			}

			void button::on_set_text(const std::string& text)
			{
				qobject()->setProperty("text", QString(text.c_str()));
			}
		}
	}
}
