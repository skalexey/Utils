#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <limits>
#include <cassert>
#include <functional>
#include <list>
#include <utils/ui/fwd.h>
#include <utils/ui/node.h>
#include <utils/common.h>

namespace utils
{
	namespace ui
	{
		class app : public virtual ui::node
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

			app(int argc, char* argv[])
				: ui::node(nullptr)
				, m_args{argc, argv}
				, m_thread_id(std::this_thread::get_id())
			{}

			std::thread::id get_thread_id() const {
				return m_thread_id;
			}

			int run() {
				// Catch exit() function calls.
				try
				{
					// TODO: call init here
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
				// return !m_on_update.empty();
				return on_update(dt);
			}

			virtual bool on_update(float dt) {
				return true;
			}

			virtual int on_run() {
				return 0;
			}

			virtual int init() {
					return 0;
			}

			void add_on_update(const on_update_t& on_update) {
				m_on_update.push_back(on_update);
			}

			int do_in_main_thread(const utils::int_cb& job);

			void exit(int erc) {
				throw run_exception(erc);
			}

		protected:
			struct args_t
			{
				int argc = -1;
				char** argv = nullptr;
			};

			const args_t& get_args() const {
				return m_args;
			}

			args_t& args() {
				return m_args;
			}
			
		private:
			args_t m_args;
			on_update_list_t m_on_update;
			std::thread::id m_thread_id;
		};
	}
}
