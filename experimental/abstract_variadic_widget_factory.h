#pragma once

#include <cstdarg>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <functional>
#include <abstract_ui/fwd.h>

namespace utils
{
	namespace ui
	{
		class widget_factory
		{
		public:
			template <typename T, typename... Args>
			std::shared_ptr<T> create(Args&&... args) const {
				auto& creators = get_creators();
				auto it = creators.find(typeid(T).name());
				if (it != creators.end())
				{
					return std::dynamic_pointer_cast<T>((*it->second)());
				}
				return nullptr;
			}

		protected:

			template <typename R>
			struct creator_base
			{
				virtual std::shared_ptr<R> operator()() const = 0;
			};

			template <typename T, typename R>
			struct creator_variadic : public creator_base<R>
			{
				std::shared_ptr<R> operator()() const override {
					return std::dynamic_pointer_cast<R>(std::make_shared<T>());
				}
			};
				//std::function<widget_ptr()>;
			// using creator_t = std::function<widget_ptr()>;
			using creator_t = creator_base<widget>;
			using creator_t_ptr = std::shared_ptr<creator_t>;
			using creators_t = std::unordered_map<std::string, creator_t_ptr>;
			virtual creators_t& get_creators() const = 0;
		};
	}
}