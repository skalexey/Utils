#pragma once

#include <memory>
#include <functional>
#include <string>
#include <DMBCore.h>
#include <utils/extern/user_input.h>
#ifndef AUTH_LOG_ERROR
#define AUTH_LOG_ERROR(msg)
#endif

namespace
{
	std::unique_ptr<dmb::Model> identity_model_ptr;
	extern fs::path identity_path;
}

// Function declarations
using on_get_identity_result_t = std::function<void(bool, std::string, std::string)>;
void get_identity(const on_get_identity_result_t& on_result);
std::string h(const std::string& s);
void ask_pass(const utils::void_string_bool_cb& on_answer);
void ask_name(const utils::void_string_bool_cb& on_answer);
void auth(const utils::void_int_cb& on_auth);
const std::string& get_user_token();
const std::string& get_user_name();
vl::Object* get_identity_cfg_data();
extern void request_auth(const std::string& user_name, const std::string& token, const utils::void_int_cb& on_result);

// Function definitions
const std::string& get_user_name()
{
	if (auto data_ptr = get_identity_cfg_data())
		return data_ptr->Def<vl::Object>("user").Def<vl::String>("name").Val();
	static std::string empty_string;
	return empty_string;
}

const std::string& get_user_token()
{
	if (auto data_ptr = get_identity_cfg_data())
		return data_ptr->Def<vl::Object>("user").Def<vl::String>("token").Val();
	static std::string empty_string;
	return empty_string;
}

vl::Object* get_identity_cfg_data()
{
	if (!identity_model_ptr)
		identity_model_ptr = std::make_unique<dmb::Model>();

	if (!identity_model_ptr->IsLoaded())
		if (!identity_model_ptr->Load(identity_path.string()))
			if (!identity_model_ptr->Store(identity_path.string(), { true }))
			{
				AUTH_LOG_ERROR("Can't create identity file");
				return nullptr;
			}
	return &identity_model_ptr->Content().Data();
}

void auth(const utils::void_int_cb& on_result)
{
	vl::Object& data = identity_model_ptr->Content().Data();
	std::string user_name, token;
	get_identity([=](bool ok, std::string user_name, std::string token) {
		if (!ok)
			return on_result(-1);
		request_auth(user_name, token, on_result);
	});
}

void ask_name(const utils::void_string_bool_cb& on_answer)
{
	ask_line("Enter your login name", on_answer);
}

void ask_pass(const utils::void_string_bool_cb& on_answer)
{
	ask_line("Enter password: ", on_answer);
}

std::string h(const std::string& s)
{
	return std::to_string(utils::string::hash(s));
}

void get_identity(const on_get_identity_result_t& on_result)
{
	auto data_ptr = get_identity_cfg_data();
	if (!data_ptr)
		return on_result(false, {}, {});

	struct context
	{
		std::string name = get_user_name();
		std::string token = get_user_token();
		bool store = false;
	};

	std::shared_ptr<context> ctx = std::make_shared<context>();

	auto finish = [=](bool ok) {
		if (ctx->store)
			identity_model_ptr->Store(identity_path.string(), { true });
		on_result(ok, ctx->name, ctx->token);
	};

	auto check_token = [=] {
		if (ctx->token.empty())
		{
			ask_pass([=](const std::string& pass, bool ok) {
				if (!ok)
					finish(false);
				auto token = h(pass);
				auto& data = *data_ptr;
				data.Def<vl::Object>("user")["token"] = token;
				ctx->store = true;
				ctx->token = token;
				finish(true);
			});
		}
		else
			finish(true);
	};

	if (ctx->name.empty())
	{
		ask_name([=](const std::string& name, bool ok) {
			if (!ok)
				on_result(false, {}, {});
			auto& data = *data_ptr;
			data.Def<vl::Object>("user")["name"] = name;
			ctx->store = true;
			ctx->name = name;
			check_token();
		});
	}
	else
		check_token();
}