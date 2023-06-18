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

			qt::button::button(ui::node* parent)
				: ui::node(parent)
				, base(parent)
				, qt::widget(parent)
			{
				app().do_in_main_thread([self = this]() {
					const QUrl url(u"qrc:QtGUI/Button.qml"_qs);
					QVariantMap initialProperties;
					initialProperties["text"] = QString(self->get_text().c_str());
					self->m_model = new button_model();
					initialProperties["model"] = QVariant::fromValue(self->m_model);
					auto r = self->qt::node::init(url, initialProperties);
					if (r != 0)
						return r;
					self->m_model->setParent(self->qobject());
					return 0;
				});

				// QQuickItem* parentLayout = rootItem->findChild<QQuickItem*>("yourGridLayoutId");
				// if (parentLayout) {
				// 	// Set button position using GridLayout
				// 	QQmlEngine::setObjectOwnership(buttonItem, QQmlEngine::CppOwnership);
				// 	parentLayout->setProperty("layoutDirection", "LeftToRight"); // Set the layout direction if needed
				// } else {
				// 	// Handle the case where the layout is not found
				// }
			}

			void qt::button::on_set_on_click(const on_click_t& on_click)
			{
				m_model->set_on_click(on_click);
			}

			void qt::button::on_show()
			{
				// if (qt::Button(get_text().c_str(), sz))
				// 	on_click(true);
				qt::widget::on_show();
			}

			const vec2i& qt::button::text_size()
			{
				// auto s = qt::CalcTextSize(get_text().c_str());
				// return m_calculated_size = { (int)s.x, (int)s.y };
				return m_calculated_size;
			}

			void qt::button::on_set_text(const std::string& text)
			{
				qobject()->setProperty("text", QString(text.c_str()));
			}
		}
	}
}
