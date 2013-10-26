//==========================================================================
#ifndef LLK_H
#define LLK_H
//==========================================================================
#pragma once
//==========================================================================
#include <Windows.h>
#include <tchar.h>
//==========================================================================

//win7 32位下的
#define	WIN732_ARBS_ADDR	(DWORD)0x0012A444	//二维数组基址
#define WIN732_REAG_ADDR	(DWORD)0x0011821E	//重列存放地址

//win7 64位
#define	WIN764_ARBS_ADDR	(DWORD)0x0018A444
#define WIN764_REAG_ADDR	(DWORD)0x0017821E

//xp 32位
#define	XP32_ARBS_ADDR		(DWORD)0x0012A480
#define XP32_REAG_ADDR		(DWORD)0x0011825A

#define ROW		11	//行
#define COL		19	//列

//==========================================================================
inline HWND TargetWindow()	{	return FindWindow(NULL, _T("QQ游戏 - 连连看角色版"));	}

bool ReadArray();
bool IsLineConnection(int x1, int y1, int x2, int y2);		//直线两点是不是可直达
bool IsOneCornerConnection(int x1, int y1, int x2, int y2);	//是否是一个转角连通
bool IsTwoCornerConnection(int x1, int y1, int x2, int y2);	//是否是两个转角连通
bool CanClear(int x1, int y1, int x2, int y2);				//(x1, y1)、(x2, y2)是否可消除
void Clear(int x1, int y1, int x2, int y2);					//清除坐标为(x1, y1)和(x2, y2)的两幅图
bool IsEmptyArray();				//是否已消除完图案

bool DoOneConnect();					//返回当次操作是否成功消除，false表示无解
void DoRearrange();						//重列
int	 GetRearrangeCount();				//获取重列次数
bool SetRearrangeCount(int num);		//设置重列次数

//==========================================================================
#endif
////////////////////////////////////////////////////////////////////////////