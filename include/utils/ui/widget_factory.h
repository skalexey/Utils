#pragma once

#include <iostream>
#include <cassert>
#include <cstdarg>
#include <memory>
#include <typeinfo>
#include <string>
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
			std::shared_ptr<T> create(node* parent = nullptr) const {
				auto& creators = get_creators();
				auto it = creators.find(typeid(T).name());
                assert(it != creators.end() && "Type not found");
				if (it != creators.end())
					return std::dynamic_pointer_cast<T>(it->second(parent));
				#if defined(LOG_ON)
					std::cout << "Can't find the type '" << typeid(T).name() << "'" << std::endl;
				#endif
				return nullptr;
			}

		protected:
			using creator_t = std::function<widget_ptr(node*)>;
			using creators_t = std::unordered_map<std::string, creator_t>;
			virtual creators_t& get_creators() const = 0;
		};
	}
}
