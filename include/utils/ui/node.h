#pragma once

#include <cassert>
#include <memory>
#include <utils/ui/fwd.h>
#include <utils/ui/entity.h>

namespace utils
{
	namespace ui
	{
		class node : public virtual entity, public std::enable_shared_from_this<node>
		{
		public:
			// Keep this without default value to let compiler catch missed calls to
			// this constructor from any derived class as this one is always virtual.
			node(node* parent);

			virtual void set_parent(node* parent) {
				m_parent = parent;
			}

			const node* get_parent() const {
				return m_parent;
			}

			node* parent() {
				return m_parent;
			}

			node* root() {
				return m_parent ? m_parent->root() : this;
			}

            app& app() {
				return const_cast<ui::app&>(const_cast<const node*>(this)->get_app());
			}

			void set_factory(widget_factory& factory) {
				m_factory = &factory;
			}
			
			const widget_factory& get_factory() const {
				assert(m_factory && "Forgot to call set_factory()?");
				return *m_factory;
			}

			const ui::app& get_app() const {
				assert(m_app && "Why do you need app() from a half-initialized widget?");
				return *m_app;
			}

		private:
			node* m_parent = nullptr;
			ui::app* m_app = nullptr;
			widget_factory* m_factory = nullptr;
		};
		using node_ptr = std::shared_ptr<node>;
	}
}
