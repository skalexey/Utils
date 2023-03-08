#pragma once

#include <utils/ui/imgui/widgets/label.h>
#include <utils/ui/imgui/widgets/button.h>
#include <utils/ui/imgui/widgets/text_input.h>
#include <utils/ui/imgui/widget_factory.h>

utils::ui::imgui::widget_factory& utils::ui::imgui::widget_factory::instance()
{
	static widget_factory instance;
	return instance;
}

utils::ui::label_ptr utils::ui::imgui::widget_factory::create_label() const
{
	return std::make_shared<utils::ui::imgui::label>();
}

utils::ui::button_ptr utils::ui::imgui::widget_factory::create_button() const
{
	return std::make_shared<utils::ui::imgui::button>();
}

utils::ui::text_input_ptr utils::ui::imgui::widget_factory::create_text_input() const
{
	return std::make_shared<utils::ui::imgui::text_input>();
}