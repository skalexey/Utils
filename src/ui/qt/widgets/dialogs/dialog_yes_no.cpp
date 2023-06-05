#include <utils/ui/qt/widget_factory.h>
#include <utils/ui/qt/widgets/dialogs/dialog_yes_no.h>
#include <utils/ui/qt/common.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			REGISTER_WIDGET(qt::dialog_yes_no);

			qt::dialog_yes_no::dialog_yes_no(ui::node* parent)
				: ui::node(parent)
				, base(parent)
				, qt::dialog(parent)
			{
				shared_constructor();
			}

			qt::dialog_yes_no::dialog_yes_no::dialog_yes_no(
				ui::node* parent
				, const std::string& msg
				, const on_answer_t& on_answer
				, const char* yes_text
				, const char* no_text
				, const std::optional<std::string>& title
			)
				: ui::node(parent)
				, base(parent, msg, on_answer, yes_text, no_text, title)
				, qt::dialog(parent)
			{
				shared_constructor();
			}

			void dialog_yes_no::shared_constructor()
			{
				yes_button().set_on_before_show([this]() {
					// qtStyle& style = qt::GetStyle();
					// float width = 0.0f;
					// width += yes_button().get_size().x;
					// width += style.ItemSpacing.x;
					// width += 60.0f;
					// width += no_button().get_size().x;
					// utils::ui::qt::AlignForWidth(width);
				});
				yes_button().set_on_show([this]() {

					// qt::SameLine();
					// qt::Dummy({ 60, 0 });
					// qt::SameLine();
				});
			}

			void qt::dialog_yes_no::button_yes_show()
			{
				base::button_yes_show();
				// qt::SameLine();
			}
		}
	}
}