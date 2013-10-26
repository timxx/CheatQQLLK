//==========================================================================
#ifndef DLG_PROC_H
#define DLG_PROC_H
//==========================================================================
#pragma once
//==========================================================================
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>

#include "resource.h"
//==========================================================================
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

void SetDlgIcon(HWND hDlg, UINT uID);	//设置对话框图标
BOOL OnInitDlg(HWND hDlg, HWND hwndFocus, LPARAM lParam);
void OnCommand(HWND hDlg, int id, HWND hwndCtl, UINT codeNotify);
void OnHotKey(HWND hDlg, int idHotKey, UINT fuModifiers, UINT vk);	//WM_HOTKEY
void OnKeyDown(HWND hDlg, UINT vk, BOOL fDown, int cRepeat, UINT flags);
void OnTimer(HWND hDlg, UINT id);

void OnButtonClicked(HWND hDlg);	//单击[我来连]处理函数
void OnClickedCheck1(HWND hDlg);	//[自动]
void OnClickedCheck2(HWND hDlg);	//[置顶]

void ShowControls(HWND hDlg, BOOL bShow = TRUE);	//显示/隐藏时间间隔那几个控件

bool EnableDebugPrivilege();			//提升为debug权限，在XP需要
bool IsVistaOrLater();					//当前系统是否是Vista及更高版本
BOOL IsWow64();							//是否是64位

DWORD WINAPI AutoConnectProc(LPVOID lParam);	//自动连线程

//==========================================================================
#endif
////////////////////////////////////////////////////////////////////////////