//==========================================================================
#ifndef LLK_H
#define LLK_H
//==========================================================================
#pragma once
//==========================================================================
#include <Windows.h>
#include <tchar.h>
//==========================================================================

//win7 32λ�µ�
#define	WIN732_ARBS_ADDR	(DWORD)0x0012A444	//��ά�����ַ
#define WIN732_REAG_ADDR	(DWORD)0x0011821E	//���д�ŵ�ַ

//win7 64λ
#define	WIN764_ARBS_ADDR	(DWORD)0x0018A444
#define WIN764_REAG_ADDR	(DWORD)0x0017821E

//xp 32λ
#define	XP32_ARBS_ADDR		(DWORD)0x0012A480
#define XP32_REAG_ADDR		(DWORD)0x0011825A

#define ROW		11	//��
#define COL		19	//��

//==========================================================================
inline HWND TargetWindow()	{	return FindWindow(NULL, _T("QQ��Ϸ - ��������ɫ��"));	}

bool ReadArray();
bool IsLineConnection(int x1, int y1, int x2, int y2);		//ֱ�������ǲ��ǿ�ֱ��
bool IsOneCornerConnection(int x1, int y1, int x2, int y2);	//�Ƿ���һ��ת����ͨ
bool IsTwoCornerConnection(int x1, int y1, int x2, int y2);	//�Ƿ�������ת����ͨ
bool CanClear(int x1, int y1, int x2, int y2);				//(x1, y1)��(x2, y2)�Ƿ������
void Clear(int x1, int y1, int x2, int y2);					//�������Ϊ(x1, y1)��(x2, y2)������ͼ
bool IsEmptyArray();				//�Ƿ���������ͼ��

bool DoOneConnect();					//���ص��β����Ƿ�ɹ�������false��ʾ�޽�
void DoRearrange();						//����
int	 GetRearrangeCount();				//��ȡ���д���
bool SetRearrangeCount(int num);		//�������д���

//==========================================================================
#endif
////////////////////////////////////////////////////////////////////////////