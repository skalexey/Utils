#include <utils/ui/imgui/widgets/dialogs/dialog_yes_no.h>
#include <imgui.h>

void utils::ui::imgui::dialog_yes_no::button_yes_show()
{
	base::button_yes_show();
	ImGui::SameLine();
}