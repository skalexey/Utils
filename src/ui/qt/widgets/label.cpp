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

			int qt::label::init()
			{
				const QUrl url(u"qrc:QtGUI/Label.qml"_qs);
				QVariantMap initial_properties;
				auto r = qt::node::init(url, initial_properties);
				if (r != 0)
					return r; // For putting BP here while debugging
				return 0;
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