#include <QUrl>
#include <QString>
#include <QVariantMap>
#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/label.h>
#include <utils/ui/qt/app.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(label);

			qt::label::label(ui::node* parent, const std::string& text)
				: ui::node(parent)
				, base(parent)
				, qt::widget(parent)
			{
				app().do_in_main_thread([self = this, text]() {
					const QUrl url(u"qrc:QtGUI/Label.qml"_qs);
					QVariantMap initialProperties;
					initialProperties["text"] = QString(text.c_str());
					auto r = self->qt::node::init(url, initialProperties);
					if (r != 0)
						return r; // For putting BP here while debugging
					return 0;
				});
			}

			void qt::label::on_show()
			{
				// qt::Text("%s", get_text().c_str());
			}

			void qt::label::set_text(const std::string& text)
			{
				app().do_in_main_thread([self = this, text]() {
					if (auto object = self->qobject())
						object->setProperty("text", QString(text.c_str()));
					return 0;
				});
			}

			const std::string& qt::label::get_text() const
			{
				if (auto object = get_qobject())
					m_tmp_text = object->property("text").toString().toStdString();
				return m_tmp_text;
			}
		}
	}
}