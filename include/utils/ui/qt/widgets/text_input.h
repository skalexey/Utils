#pragma once

#include <string>
#include <utils/ui/qt/fwd.h>
#include <utils/ui/qt/widgets/text_input_model.h>
#include <utils/ui/widgets/text_input.h>
#include <utils/ui/qt/widget.h>

class QQuickItem;

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class text_input : public qt::widget, public utils::ui::text_input
			{
			public:
				using base = utils::ui::text_input;

				text_input(
					ui::node* parent = nullptr
					, const std::string& label = {}
					, const std::string& default_value = {}
					, const on_update_t& on_update = nullptr
				);

			const std::string& get_value() const override;
			void set_value(const std::string& value) override;
			const QQuickItem* get_text_field() const;
			const QQuickItem* get_label() const;
			QQuickItem* text_field();
			QQuickItem* label();

			protected:
				bool show_input() override;
				void show_text() override;

			private:
				WIDGET_REGISTRATOR(qt::widget_factory, text_input);
				text_input_model* m_model = nullptr;
				mutable std::string m_tmp_value; // Needed as get_value() returns const std::string&,
				// but Qt can only return std::string created from QString which in turn is created from QVariant.
				QQuickItem* m_text_field = nullptr;
				QQuickItem* m_label = nullptr;
			};
			using text_input_ptr = std::shared_ptr<text_input>;
		}
	}
}