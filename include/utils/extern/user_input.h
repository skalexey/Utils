#pragma once

#include <string>
#include <utils/common.h>

extern void ask_user(
	const std::string& question
	, const utils::void_bool_cb& on_answer = nullptr
	, const char* yes_btn_text = nullptr
	, const char* no_btn_text = nullptr
);

extern void ask_line(
	const std::string& msg
	, const utils::void_string_bool_cb& on_answer
	, const std::string& default_value = {}
	, const char* ok_btn_text = nullptr
	, const char* cancel_btn_text = nullptr
);

extern void show_message(
	const std::string& message
	, const utils::void_cb& on_close = nullptr
	, const char* ok_btn_text = nullptr
);