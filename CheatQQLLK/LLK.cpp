//==========================================================================
#include "LLK.h"
//==========================================================================
DWORD dwArrayAddr = 0;	//��ά�����ַ
DWORD dwReargAddr = 0;	//�����ڴ��ַ

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

	if(x1 == x2)   //ͬһ��
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
	if(y1 == y2) //ͬһ��
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
	if( array[x1][y2] == 0 &&					//(x1, y1)��(x1, y2)����ֱ��
		IsLineConnection(x1, y1 , x1, y2) &&	//(x1, y1)��(x1, y2)�Ƿ����ͨ
		IsLineConnection(x2, y2 , x1, y2)		//(x2, y2)��(x1, y2)�Ƿ����ͨ
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
	//����ɨ��
	for(i=y1 -1; i>=0; --i)
	{
		if(array[x1][i] != 0)
			break;
		//�˵����(x2, y2)����һ��ת����ͨ
		//��(x1, y1)��(x2, y2)�ɾ�������ת����ͨ
		if(IsOneCornerConnection( x1, i , x2, y2))
			return true;
	}
	//����ɨ��
	for(i = x1 - 1; i>=0; --i)
	{
		if(array[i][y1] != 0)
			break;
		if(IsOneCornerConnection(i, y1 ,x2, y2))
			return true;
	}
	//����ɨ��
	for(i = y1 + 1; i<COL; ++i)
	{
		if(array[x1][i] !=0 )
			break;
		if(IsOneCornerConnection( x1, i , x2, y2))
			return true;
	}
	//����ɨ��
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
			MessageBox(GetActiveWindow(), _T("�����ʹ���˹���ԱȨ��������Ϸ�����ù���ԱȨ�������д���ң�"),\
				_T("Ȩ�޹��͵���ʧ�ܣ�"), MB_ICONINFORMATION);
		}
		return false;
	}
	for (int x1=0; x1<ROW; x1++)
	{
		for (int y1=0; y1<COL; y1++)
		{
			if (array[x1][y1] == 0)
				continue;
			//������һ����(x1, y1)��ͬ��ͼ��
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
	else if ( count== 0)	//���д�������ʱ�Զ�����1
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