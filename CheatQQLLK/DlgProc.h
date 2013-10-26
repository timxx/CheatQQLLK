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

void SetDlgIcon(HWND hDlg, UINT uID);	//���öԻ���ͼ��
BOOL OnInitDlg(HWND hDlg, HWND hwndFocus, LPARAM lParam);
void OnCommand(HWND hDlg, int id, HWND hwndCtl, UINT codeNotify);
void OnHotKey(HWND hDlg, int idHotKey, UINT fuModifiers, UINT vk);	//WM_HOTKEY
void OnKeyDown(HWND hDlg, UINT vk, BOOL fDown, int cRepeat, UINT flags);
void OnTimer(HWND hDlg, UINT id);

void OnButtonClicked(HWND hDlg);	//����[������]������
void OnClickedCheck1(HWND hDlg);	//[�Զ�]
void OnClickedCheck2(HWND hDlg);	//[�ö�]

void ShowControls(HWND hDlg, BOOL bShow = TRUE);	//��ʾ/����ʱ�����Ǽ����ؼ�

bool EnableDebugPrivilege();			//����ΪdebugȨ�ޣ���XP��Ҫ
bool IsVistaOrLater();					//��ǰϵͳ�Ƿ���Vista�����߰汾
BOOL IsWow64();							//�Ƿ���64λ

DWORD WINAPI AutoConnectProc(LPVOID lParam);	//�Զ����߳�

//==========================================================================
#endif
////////////////////////////////////////////////////////////////////////////