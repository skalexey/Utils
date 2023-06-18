#include <QUrl>
#include <QString>
#include <QVariantMap>
#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/text.h>
#include <utils/ui/qt/app.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(text);

			qt::text::text(ui::node* parent, const std::string& text)
				: ui::node(parent)
				, base(parent)
				, qt::widget(parent)
			{
				app().do_in_main_thread([self = this, text]() {
					const QUrl url(u"qrc:QtGUI/Text.qml"_qs);
					QVariantMap initialProperties;
					initialProperties["text"] = QString(text.c_str());
					auto r = self->qt::node::init(url, initialProperties);
					if (r != 0)
						return r; // For putting BP here while debugging
					return 0;
				});
			}

			void qt::text::on_show()
			{
				// qt::TextWrapped("%s", get_text().c_str());
			}

			void qt::text::set_text(const std::string& text)
			{
				app().do_in_main_thread([self = this, text]() {
					if (auto object = self->qobject())
						object->setProperty("text", QString(text.c_str()));
					return 0;
				});
			}

			const std::string& qt::text::get_text() const
			{
				if (auto object = get_qobject())
					m_tmp_text = object->property("text").toString().toStdString();
				return m_tmp_text;
			}
		}
	}
}