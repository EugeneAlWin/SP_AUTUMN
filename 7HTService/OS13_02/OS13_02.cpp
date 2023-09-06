#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "HT_LIB.h"
#include <string>
#include <sstream>
#include <wchar.h>
#include <thread>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include <stdio.h>

HANDLE mutexName;
constexpr auto TRACEPATH = "D:\\02.trace";
HANDLE hThread;
OS13HANDEL ht = nullptr;

HTHANDLE* HT;
void trace(const char* msg, int r = std::ofstream::out)
{
	std::ofstream out;
	out.open(TRACEPATH, r);
	out << msg << "\n";
	out.close();
}

wchar_t* GetWC(const char* c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}


BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal.
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		printf("Ctrl-C event\n\n");
		CloseHandle(hThread);
		HT_LIB::HT::Close(ht, HT);
		HT_LIB::Dispose(ht);
		return FALSE;


	default:
		return FALSE;
	}
}

int main(int argc, char* argv[])
{
	SetConsoleCtrlHandler(CtrlHandler, TRUE); // Для обработки прерывания через ctrl-c
	srand(time(0));

	wchar_t* fileName;
	const wchar_t* directoryPath = L"..\\..\\HT";
	std::wstring filePath(L"..\\HT\\test2.ht");
	std::wstring HTUsers = L"";
	std::wstring HTPassword = L"";

	HANDLE hStopEvent = CreateEventW(NULL,
		TRUE, //FALSE - автоматический сброс; TRUE - ручной
		FALSE,
		L"Global\\HTStopExist");
	try {
		if (argv[1]) {
			fileName = GetWC(argv[1]);
			std::wstring s(directoryPath);
			s += L"\\" + std::wstring(fileName);
			filePath = s;
			if (argc == 4)
			{
				HTUsers = GetWC(argv[2]);
				HTPassword = GetWC(argv[3]);
			}
		}

		filePath = GetWC(argv[1]);
		trace(std::string(filePath.begin(), filePath.end()).c_str());

		ht = HT_LIB::Init();


		if (HTUsers.empty())
		{
			HT = HT_LIB::HT::OpenExist(ht, filePath.c_str());
			if (HT == NULL)
				throw "Invalid handle";
		}
		else
		{
			HT = HT_LIB::HT::OpenExist(ht, HTUsers, HTPassword, filePath);
			if (HT == NULL)
				throw "Invalid handle";
		}


		while (true)
		{
			int numberKey = rand() % 50 + 1;
			std::string key{};

			std::stringstream convert;
			convert << numberKey;
			key = convert.str();

			Element* el = HT_LIB::HT::CreateNewFullElement(ht, key.c_str(), key.length() + 1, "0", 2);

			bool success = true;
			if (!HT_LIB::HT::Insert(ht, HT, el))
			{
				std::cout << HT_LIB::HT::GetLastError(ht, HT) << std::endl;
				success = false;
			}
			else
				HT_LIB::HT::print(ht, el);

			std::tuple<Element*, bool, std::wstring, std::wstring> pr{ el,success, filePath, std::wstring(L"Insert") };
			hThread = CreateThread(NULL, 0, HT_LIB::HT::RecordingWork, &pr, 0, NULL);

			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);

			if (WaitForSingleObject(hStopEvent, 0) != WAIT_TIMEOUT) {
				HT_LIB::HT::Close(ht, HT);
				HT_LIB::Dispose(ht);
				return 0;
			}

			Sleep(1000);
		}
		return 0;
	}
	catch (const char* err)
	{
		std::cout << err << std::endl;
		return 1;
	}
}