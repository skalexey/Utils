#pragma once

#include <memory>
#include <string>
#include <utils/ui/qt/fwd.h>
#include <utils/ui/widgets/label.h>
#include <utils/ui/qt/widget.h>

namespace utils
{
	namespace ui
	{
		namespace qt
		{
			class label : public qt::widget, public utils::ui::label
			{
			public:
				using base = ui::label;

				void set_text(const std::string& text) override;
				const std::string& get_text() const override;
				
			protected:
				int init() override;

			private:
				WIDGET_REGISTRATOR(qt::widget_factory, label);
				
			private:
				mutable std::string m_tmp_text; // Needed as get_text() returns const std::string&,
				// but Qt can only return std::string created from QString which in turn is created
			};
			
			using label_ptr = std::shared_ptr<label>;
		}
	}
}