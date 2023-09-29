#pragma once
#include <list>
#include <string>
#include <memory>
#include <cstdint>
#include <inttypes.h>

namespace port_proxy
{
	namespace model
	{
		//TODO:tcp or udp
		typedef struct tagRule
		{
			char szName[512] {0x00};

			char chInIPvN = '4';
			char szInAddress[64] {0x00};
			uint16_t uInPort = 0;

			char chOutIPvN = '4';
			uint16_t uOutPort = 0;
			char szOutAddress[64] {0x00};
		}Rule;
		
		using RuleList_t = std::list<std::shared_ptr<Rule>>;
	}
}