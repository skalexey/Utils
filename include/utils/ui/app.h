#pragma once

#include <functional>
#include <list>
#include <utils/ui/widget.h>

namespace utils
{
	namespace ui
	{
		class app
		{
			class run_exception
			{
			public:
				run_exception(int erc) : m_erc(erc) {}
				int get_erc() const { return m_erc; }
			private:
				int m_erc = -1;
			};

		public:
			using on_update_t = std::function<bool(float dt)>;
			using on_update_list_t = std::list<on_update_t>;

			int run() {
				// Catch exit() function calls.
				try
				{
					return on_run();
				}
				catch(const run_exception& ex)
				{
					return ex.get_erc();
				}
			}
			virtual bool update(float dt) {
				for (auto it = m_on_update.begin(); it != m_on_update.end();)
					if (!(*it)(dt))
						it = m_on_update.erase(it);
					else
						++it;
				return !m_on_update.empty();
			}

			virtual int on_run() {
				return 0;
			}

			void add_on_update(const on_update_t& on_update) {
				m_on_update.push_back(on_update);
			}

			void exit(int erc) {
				throw run_exception(erc);
			}

		private:
			on_update_list_t m_on_update;
		};
	}
}
