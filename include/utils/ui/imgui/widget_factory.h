#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <utils/ui/widget_factory.h>

namespace utils
{
	namespace ui
	{
		namespace imgui
		{
			class widget_factory : public utils::ui::widget_factory
			{
			public:
				static widget_factory& instance();

			public:
				template <typename Base, typename Final>
				static void register_creator() {
					if (!m_creators)
						m_creators = new creators_t();
					(*m_creators)[typeid(Base).name()] = []() {
						return std::make_shared<Final>();
					};
				}

			protected:
				creators_t& get_creators() const override {
					return *m_creators;
				}
			private:
				static creators_t* m_creators;
			};
		}
	}
}