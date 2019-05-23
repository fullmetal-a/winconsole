#include "winconsole.h"
#include <stdexcept>

/*

	HINSTANCE				m_hInst;
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

*/

enum class ErrorCode
{
	hInstInitErr = 0,
	InitError,
	UnknownStyle,
	RegClassErr,
	EmptyClassName
};

constexpr char* _errMsgs[] =
{
	"Error: Cannot initialize CWinConsole window, hInstance is NULL.",
	"Error: Cannot create CWinConsole instance. WinConsole system is not initialized. You forgot to call CWinConsole::InitWinConsoleSystem() in main().",
	"Error: Unknown window style.",
	"Error: Failed to register window class.",
	"Error: Failed to generate class name."
};

inline const char* _errText(ErrorCode code)
{
	return _errMsgs[static_cast<size_t>(code)];
}

#define WC_ERR(code) std::runtime_error(::_errText(::ErrorCode::code))
bool				 CWinConsole::s_bWCSystemInitialized = false;
typedef				 std::pair<CWinConsole*, CWinConsole::WinConsoleHandle> WCPair;

LRESULT CALLBACK _WCWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return NULL;
}

CWinConsole::CWinConsole()
{
	if (!s_bWCSystemInitialized)
		throw WC_ERR(InitError);

	m_Handle = _registerWC(this);
}

CWinConsole::CWinConsole(const TCHAR* title, bool allowResize, Style style)
{
	if (!s_bWCSystemInitialized)
		throw WC_ERR(InitError);

	m_Handle = _registerWC(this);
}

CWinConsole::CWinConsole(const CWinConsole& copy)
{
	if (!s_bWCSystemInitialized)
		throw WC_ERR(InitError);
}

CWinConsole::~CWinConsole()
{

}

CWinConsole::WinConsoleHandle CWinConsole::_registerWC(CWinConsole* ptr)
{
	WinConsoleHandle handle = *(s_Handles.rbegin()) + 1;
	s_Handles.insert(handle);
	s_HandleMap.insert(WCPair(ptr, handle));
	return handle;
}

std::string CWinConsole::_generateClassName(WinConsoleHandle handle, RandomEngine& erand)
{
	char lpzHandle[16];
	sprintf_s(lpzHandle, sizeof(lpzHandle), "%u", handle);
	size_t llen = erand.GetRandomBetween<size_t>(2, 32);
	std::string cName;
	cName.resize(llen + 1, '\0');

	for (size_t i = 0; i < llen; i++)
	{
		unsigned char c = erand.GetRandomBetween<unsigned char>(1, 255);
		cName[i] = *reinterpret_cast<char*>(&c);
	}

	cName += lpzHandle;
	return cName;
}

void CWinConsole::InitWinConsoleSystem()
{
	if (s_bWCSystemInitialized)
		return;

	s_hInst = GetModuleHandle(NULL);
	if (!s_hInst)
		throw WC_ERR(InitError);

	s_Handles.insert(0);
	s_HandleMap.insert(WCPair(nullptr, 0));
	InitializeCriticalSection(&s_CS);
	s_bWCSystemInitialized = true;
}

void CWinConsole::_initConsoleWindow(const TCHAR* title, bool allowResize)
{
	HBRUSH color;
	m_sWinClassName = _generateClassName(m_Handle, m_rand);

	if (m_sWinClassName.length() == 0)
		throw WC_ERR(EmptyClassName);

	switch (m_Style)
	{
		case Style::Regular:
			color = (HBRUSH)COLOR_WINDOW;
			break;

		case Style::Dark:
			color = CreateSolidBrush(RGB(25,25,25));
			break;

		default:
			throw WC_ERR(UnknownStyle);
	}

	m_wcWndClass.cbSize = sizeof(WNDCLASSEX);
	m_wcWndClass.style = CS_HREDRAW | CS_VREDRAW;
	m_wcWndClass.lpfnWndProc = _WCWndProc;
	m_wcWndClass.cbClsExtra = 0;
	m_wcWndClass.cbWndExtra = DLGWINDOWEXTRA;
	m_wcWndClass.hInstance = s_hInst;
	m_wcWndClass.hIcon = NULL;
	m_wcWndClass.hCursor = NULL;
	m_wcWndClass.hbrBackground = color;
	m_wcWndClass.lpszMenuName = NULL;
	m_wcWndClass.lpszClassName = m_sWinClassName.c_str();
	m_wcWndClass.hIconSm = NULL;

	if (!RegisterClassEx(&m_wcWndClass))
		throw WC_ERR(RegClassErr);
}