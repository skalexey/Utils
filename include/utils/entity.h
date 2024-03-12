#pragma once

#include <memory>

namespace utils
{
	class entity : public std::enable_shared_from_this<entity>
	{
		public:
			template <typename T>
			T& as() {
				return dynamic_cast<T&>(*self());
			}
			template <typename T>
			const T& as() const {
				return dynamic_cast<const T&>(*self());
			}
			template <typename T>
			std::shared_ptr<const T> as_ptr() const {
				return std::dynamic_pointer_cast<const T>(self()->shared_from_this());
			}
			template <typename T>
			std::shared_ptr<T> as_ptr() {
				return std::dynamic_pointer_cast<T>(self()->shared_from_this());
			}
			template <typename T>
			bool is() const {
				return dynamic_cast<const T*>(self()) != nullptr;
			}
			virtual ~entity() = default;

		protected:
			virtual const entity* self() const {
				return this;
			}

			entity* self() {
				return const_cast<entity*>(const_cast<const entity*>(this)->self());
			}
	};
	using entity_ptr = std::shared_ptr<entity>;
}
