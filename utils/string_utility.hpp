#pragma once
#include <vector>
#include <string>
#include <functional>
namespace utility
{
	namespace str
	{
		inline static int ssplit_if(const std::string& strSource, \
			std::vector<std::string>& vectorResult, const std::string& strSpilt, \
			const std::function<bool(const std::string&)>& fnToken
		)
		{
			bool bInsert = true;
			auto fnCheckAndInsert = [fnToken, &bInsert, &vectorResult](const std::string& strToken) {
				if (strToken.empty())
					return;
				if (fnToken)
					bInsert = fnToken(strToken);
				if (bInsert)
					vectorResult.push_back(strToken);
			};

			int nHeadPos = 0, nNilPos = strSource.find(strSpilt);
			while (std::string::npos != nNilPos)
			{
				fnCheckAndInsert(strSource.substr(nHeadPos, nNilPos - nHeadPos));
				nHeadPos = nNilPos + strSpilt.size();
				nNilPos = strSource.find(strSpilt, nHeadPos);
			}
			if (strSource.length() != nHeadPos)
				fnCheckAndInsert(strSource.substr(nHeadPos));
			return vectorResult.size();
		}


		inline static int ssplit(const std::string& strSource, std::vector<std::string>& vectorResult, const std::string& strSpilt)
		{
			return ssplit_if(strSource, vectorResult, strSpilt, NULL);
		}
	}
}