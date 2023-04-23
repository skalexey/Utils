#pragma once

#include <utils/ui/imgui/widget_factory.h>

utils::ui::imgui::widget_factory::creators_t* utils::ui::imgui::widget_factory::m_creators = nullptr;

utils::ui::imgui::widget_factory& utils::ui::imgui::widget_factory::instance()
{
	static widget_factory instance;
	return instance;
}
