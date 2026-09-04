#include "Application.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	if (!APPLICATION->Initialize())
	{
		MessageBox(NULL, L"Failed to initialize application", L"ERROR", MB_OK);
		APPLICATION->Shutdown();
		return 0;
	}

	APPLICATION->Run();

	APPLICATION->Shutdown();
	
	return 0;
}
