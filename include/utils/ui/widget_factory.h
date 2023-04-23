#pragma once

#include <cstdarg>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <functional>
#include <utils/ui/fwd.h>

namespace utils
{
	namespace ui
	{
		class widget_factory
		{
		public:
			template <typename T>
			std::shared_ptr<T> create() const {
				auto& creators = get_creators();
				auto it = creators.find(typeid(T).name());
				if (it != creators.end())
					return std::dynamic_pointer_cast<T>(it->second());
				return nullptr;
			}

		protected:
			using creator_t = std::function<widget_ptr()>;
			using creators_t = std::unordered_map<std::string, creator_t>;
			virtual creators_t& get_creators() const = 0;
		};
	}
}