#pragma once
#include <vector>
#include <string>
#include <codecvt>
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

		inline static void strip_last(std::string& source, const std::initializer_list<char>& hint_words)
		{
			if(source.empty()) return;
			bool find = false;
			int nPos = source.length();
			for (auto iter = source.rbegin(); iter != source.rend(); iter++)
			{
				for (const auto& elem : hint_words)
				{
					if (*iter == elem)
					{
						find = true;
						nPos--;
						break;
					}
				}
				if(find) continue;
				break;
			}
			if (nPos == source.length()) return;
			source.erase(nPos);
		}

		inline std::string gbk_to_utf8(const std::string& str)
		{
			std::wstring_convert<std::codecvt<wchar_t, char, mbstate_t>> gbk_cvt(new std::codecvt<wchar_t, char, mbstate_t>("Chinese"));
			std::wstring t = gbk_cvt.from_bytes(str);
			std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_cvt;
			return utf8_cvt.to_bytes(t);
		}
	}
}