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
				// TODO: use variadic arguments list
				virtual utils::ui::label_ptr create_label() const;
				virtual utils::ui::text_input_ptr create_text_input() const;

			public:
				template <typename Base, typename Final>
				static void register_creator() {
					if (!m_creators)
						m_creators = new creators_t();
					auto p = std::make_shared<creator_variadic<Final, ui::widget>>();
					(*m_creators)[typeid(Base).name()] = std::dynamic_pointer_cast<creator_t>(p);
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