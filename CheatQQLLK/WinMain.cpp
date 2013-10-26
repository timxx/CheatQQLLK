//==========================================================================
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' "\
	"version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//==========================================================================
#include "DlgProc.h"

//==========================================================================

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPTSTR lpCmdLine, int nCmdShow)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
	return 0;
}
//==========================================================================