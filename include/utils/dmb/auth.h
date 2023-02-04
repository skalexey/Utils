#pragma once
#include <string>
#include <DMBCore.h>

namespace
{
	std::string empty_string;
	std::unique_ptr<dmb::Model> identity_model_ptr;
}

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