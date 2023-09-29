#include "control.h"
#include "cmd_utility.hpp"
#include "string_utility.hpp"

using namespace port_proxy;
using namespace model;

control::control()
{
}


control::~control()
{
}

bool control::list(RuleList_t& vRuleList)
{
	vRuleList.clear();
	std::string strResult;
	if (!utility::cmd::execCmd("netsh interface portproxy show all", strResult))
	{
		return false;
	}
	std::vector<std::string> vLineList;
	utility::str::ssplit_if(strResult, vLineList, "\r\n", [](const std::string& strToken) {
		return strToken.size() && strToken[0] != '-'/* && strncmp(strToken.c_str(), u8"地址", strlen(u8"地址"))*/;
	});
	//TODO
	if (vLineList.empty())
		return false;
	
	// 默认状态机
	char chIn = '4', chOut = '4';

	for (auto& iter = vLineList.begin(); iter != vLineList.end(); iter++)
	{
		std::vector<std::string> vCols;
		utility::str::ssplit(*iter, vCols, " ");
		if(!strncmp(vCols[1].c_str(), "ipv", strlen("ipv")))
		{
			if (!strncmp(vCols[1].c_str(), "ipv6:", strlen("ipv6:")))
				chIn = '6';
			if (!strncmp(vCols[3].c_str(), "ipv6:", strlen("ipv6:")))
				chOut = '6';
			++iter;
			continue;
		}
		
		auto spRule = std::make_shared<Rule>();

		spRule->chInIPvN = chIn;
		sprintf_s(spRule->szInAddress, sizeof(spRule->szInAddress) - 1, vCols[0].c_str());
		spRule->uInPort = atoi(vCols[1].c_str());

		spRule->chOutIPvN = chOut;
		spRule->uOutPort = atoi(vCols[3].c_str());
		sprintf_s(spRule->szOutAddress, sizeof(spRule->szOutAddress) - 1, vCols[2].c_str());

		vRuleList.emplace_back(std::move(spRule));
	}

	return true;
}



bool control::addRule(const std::shared_ptr<Rule>& spRule, std::string& strResult)
{
	char szCmdLine[512]{ 0x00 };
	sprintf_s(szCmdLine, 512,
		"netsh interface portproxy add "
		"v%ctov%c listenport=%" PRIu16
		" connectaddress=%s connectport=%" PRIu16,
		spRule->chInIPvN, spRule->chOutIPvN, spRule->uInPort,
		spRule->szOutAddress, spRule->uOutPort
	);
	std::cout << szCmdLine << std::endl;
	if (!utility::cmd::execCmd(szCmdLine, strResult))
		return false;
	return true;
}

//delete v4tov4 listenport= {Integer | ServiceName} [[listenaddress=] {IPv4Address | HostName}
bool control::deleteRule(const std::shared_ptr<Rule>& spRule, std::string& strResult)
{
	char szCmdLine[512]{0x00};
	sprintf_s(szCmdLine, 512,
		"netsh interface portproxy delete "
		"v%ctov%c listenport=%" PRIu16,
		spRule->chInIPvN, spRule->chOutIPvN, spRule->uInPort
	);
	std::cout << szCmdLine << std::endl;
	if (!utility::cmd::execCmd(szCmdLine, strResult))
	{
		return false;
	}
	return false;
}
