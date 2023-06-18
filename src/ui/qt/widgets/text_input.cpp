#include <QtGlobal>
#include <QQuickItem>
#include <utils/ui/widgets/text_input.h>
#include <utils/io_utils.h>
#include <utils/ui/qt/app.h>
#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/text_input.h>

namespace
{
	std::string empty_string;
}

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(text_input);

			qt::text_input::text_input(
				ui::node* parent
				, const std::string& label
				, const std::string& default_value
				, const on_update_t& on_update
			)
				: ui::node(parent)
				, base(parent, label, default_value, on_update)
				, qt::widget(parent)
			{
				app().do_in_main_thread([self = this]() {
					const QUrl url(u"qrc:QtGUI/TextInput.qml"_qs);
					QVariantMap initialProperties;
					initialProperties["text"] = QString(self->get_default_value().c_str());
					self->m_model = new text_input_model();
					initialProperties["model"] = QVariant::fromValue(self->m_model);
					auto r = self->qt::node::init(url, initialProperties);
					if (r != 0)
						return r;
					self->m_model->setParent(self->qobject());
					self->m_text_field = self->qobject()->findChild<QQuickItem*>("input");
					self->m_label = self->qobject()->findChild<QQuickItem*>("label");
					return 0;
				});
			}

            bool qt::text_input::show_input()
			{
				// auto ret = qt::InputText(m_input_label.c_str(), m_edit_value.data(), m_edit_value.capacity());
				// TODO: move it to qt::widget class:
				// Store size and position of qt widget for further getting it with get_size() and get_position().
				return true;
			}

			void qt::text_input::show_text()
			{
				// qt::Text("%s", m_label.c_str());
				// qt::SameLine();
			}

			const std::string& qt::text_input::get_value() const
			{
				if (auto object = get_qobject())
				{
					Q_ASSERT(get_text_field());
					return m_tmp_value = get_text_field()->property("text").toString().toStdString();
				}
				return empty_string;
			}

			void qt::text_input::set_value(const std::string& value)
			{
				app().do_in_main_thread([self = this, value]() {
					if (auto object = self->qobject())
						object->setProperty("text", QString(value.c_str()));
					return 0;
				});
			}

			const QQuickItem* qt::text_input::get_text_field() const
			{
				return m_text_field;
			}

			QQuickItem* qt::text_input::text_field()
			{
				return m_text_field;
			}

			const QQuickItem* qt::text_input::get_label() const
			{
				return m_label;
			}

			QQuickItem* qt::text_input::label()
			{
				return m_label;
			}
		}
	}
}