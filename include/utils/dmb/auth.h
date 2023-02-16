#pragma once
#include <string>
#include <DMBCore.h>
#include <utils/io_utils.h>

namespace
{
	const std::string empty_string;
	extern std::unique_ptr<dmb::Model> identity_model_ptr;
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
extern bool request_auth(const std::string& user_name, const std::string& token);

// Function definitions
const std::string& get_user_name()
{
	if (auto data_ptr = get_identity_cfg_data())
		return (*data_ptr)["user"].AsObject().Get("name").AsString().Val();
	return empty_string;
}

const std::string& get_user_token()
{
	if (auto data_ptr = get_identity_cfg_data())
		return (*data_ptr)["user"].AsObject().Get("token").AsString().Val();
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
				LOG_ERROR("Can't create identity file");
				return nullptr;
			}
	return &identity_model_ptr->GetContent().GetData();
}

bool auth()
{
	vl::Object& data = identity_model_ptr->GetContent().GetData();
	std::string user_name, token;
	if (!get_identity(&user_name, &token))
		return false;
	return request_auth(user_name, token);
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
		name = "skalexey";
		// if (!ask_name(name))
			// return false;
		data["user"].AsObject().Set("name", name);
		store = true;
	}
	if (user_name)
		user_name->swap(name);

	if (token.empty())
	{
		std::string pass;
		pass = "unkantenaura";
		// if (!ask_pass(pass))
			// return false;
		token = h(pass);
		data["user"].AsObject().Set("token", token);
		store = true;
	}
	if (user_pass)
		user_pass->swap(token);
	
	if (store)
		identity_model_ptr->Store(identity_path.string(), { true });

	return true;
}