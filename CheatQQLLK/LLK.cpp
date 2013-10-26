//==========================================================================
#include "LLK.h"
//==========================================================================
DWORD dwArrayAddr = 0;	//二维数组基址
DWORD dwReargAddr = 0;	//重列内存地址

unsigned char array[ROW][COL] = {0};
//==========================================================================
bool ReadArray()
{
	HWND hWndLLK = TargetWindow();
	if (!hWndLLK)	return false;

	DWORD dwPID = 0;
	GetWindowThreadProcessId(hWndLLK, &dwPID);

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!hProcess)	return false;

	if (!ReadProcessMemory(hProcess, (LPCVOID)dwArrayAddr, (LPVOID)array, ROW*COL, NULL))
		return false;

	CloseHandle(hProcess);

	return true;
}


bool IsLineConnection(int x1, int y1, int x2, int y2)
{
	int i,temp;

	if (!(x1 == x2 || y1 == y2))
		return false;

	if(x1 == x2)   //同一行
	{   
		temp = abs(y2 - y1);
		if(y2 > y1)
		{
			for(i=1; i < temp; i++)
				if(array[x1][y1+i] !=0 )
					return false;
		}
		else
		{
			for(i=1; i < temp; i++)
				if(array[x1][y1-i] !=0 )
					return false;
		}
	}
	if(y1 == y2) //同一列
	{
		temp = abs(x2 - x1);
		if(x2 > x1)
		{
			for(i=1; i<temp; i++)
				if(array[x1+i][y1] !=0 )
					return false;
		}
		else
		{
			for(i=1; i<temp; i++)
				if(array[x1-i][y1] !=0 )
					return false;
		}
	}
	return true;
}

bool IsOneCornerConnection(int x1, int y1, int x2, int y2)
{	
	if( array[x1][y2] == 0 &&					//(x1, y1)、(x1, y2)连成直线
		IsLineConnection(x1, y1 , x1, y2) &&	//(x1, y1)、(x1, y2)是否可连通
		IsLineConnection(x2, y2 , x1, y2)		//(x2, y2)、(x1, y2)是否可连通
		)
		return true;

	if( array[x2][y1] == 0 &&
		IsLineConnection(x1, y1 , x2, y1) &&
		IsLineConnection(x2, y2 , x2, y1)
		)
		return true;

	return false ;
}

bool IsTwoCornerConnection(int x1, int y1, int x2, int y2)
{
	int i ; 
	//往左扫描
	for(i=y1 -1; i>=0; --i)
	{
		if(array[x1][i] != 0)
			break;
		//此点可与(x2, y2)经过一个转角连通
		//则(x1, y1)、(x2, y2)可经过两个转角连通
		if(IsOneCornerConnection( x1, i , x2, y2))
			return true;
	}
	//往上扫描
	for(i = x1 - 1; i>=0; --i)
	{
		if(array[i][y1] != 0)
			break;
		if(IsOneCornerConnection(i, y1 ,x2, y2))
			return true;
	}
	//往右扫描
	for(i = y1 + 1; i<COL; ++i)
	{
		if(array[x1][i] !=0 )
			break;
		if(IsOneCornerConnection( x1, i , x2, y2))
			return true;
	}
	//往下扫描
	for( i = x1 + 1; i<ROW ; ++i)
	{
		if(array[i][y1] !=0 )
			break;
		if(IsOneCornerConnection(i, y1 ,x2, y2))
			return true;
	}
	return false;
}

bool CanClear(int x1, int y1, int x2, int y2)
{
	if(array[x1][y1] != array[x2][y2])
		return false;

	if(IsLineConnection(x1, y1, x2, y2))
		return true;

	if(IsOneCornerConnection(x1, y1, x2, y2) || IsTwoCornerConnection(x1, y1, x2, y2))
		return true;
	return false;
}

void Clear(int x1, int y1, int x2, int y2)
{
	const int x = 25;
	const int y = 195;

	HWND hWndLLK = TargetWindow();
	if (!hWndLLK)	return ;

	SendMessage(hWndLLK, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x + y1*31, y + x1*35));
	SendMessage(hWndLLK, WM_LBUTTONUP, 0, MAKELPARAM(x + y1*31, y + x1*35));

	SendMessage(hWndLLK, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x + y2*31, y + x2*35));
	SendMessage(hWndLLK, WM_LBUTTONUP, 0, MAKELPARAM(x + y2*31, y + x2*35));
}

bool IsEmptyArray()
{
	for (int i=0; i<ROW; i++)
		for (int j=0; j<COL; j++)
			if (array[i][j] != 0)
				return false;
	return true;
}

bool DoOneConnect()
{
	if (!ReadArray())
	{
		if (GetLastError() == 5)
		{
			MessageBox(GetActiveWindow(), _T("如果你使用了管理员权限运行游戏，请用管理员权限来运行此外挂！"),\
				_T("权限过低导致失败！"), MB_ICONINFORMATION);
		}
		return false;
	}
	for (int x1=0; x1<ROW; x1++)
	{
		for (int y1=0; y1<COL; y1++)
		{
			if (array[x1][y1] == 0)
				continue;
			//查找下一个与(x1, y1)相同的图案
			for (int x2=x1; x2<ROW; x2++)
			{
				for (int y2=0; y2<COL; y2++)
				{
					if ((x1==x2 && y1==y2) || array[x2][y2] == 0)
						continue;

					if (array[x1][y1] != array[x2][y2])
						continue;

					if (CanClear(x1, y1, x2, y2))
					{
						Clear(x1, y1, x2, y2);
						return true;
					}
				}
			}
		}
	}
	return false;
}

void DoRearrange()
{
	HWND hWnd = TargetWindow();
	if (!hWnd)	return ;

	int count = GetRearrangeCount();
	if ( count == -1)
		return ;
	else if ( count== 0)	//重列次数用完时自动加上1
		SetRearrangeCount(1);

	RECT rect;
	GetWindowRect(hWnd, &rect);

	SendMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(655, 195));
	SendMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(655, 195));
}

int GetRearrangeCount()
{
	HWND hWndLLK = TargetWindow();
	if (!hWndLLK)	return -1;

	DWORD dwPID = 0;
	GetWindowThreadProcessId(hWndLLK, &dwPID);


	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!hProcess)	return -1;

	unsigned char count = 0;
	if (!ReadProcessMemory(hProcess, (LPCVOID)dwReargAddr, (LPVOID)&count, sizeof(unsigned char), NULL))
	{
		CloseHandle(hProcess);
		return -1;
	}

	CloseHandle(hProcess);

	return count;
}

bool SetRearrangeCount(int num)
{
	HWND hWndLLK = TargetWindow();
	if (!hWndLLK)	return false;

	DWORD dwPID = 0;
	GetWindowThreadProcessId(hWndLLK, &dwPID);


	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
	if (!hProcess)	return false;

	unsigned char count = 0;
	if (!WriteProcessMemory(hProcess, (LPVOID)dwReargAddr, (LPCVOID)&num, sizeof(unsigned char), NULL))
	{
		CloseHandle(hProcess);
		return false;
	}

	CloseHandle(hProcess);

	return true;
}