//==========================================================================
#include "LLK.h"
#include "DlgProc.h"
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

//==========================================================================
#define HANDLE_DLGMSG(hwnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hwnd, uMsg,     \
      HANDLE_##message((hwnd), (wParam), (lParam), (fn))))

#define TIMERID	1

extern DWORD dwArrayAddr;
extern DWORD dwReargAddr;

bool fThreadStarted = false;
HANDLE hThread = NULL;
DWORD dwThreadId = 0;
//==========================================================================
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		HANDLE_DLGMSG(hDlg, WM_INITDIALOG,	OnInitDlg);
		HANDLE_DLGMSG(hDlg, WM_COMMAND,		OnCommand);
		HANDLE_DLGMSG(hDlg, WM_HOTKEY,		OnHotKey);
		HANDLE_DLGMSG(hDlg, WM_KEYDOWN,		OnKeyDown);
		HANDLE_DLGMSG(hDlg, WM_TIMER,		OnTimer);
	}
	return FALSE;
}


void SetDlgIcon(HWND hDlg, UINT uID)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE);
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(uID));

	SendMessage(hDlg, WM_SETICON, TRUE,  (LPARAM)hIcon);
	SendMessage(hDlg, WM_SETICON, FALSE, (LPARAM)hIcon);
}

BOOL OnInitDlg(HWND hDlg, HWND hwndFocus, LPARAM lParam)
{
	InitCommonControls();//important for XP!!!

	SetDlgIcon(hDlg, IDI_APP);

	SetDlgItemInt(hDlg, IDC_EDIT1, 1000, TRUE);
	CheckDlgButton(hDlg, IDC_CHECK2, BST_CHECKED);
	ShowControls(hDlg, FALSE);

	EnableDebugPrivilege();

	RegisterHotKey(hDlg, 0x0100, 0, VK_F6);
	RegisterHotKey(hDlg, 0x0110, 0, VK_F7);
	RegisterHotKey(hDlg, 0x0111, 0, VK_F8);

	dwArrayAddr = IsVistaOrLater() ?
		(IsWow64() ? WIN764_ARBS_ADDR : WIN732_ARBS_ADDR) :
		XP32_ARBS_ADDR;
	dwReargAddr = IsVistaOrLater() ?
		(IsWow64() ? WIN764_REAG_ADDR : WIN732_REAG_ADDR) :
		XP32_REAG_ADDR;

	SetTimer(hDlg, TIMERID, 100, NULL);

	return TRUE;
}

void OnCommand(HWND hDlg, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDCANCEL:
		EndDialog(hDlg, id);
		break;

	case IDC_BUTTON1:
		OnButtonClicked(hDlg);
		break;

	case IDC_CHECK1:
		SetForegroundWindow(hDlg);//使窗口置前，可以直接输入时间
		OnClickedCheck1(hDlg);
		break;

	case IDC_CHECK2:
		OnClickedCheck2(hDlg);
		break;
	}
}

void OnHotKey(HWND hDlg, int idHotKey, UINT fuModifiers, UINT vk)
{
	if ( vk == VK_F6)
	{
		OnButtonClicked(hDlg);
	}
	else if (vk == VK_F7)
	{
		if (IsWindowEnabled(GetDlgItem(hDlg, IDC_CHECK1)))
			SendDlgItemMessage(hDlg, IDC_CHECK1, BM_CLICK, 0, 0);
	}
	else if (vk == VK_F8)
	{
		SendDlgItemMessage(hDlg, IDC_CHECK2, BM_CLICK, 0, 0);
	}
}

void OnKeyDown(HWND hDlg, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	if (vk == VK_RETURN)
	{
		if (IsWindowEnabled(GetDlgItem(hDlg, IDC_BUTTON1)))
			SendDlgItemMessage(hDlg, IDC_BUTTON1, BM_CLICK, 0, 0);
	}
}

