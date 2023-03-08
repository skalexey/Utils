#pragma once

#include <memory>
#include <functional>
#include <string>
#include <utils/vec2.h>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		class window : public widget
		{
			using base = widget;
			using on_close_cb = base::on_hide_cb;

			public:

				void close() {
					set_visible(false);
					on_close();
				}

				void set_on_close(const on_close_cb& on_close) {
					set_on_hide(on_close);
				}

				bool is_open() const {
					return is_visible();
				}

				const std::string& get_title() const override {
					static std::string default_title = "window";
					return default_title;
				};

			protected:
				virtual void on_close() {}
		};

		using window_ptr = std::shared_ptr<window>;
	}
}