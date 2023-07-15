#pragma once

#ifdef LOG_ON
	#include <iostream>
	#include <sstream>
	#include <memory>
	#include <mutex>

	namespace utils
	{
		class user_log_stream;

		struct basic_stream_impl
		{
			void accept(const std::string& str) {
				std::lock_guard<std::mutex> lock(m_mutex);
				accept_impl(str);
			}

			virtual void accept_impl(const std::string& str) = 0;
			
		private:
			std::mutex m_mutex;
		};
		using basic_stream_impl_ptr = std::shared_ptr<basic_stream_impl>;

		template <typename stream_impl_t>
		struct basic_log_stream
		{
			basic_log_stream() = default;
			virtual ~basic_log_stream() {
				try
				{
					stream_impl().accept(m_buffer.str());
				}
				catch (const std::exception& e)
				{
					std::cout << "basic_log_stream: Exception in destructor: '" << e.what() << "'";
				}
				catch (...)
				{
					std::cout << "basic_log_stream: Unrecognized exception in destructor";
				}
			}
			inline static basic_stream_impl_ptr m_impl = std::make_shared<stream_impl_t>();

			template <typename T>
			basic_log_stream &operator<<(const T &t) {
				if (!is_valid())
					return *this;
				m_buffer << t;
				return *this;
			}
			template <typename new_stream_impl_t>
			static void set_impl() {
				m_impl = std::make_shared<new_stream_impl_t>();
			}
			bool is_valid() const {
				return m_impl != nullptr;
			}

		protected:
			basic_stream_impl& stream_impl() {
				return* m_impl;
			}

		private:
			std::stringstream m_buffer;
		};

		#define DEFINE_LOG_STREAM_IMPL(class_name, stream_expression) \
			struct class_name : public utils::basic_stream_impl \
			{ \
				void accept_impl(const std::string& str) override { \
					stream_expression << str; \
				} \
			}

		#define DEFINE_LOG_STREAM(class_name, stream_expression) \
			DEFINE_LOG_STREAM_IMPL(class_name##_impl, stream_expression); \
			class class_name : public utils::basic_log_stream<class_name##_impl> {};

		DEFINE_LOG_STREAM(default_log_stream, std::cout);
	}

	// Allow user to set their own
	#define LOG_STREAM(stream_expression) namespace utils { DEFINE_LOG_STREAM( user_log_stream, stream_expression ) }

	#define GLOBAL_LOG_STREAM(stream_expression) \
		DEFINE_LOG_STREAM_IMPL(user_log_stream_impl, stream_expression); \
		utils::default_log_stream::set_impl<user_log_stream_impl>();

#else
	#define DEFINE_LOG_STREAM_IMPL(class_name, stream_expression)
	#define DEFINE_LOG_STREAM(class_name, stream_expression)
	#define LOG_STREAM(stream_expression) 
#endif


