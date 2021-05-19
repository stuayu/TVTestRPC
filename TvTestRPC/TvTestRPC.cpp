// TvTestRPC.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include <ctime>
#include <shlwapi.h>
#include "resource.h"
#include <vector>
#pragma comment(lib,"shlwapi.lib")

#define TvTestRPC_WINDOW_CLASS TEXT("TvTestRPC Window")
#define TvTestRPC_TIMER_ID 1

class CMyPlugin : public TVTest::CTVTestPlugin
{
	DiscordEventHandlers handlers{};
	static time_t SystemTime2Timet(const SYSTEMTIME&);
	static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static CMyPlugin *GetThis(HWND hwnd);
	static INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pClientData);
	bool ShowDialog(HWND hwndOwner);
	TCHAR m_szIniFileName[MAX_PATH];
	bool conf_ChannelMode = false;
	bool conf_TimeMode = false;
	bool conf_LogoMode = false;
	bool conf_isFinalized = false;
	HWND m_hwnd;
	bool InitSettings();
	bool pluginState = false;
	void SaveConf();
	void InitDiscord();
	void UpdateState();
	const std::vector<WORD> knownIds = { 32736, 32737, 32738, 32739, 32740, 32741, 3274, 32742, 32327, 32375, 32391};

public:
	DWORD GetVersion() override
	{
		return TVTEST_PLUGIN_VERSION_(0, 0, 1);
	}

	bool GetPluginInfo(TVTest::PluginInfo* pInfo) override
	{
		pInfo->Type = TVTest::PLUGIN_TYPE_NORMAL;
		pInfo->Flags = TVTest::PLUGIN_FLAG_HASSETTINGS;
		pInfo->pszPluginName = L"TvTest RPC";
		pInfo->pszCopyright = L"(c) 2019-2020 noriokun4649";
		pInfo->pszDescription = L"DiscordRPCをTvTestで実現します。Discordで視聴中のチャンネルなどの情報が通知されます。";
		return true;
	}

	bool Initialize() override
	{
		InitDiscord();
		m_pApp->SetEventCallback(EventCallback, this);

		// ウィンドウクラスの登録
		WNDCLASS wc{};
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = g_hinstDLL;
		wc.hIcon = nullptr;
		wc.hCursor = nullptr;
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = TvTestRPC_WINDOW_CLASS;
		if (::RegisterClass(&wc) == 0) {
			return false;
		}

		// ウィンドウの作成
		m_hwnd = ::CreateWindowEx(
			0, TvTestRPC_WINDOW_CLASS, nullptr, WS_POPUP,
			0, 0, 0, 0, HWND_MESSAGE, nullptr, g_hinstDLL, this);
		if (m_hwnd == nullptr)
		{
			return false;
		}

		return true;
	}

	bool Finalize() override
	{
		Discord_Shutdown();
		SaveConf();

		// ウィンドウ・タイマーの破棄
		if (m_hwnd) {
			::KillTimer(m_hwnd, TvTestRPC_TIMER_ID);
			::DestroyWindow(m_hwnd);
		}
		
		return true;
	}
};

bool CMyPlugin::InitSettings() {
	::GetModuleFileName(g_hinstDLL, m_szIniFileName, MAX_PATH);
	::PathRenameExtension(m_szIniFileName, TEXT(".ini"));
	conf_ChannelMode = ::GetPrivateProfileInt(TEXT("Settings"), TEXT("Mode"), conf_ChannelMode, m_szIniFileName) != 0;
	conf_TimeMode = ::GetPrivateProfileInt(TEXT("Settings"), TEXT("TimeMode"), conf_ChannelMode, m_szIniFileName) != 0;
	conf_LogoMode = ::GetPrivateProfileInt(TEXT("Settings"), TEXT("LogoMode"), conf_ChannelMode, m_szIniFileName) != 0;
	m_pApp->AddLog(m_szIniFileName);
	conf_isFinalized = true;
	return true;
}

void CMyPlugin::InitDiscord()
{
	Discord_Initialize("577065126084214816", &handlers, 1, NULL);
}

void CMyPlugin::UpdateState()
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	TVTest::ProgramInfo Info;
	TVTest::ServiceInfo Service;
	TVTest::ChannelInfo ChannelInfo;
	Info.Size = sizeof(Info);
	Service.Size = sizeof(Service);
	ChannelInfo.Size = sizeof(ChannelInfo);
	wchar_t eventName[128];
	wchar_t eventText[128];
	wchar_t eventExtText[128];
	Info.pszEventName = eventName;
	Info.MaxEventName = sizeof(eventName) / sizeof(eventName[0]);
	Info.pszEventText = eventText;
	Info.MaxEventText = sizeof(eventText) / sizeof(eventText[0]);
	Info.pszEventExtText = eventExtText;
	Info.MaxEventExtText = sizeof(eventExtText) / sizeof(eventExtText[0]);
	std::string channelName;
	std::string eventNamed;

	if (m_pApp->GetCurrentProgramInfo(&Info)) {
		eventNamed = wide_to_utf8(Info.pszEventName);
		auto start = SystemTime2Timet(Info.StartTime);
		auto end = SystemTime2Timet(Info.StartTime) + Info.Duration;
		discordPresence.startTimestamp = start;
		if (!conf_TimeMode) discordPresence.endTimestamp = end;
	}

	if (m_pApp->GetServiceInfo(0, &Service) && m_pApp->GetCurrentChannelInfo(&ChannelInfo)) {
		if (!conf_ChannelMode) {
			channelName = wide_to_utf8(Service.szServiceName);
		}
		else {
			channelName = wide_to_utf8(ChannelInfo.szChannelName);
		}

		auto id = ChannelInfo.NetworkID;
		auto isId = find(knownIds.begin(), knownIds.end(), id) != knownIds.end();

		if (isId && conf_LogoMode) {
			auto netId = std::to_string(id);
			discordPresence.largeImageKey = netId.c_str();
			discordPresence.smallImageKey = "tvtest";
			discordPresence.smallImageText = "TvTest";
		}
		else {
			discordPresence.largeImageKey = "tvtest";
			discordPresence.smallImageKey = "";
			discordPresence.smallImageText = "";
		}
	}

	discordPresence.details = channelName.c_str();
	discordPresence.largeImageText = channelName.c_str();
	discordPresence.state = eventNamed.c_str();
	discordPresence.partyId = "";
	discordPresence.partySize = 0;
	discordPresence.partyMax = 0;
	discordPresence.matchSecret = "";
	discordPresence.joinSecret = "";
	discordPresence.spectateSecret = "";
	discordPresence.instance = 0;
	Discord_UpdatePresence(&discordPresence);
}

