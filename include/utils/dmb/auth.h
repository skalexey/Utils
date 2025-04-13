#pragma once
#include <string>
#include <DMBCore.h>
#include <utils/io_utils.h>
#include <utils/extern/user_input.h>

#ifndef AUTH_LOG_ERROR
#define AUTH_LOG_ERROR(msg)
#endif

namespace
{
	std::unique_ptr<dmb::Model> identity_model_ptr;
	extern const fs::path identity_path;
}

// Function declarations
bool get_identity(std::string* name = nullptr, std::string* pass = nullptr);
std::string h(const std::string& s);
bool ask_pass(std::string& s);
bool ask_name(std::string& s);
bool auth();
const std::string& get_user_token();
const std::string& get_user_name();
vl::Object* get_identity_cfg_data();
extern int request_auth(const std::string& user_name, const std::string& token);

// Function definitions
const std::string& get_user_name()
{
	if (auto data_ptr = get_identity_cfg_data())
	{
		auto& user = data_ptr->Get("user").as<vl::Object>();
		if (user.Has("name"))
			return user.Get("name").as<vl::String>().Val();
	}
	static std::string empty_string;
	return empty_string;
}

const std::string& get_user_token()
{
	if (auto data_ptr = get_identity_cfg_data())
	{
		auto& user = data_ptr->Get("user").as<vl::Object>();
		if (user.Has("token"))
			return user.Get("token").as<vl::String>().Val();
	}
	static std::string empty_string;
	return empty_string;
}

vl::Object* get_identity_cfg_data()
{
	if (!identity_model_ptr)
		identity_model_ptr = std::make_unique<dmb::Model>();

	auto& data = identity_model_ptr->Content().Data();
	auto& user = data["user"];
	if (!user)
		data["user"] = vl::Object();

	if (!identity_model_ptr->IsLoaded())
		if (!identity_model_ptr->Load(identity_path.string()))
			if (!identity_model_ptr->Store(identity_path.string(), { true }))
			{
				AUTH_LOG_ERROR("Can't create identity file");
				return nullptr;
			}
	return &identity_model_ptr->Content().Data();
}

bool auth()
{
	vl::Object& data = identity_model_ptr->Content().Data();
	std::string user_name, token;
	if (!get_identity(&user_name, &token))
		return false;
	return request_auth(user_name, token) == 0;
}

bool ask_name(std::string& s)
{
	return utils::input::ask_line(s, "Enter your login name: ", " > ");
}

bool ask_pass(std::string& s)
{
	return utils::input::ask_line(s, "Enter password: ", " > ");
}

std::string h(const std::string& s)
{
	return std::to_string(utils::string::hash(s));
}

bool get_identity(std::string* user_name, std::string* user_pass)
{
	auto data_ptr = get_identity_cfg_data();
	if (!data_ptr)
		return false;

	auto& data = *data_ptr;
	
	auto name = get_user_name();
	auto token = get_user_token();
	bool store = false;
	if (name.empty())
	{
		if (!ask_name(name))
			return false;
		data["user"].as<vl::Object>().Set("name", name);
		store = true;
	}
	if (user_name)
		user_name->swap(name);

	if (token.empty())
	{
		std::string pass;
		if (!ask_pass(pass))
			return false;
		token = h(pass);
		data["user"].as<vl::Object>().Set("token", token);
		store = true;
	}
	if (user_pass)
		user_pass->swap(token);
	
	if (store)
		identity_model_ptr->Store(identity_path.string(), { true });

	return true;
}