void OnTimer(HWND hDlg, UINT id)
{
	if (id == TIMERID)
	{
		if (!TargetWindow())
		{
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), FALSE);
			if (IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED)
				EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
		}
		else
		{
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);
			EnableWindow(GetDlgItem(hDlg, IDC_CHECK1), TRUE);

			if (!fThreadStarted &&
				IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED)
				EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		}
	}
}

void OnButtonClicked(HWND hDlg)
{
	if (fThreadStarted)
	{
		TerminateThread(hThread, 0);
		fThreadStarted = false;
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		SetWindowText(GetDlgItem(hDlg, IDC_BUTTON1), _T("我来连\r\n(F6)"));

		return ;
	}

	if (IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED)
	{
		fThreadStarted = true;
		SetWindowText(GetDlgItem(hDlg, IDC_BUTTON1), _T("停止\r\n(F6)"));
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), FALSE);
		hThread = CreateThread(NULL, 0, AutoConnectProc, (LPVOID)hDlg, 0, &dwThreadId);
	}
	else
	{
		bool canGo = DoOneConnect();
		if (!canGo)	//无解
		{
			if (!IsEmptyArray())//且游戏未结束，，使用重列道具
			{
				DoRearrange();
				DoOneConnect();
			}
		}
	}
}
void OnClickedCheck1(HWND hDlg)
{
	if (IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED)
	{
		ShowControls(hDlg);
		SetFocus(GetDlgItem(hDlg, IDC_EDIT1));
		SendMessage(GetDlgItem(hDlg, IDC_EDIT1), EM_SETSEL, 0, -1);
	}
	else
	{
		EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
		ShowControls(hDlg, FALSE);
		SetFocus(GetDlgItem(hDlg, IDC_BUTTON1));

		if (fThreadStarted)
		{
			TerminateThread(hThread, 0);
			fThreadStarted = false;
			EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
			SetWindowText(GetDlgItem(hDlg, IDC_BUTTON1), _T("我来连\r\n(F6)"));
			return ;
		}
	}
}

void OnClickedCheck2(HWND hDlg)
{
	if(IsDlgButtonChecked(hDlg, IDC_CHECK2) == BST_CHECKED)
	{
		SetWindowPos(hDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		BringWindowToTop(hDlg);
	}
	else
		SetWindowPos(hDlg, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void ShowControls(HWND hDlg, BOOL bShow)
{
	for (int id = IDC_EDIT1; id<=IDS_MS; id++)
		ShowWindow(GetDlgItem(hDlg, id), bShow ? SW_SHOW : SW_HIDE);
}

bool EnableDebugPrivilege()
{
	HANDLE hToken;
	HANDLE hProcess = GetCurrentProcess();

	if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) )
		return false;

	TOKEN_PRIVILEGES tkp;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid) )
		return false;

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0))
	{
		CloseHandle(hToken);
		return false;
	}
	CloseHandle(hToken);
	return true;
}

bool IsVistaOrLater()
{
	OSVERSIONINFO osvi = {0};
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (!GetVersionEx(&osvi))
		return false;

	return osvi.dwMajorVersion >= 6;
}

BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;
	if (!IsWow64Process(GetCurrentProcess(),&bIsWow64))
		return FALSE;

	return bIsWow64;
}

DWORD WINAPI AutoConnectProc(LPVOID lParam)
{
	HWND hDlg = (HWND)lParam;
	DWORD dwTime = GetDlgItemInt(hDlg, IDC_EDIT1, NULL, TRUE);

	dwTime = dwTime < 0? 0 : dwTime;

	if (!ReadArray())	return 0;

	while(!IsEmptyArray())
	{
		//要是关了那个东东还做干吗
		if (!TargetWindow())
			break;

		if (!DoOneConnect() && !IsEmptyArray())
			DoRearrange();	//无解，使用重列道具

		Sleep(dwTime);
	}
	//结束，还原原来状态
	fThreadStarted = false;
	EnableWindow(GetDlgItem(hDlg, IDC_EDIT1), TRUE);
	SetWindowText(GetDlgItem(hDlg, IDC_BUTTON1), _T("我来连\r\n(F6)"));

	return 1;
}
////////////////////////////////////////////////////////////////////////////