void CMyPlugin::SaveConf() {
	if (conf_isFinalized) {
		struct IntString {
			IntString(int Value) { ::wsprintf(m_szBuffer, TEXT("%d"), Value); }
			operator LPCTSTR() const { return m_szBuffer; }
			TCHAR m_szBuffer[16];
		};

		::WritePrivateProfileString(TEXT("Settings"), TEXT("Mode"), IntString(conf_ChannelMode), m_szIniFileName);
		::WritePrivateProfileString(TEXT("Settings"), TEXT("TimeMode"), IntString(conf_TimeMode), m_szIniFileName);
		::WritePrivateProfileString(TEXT("Settings"), TEXT("LogoMode"), IntString(conf_LogoMode), m_szIniFileName);
	}
}

time_t CMyPlugin::SystemTime2Timet(const SYSTEMTIME& st)
{
	struct tm gm = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	return mktime(&gm);
}

TVTest::CTVTestPlugin* CreatePluginClass()
{
	return new CMyPlugin;
}

bool CMyPlugin::ShowDialog(HWND hwndOwner) {

	TVTest::ShowDialogInfo Info;
	Info.Flags = 0;
	Info.hinst = g_hinstDLL;
	Info.pszTemplate = MAKEINTRESOURCE(IDD_DIALOG1);
	Info.pMessageFunc = SettingsDlgProc;
	Info.pClientData = this;
	Info.hwndOwner = hwndOwner;

	return m_pApp->ShowDialog(&Info) == IDOK;
}

INT_PTR CALLBACK CMyPlugin::SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pClientData) {
	CMyPlugin* pThis = static_cast<CMyPlugin*>(pClientData);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		::CheckDlgButton(hDlg, IDC_CHECK1, pThis->conf_ChannelMode ? BST_CHECKED : BST_UNCHECKED);
		::CheckDlgButton(hDlg, IDC_CHECK2, pThis->conf_TimeMode ? BST_CHECKED : BST_UNCHECKED);
		::CheckDlgButton(hDlg, IDC_CHECK3, pThis->conf_LogoMode ? BST_CHECKED : BST_UNCHECKED);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			pThis->conf_ChannelMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED;
			pThis->conf_TimeMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK2) == BST_CHECKED;
			pThis->conf_LogoMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK3) == BST_CHECKED;
			pThis->UpdateState();
		case IDCANCEL:
			::EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	default:
		break;
		return FALSE;
	}
	return FALSE;
}

LRESULT CALLBACK CMyPlugin::EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData)
{
	auto *pThis = static_cast<CMyPlugin*>(pClientData);
	
	switch (Event)
	{
	case TVTest::EVENT_PLUGINENABLE:
		if (lParam1 != 0) {
			pThis->UpdateState();
			pThis->InitSettings();
		}
		else
		{
			pThis->pluginState = false;
			Discord_ClearPresence();
		}
		
		return TRUE;

	case TVTest::EVENT_SERVICECHANGE:
	case TVTest::EVENT_CHANNELCHANGE:
	case TVTest::EVENT_SERVICEUPDATE:
		pThis->UpdateState();
		return TRUE;

	case TVTest::EVENT_PLUGINSETTINGS:
		return pThis->ShowDialog(reinterpret_cast<HWND>(lParam1));

	default:
		return 0;
	}
}

/*
 * ウィンドウプロシージャ
 * タイマー処理を行う
 */
LRESULT CALLBACK CMyPlugin::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CREATE:
		auto pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		auto* pThis = static_cast<CMyPlugin*>(pcs->lpCreateParams);

		::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
		::SetTimer(hwnd, TvTestRPC_TIMER_ID, 3000, nullptr);

		return TRUE;
	

	case WM_TIMER:
		if (wParam == TvTestRPC_TIMER_ID)
		{
			auto *pThis = GetThis(hwnd);
			pThis->UpdateState();
		}
		
		return 0;
		
	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

/*
 * ウィンドウハンドルから this を取得する
 */
CMyPlugin *CMyPlugin::GetThis(HWND hwnd)
{
	return reinterpret_cast<CMyPlugin*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
}
