//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2016 Media Design School.
//
// File Name	: 
// Description	: 
// Author		: Your Name
// Mail			: your.name@mediadesign.school.nz
//

//Library Includes
#include <windows.h>
#include <windowsx.h>

//Local Includes
#include "Game.h"
#include "Clock.h"
#include "utils.h"
#include "resource.h"
#include "level.h"

//Extra functionality for reading/writing to Debug Box
template<typename T>
std::wstring ToWideString(const T& _value)
{
	std::wstringstream theStream;
	theStream << _value << std::ends;
	return (theStream.str());
}

float ReadFromEditBox(HWND _hDlg, int _iResourceID)
{
	wchar_t _pcValue[10];
	ZeroMemory(_pcValue, 10);
	GetDlgItemText(_hDlg, _iResourceID, _pcValue, 10);
	if (_pcValue[0] == 0)
	{
		return 0.0f;
	}
	else
	{
		return static_cast<float>(_wtof(_pcValue));
	}
}

void WriteToEditBox(HWND _hDlg, int _iResourceID, float _fValue)
{
	std::wstring _strValue = ToWideString(_fValue);
	SetDlgItemText(_hDlg, _iResourceID, _strValue.c_str());
}

const int kiWidth = 500;
const int kiHeight = 600;

bool g_bDebugWindowOpen;

HWND g_hDebugMenu;

#define WINDOW_CLASS_NAME L"BSENGGFRAMEWORK"

LRESULT CALLBACK
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uiMsg)
	{
	case WM_CREATE:
	{
		g_bDebugWindowOpen = false;
	}
		break;
	case WM_KEYDOWN:
	{
		switch (LOWORD(_wParam))
		{
		case(VK_ESCAPE):
		{
			g_bDebugWindowOpen = !g_bDebugWindowOpen;

			if (!g_bDebugWindowOpen)
			{
				CGame::GetInstance().GetClock()->ResetDeltaTick();
				ShowWindow(g_hDebugMenu, SW_HIDE);
			}
			else //Open up the window
			{
				ShowWindow(g_hDebugMenu, SW_SHOWNORMAL);
			}
			break;
		}
		default:
			break;
		}
	}
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return(0);
	}
		break;

	default:break;
	}

	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

HWND CreateAndRegisterWindow(HINSTANCE _hInstance, int _iWidth, int _iHeight, const wchar_t* _pcTitle)
{
	WNDCLASSEX winclass;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = static_cast<HBRUSH> (GetStockObject(NULL_BRUSH));
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&winclass))
	{
		// Failed to register.
		return (0);
	}

	HWND hwnd;
	hwnd = CreateWindowEx(NULL,
		WINDOW_CLASS_NAME,
		_pcTitle,
		WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		_iWidth, _iHeight,
		NULL,
		NULL,
		_hInstance,
		NULL);

	if (!hwnd)
	{
		// Failed to create.
		return (0);
	}

	return (hwnd);
}

BOOL CALLBACK DebugDlgProc(HWND _hwnd,
	UINT _msg,
	WPARAM _wparam,
	LPARAM _lparam)
{
	switch (_msg)
	{
	case WM_SHOWWINDOW:	//Make sure to show current stats
	{
		
		WriteToEditBox(_hwnd, IDC_EDIT1, CGame::GetInstance().GetLevel()->GetPlayerMoveSpeed());
		WriteToEditBox(_hwnd, IDC_EDIT2, CGame::GetInstance().GetLevel()->GetPlayerShootCooldown());

		WriteToEditBox(_hwnd, IDC_EDIT3, CGame::GetInstance().GetLevel()->GetInvaderShootCooldown());
		WriteToEditBox(_hwnd, IDC_EDIT4, CGame::GetInstance().GetLevel()->GetInvaderMoveSpeedX());
		WriteToEditBox(_hwnd, IDC_EDIT5, CGame::GetInstance().GetLevel()->GetInvaderMoveSpeedY());

		WriteToEditBox(_hwnd, IDC_EDIT6, CGame::GetInstance().GetLevel()->GetSpecialShipCooldown());

		WriteToEditBox(_hwnd, IDC_EDIT7, CGame::GetInstance().GetLevel()->GetPlayerBulletSpeed());
		WriteToEditBox(_hwnd, IDC_EDIT8, CGame::GetInstance().GetLevel()->GetInvaderBulletSpeed());
		
		return TRUE;
		break;
	}
	case WM_CLOSE:
	{
		ShowWindow(_hwnd, SW_HIDE);
		g_bDebugWindowOpen = false;
		CGame::GetInstance().GetClock()->ResetDeltaTick();

		return TRUE;
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(_wparam))
		{
		case IDOK: //Apply changes
		{
			CGame::GetInstance().GetLevel()->SetPlayerMoveSpeed(ReadFromEditBox(_hwnd, IDC_EDIT1));
			CGame::GetInstance().GetLevel()->SetPlayerShootCooldown(ReadFromEditBox(_hwnd, IDC_EDIT2));

			CGame::GetInstance().GetLevel()->SetInvaderShootCooldown(ReadFromEditBox(_hwnd, IDC_EDIT3));
			CGame::GetInstance().GetLevel()->SetInvaderMoveSpeedX(ReadFromEditBox(_hwnd, IDC_EDIT4));
			CGame::GetInstance().GetLevel()->SetInvaderMoveSpeedY(ReadFromEditBox(_hwnd, IDC_EDIT5));

			CGame::GetInstance().GetLevel()->SetSpecialShipCooldown(ReadFromEditBox(_hwnd, IDC_EDIT6));

			CGame::GetInstance().GetLevel()->SetPlayerBulletSpeed(ReadFromEditBox(_hwnd, IDC_EDIT7));
			CGame::GetInstance().GetLevel()->SetInvaderBulletSpeed(ReadFromEditBox(_hwnd, IDC_EDIT8));

			MessageBox(_hwnd, L"Changes have been made!", L"Woo.", MB_OK);
			return TRUE;
			break;
		}
		}

		return TRUE;
		break;
	}
	}
	return FALSE;
}

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline, int _iCmdshow)
{
	MSG msg;
	RECT _rect;
	ZeroMemory(&msg, sizeof(MSG));


	HWND hwnd = CreateAndRegisterWindow(_hInstance, kiWidth, kiHeight, L"Unhappy Invaders");

	CGame& rGame = CGame::GetInstance();
	
	GetClientRect(hwnd, &_rect);

	//if (!rGame.Initialise(_hInstance, hwnd, kiWidth, kiHeight))
	if (!rGame.Initialise(_hInstance, hwnd, _rect.right, _rect.bottom))
	{
		// Failed
		return (0);
	}

	//Create debug box
	g_hDebugMenu = CreateDialog(_hInstance, MAKEINTRESOURCE(IDD_DebugMenu), hwnd, DebugDlgProc);

	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (g_hDebugMenu == 0 || !IsDialogMessage(g_hDebugMenu, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if (!g_bDebugWindowOpen)
		{
			rGame.ExecuteOneFrame();
		}
	}

	CGame::DestroyInstance();

	return (static_cast<int>(msg.wParam));
}
