#pragma once
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <list>
#include <set>
#include <map>
#include "random.h"

#define private_static private
#define public_static  public

class CWinConsole
{
public:
	enum class Style
	{
		Regular,
		Dark
	};

	typedef unsigned int								WinConsoleHandle;
	typedef std::set<WinConsoleHandle>					WCHandles;
	typedef std::map<CWinConsole*, WinConsoleHandle>	WCHandleMap;

private:
	WNDCLASSEX				m_wcWndClass;
	HWND					m_hMainWnd;
	HWND					m_hText;
	HWND					m_hInput;
	bool					m_bOwner;
	bool					m_bAllowResize;
	std::string				m_sText;
	std::list<std::string>	m_lsLastInput;
	HANDLE					m_hThread;
	CRITICAL_SECTION		m_CS;
	Style					m_Style;
	WinConsoleHandle		m_Handle;
	std::string				m_sWinClassName;
	RandomEngine			m_rand;

	void _initConsoleWindow(const TCHAR* title, bool allowResize);

private_static:
	static HINSTANCE		s_hInst;
	static bool				s_bWCSystemInitialized;
	static WCHandles		s_Handles;
	static WCHandleMap		s_HandleMap;
	static CRITICAL_SECTION	s_CS;

	static WinConsoleHandle _registerWC(CWinConsole* ptr);
	static void _closeWCHandle(WinConsoleHandle handle);
	static std::string _generateClassName(WinConsoleHandle handle, RandomEngine& erand);

public_static:
	static void InitWinConsoleSystem();
	static CWinConsole* GetWinConsoleByHandle(WinConsoleHandle handle);

public:
	CWinConsole();
	CWinConsole(const TCHAR* title, bool allowResize = false, Style style = Style::Regular);
	CWinConsole(const CWinConsole& copy);
	~CWinConsole();

};