#pragma once
#include "model.h"

namespace port_proxy
{
	class control
	{
	public:
		control();
		virtual ~control();

		static control& instance()
		{
			static control sl_Instance;
			return sl_Instance;
		}

		bool list(model::RuleList_t& vRuleList);

		bool addRule(const std::shared_ptr<model::Rule>& spRule, std::string& strResult);

		bool deleteRule(const std::shared_ptr<model::Rule>& spRule, std::string& strResult);

	};
}

