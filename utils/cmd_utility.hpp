#pragma once
#include <string>
#include <memory>
#include <windows.h>
#include <iostream>
namespace utility
{
	namespace cmd
	{
		inline bool execCmd(const char* szCmd, std::string& strResult)
		{
			SECURITY_ATTRIBUTES saPipe;
			saPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
			saPipe.lpSecurityDescriptor = NULL;
			saPipe.bInheritHandle = true;

			HANDLE hReadPipe, hWritePipe;
			BOOL bSuccess = CreatePipe(&hReadPipe,
				&hWritePipe,
				&saPipe,
				0
			);
			if (!bSuccess)
			{
				return false;
			}

			PROCESS_INFORMATION pi;
			STARTUPINFOA si;
			memset(&si, 0, sizeof(si));
			GetStartupInfoA(&si);
			si.hStdError = hWritePipe;
			si.hStdOutput = hWritePipe;
			//隐藏命令行窗口
			si.wShowWindow = SW_SHOW;
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

			auto buffer_len = strlen(szCmd) + 1;
			std::unique_ptr<char> uspCmdLine(new char[buffer_len]);
			snprintf(uspCmdLine.get(), buffer_len, szCmd);
			if (!CreateProcessA(NULL, uspCmdLine.get(), NULL, NULL, true, 0, NULL, NULL, &si, &pi))
			{
				CloseHandle(pi.hThread);
				return false;
			}

			//TODO:control wait time
			DWORD dwRet = WaitForSingleObject(pi.hProcess, 3 * 1000);
			char result[1024] = { 0 };
			DWORD dw;
			bSuccess = ReadFile(hReadPipe, result, 1024, &dw, NULL);
			strResult = result;
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);

			CloseHandle(hReadPipe);
			CloseHandle(hWritePipe);

			std::cout << result << std::endl;
			return true;
		}
	}
}