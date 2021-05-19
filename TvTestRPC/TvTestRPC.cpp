#include "stdafx.h"
#include <ctime>
#include <shlwapi.h>
#include "resource.h"
#include <vector>
#include "Utils.h"

#pragma comment(lib, "shlwapi.lib")

#define TvTestRPC_WINDOW_CLASS L"TvTestRPC Window"
#define TvTestRPC_TIMER_ID 1

class CMyPlugin final : public TVTest::CTVTestPlugin
{
	DiscordEventHandlers handlers{};
	const std::vector<WORD> knownIds = {32736, 32737, 32738, 32739, 32740, 32741, 3274, 32742, 32327, 32375, 32391};

	TCHAR m_iniFileName[MAX_PATH]{};
	HWND m_hwnd{};
	WORD m_eventId{};

	bool m_channelMode = false;
	bool m_timeMode = false;
	bool m_logoMode = false;
	bool m_isReady = false;

	bool LoadConfig();
	void SaveConfig() const;
	bool ShowDialog(HWND hwndOwner);

	void UpdatePresence();

	static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK SettingsDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pClientData);
	static CMyPlugin* GetThis(HWND hwnd);

public:
	/*
	 * プラグインのバージョンを返す
	 */
	DWORD GetVersion() override
	{
		return TVTEST_PLUGIN_VERSION_(0, 0, 1);
	}

	/*
	 * プラグインの情報を返す
	 */
	bool GetPluginInfo(TVTest::PluginInfo* pInfo) override
	{
		pInfo->Type = TVTest::PLUGIN_TYPE_NORMAL;
		pInfo->Flags = TVTest::PLUGIN_FLAG_HASSETTINGS;
		pInfo->pszPluginName = L"TvTest RPC";
		pInfo->pszCopyright = L"(c) 2019-2020 noriokun4649";
		pInfo->pszDescription = L"DiscordRPCをTvTestで実現します。Discordで視聴中のチャンネルなどの情報が通知されます。";

		return true;
	}

	/*
	 * プラグインの初期化を行う
	 */
	bool Initialize() override
	{
		// Discord RPC クライアントの初期化
		Discord_Initialize("577065126084214816", &handlers, 1, nullptr);

		// TVTest イベントコールバックの設定
		m_pApp->SetEventCallback(EventCallback, this);

		// ウィンドウクラスの登録
		WNDCLASS wc;
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
		if (::RegisterClass(&wc) == 0)
		{
			return false;
		}

		// ウィンドウの作成
		m_hwnd = ::CreateWindowEx(
			0, TvTestRPC_WINDOW_CLASS, nullptr, WS_POPUP,
			0, 0, 0, 0, HWND_MESSAGE, nullptr, g_hinstDLL, this
		);
		if (m_hwnd == nullptr)
		{
			return false;
		}

		return true;
	}

	/*
	 * プラグインの終了処理を行う
	 */
	bool Finalize() override
	{
		// 設定を保存
		SaveConfig();

		// タイマー・ウィンドウの破棄
		::KillTimer(m_hwnd, TvTestRPC_TIMER_ID);
		::DestroyWindow(m_hwnd);

		// Discord RPC クライアントの破棄
		Discord_Shutdown();

		return true;
	}
};

/*
 * プラグインのインスタンスを作成する
 */
TVTest::CTVTestPlugin* CreatePluginClass()
{
	return new CMyPlugin;
}

/*
 * 設定を読み込む
 */
bool CMyPlugin::LoadConfig()
{
	::GetModuleFileName(g_hinstDLL, m_iniFileName, MAX_PATH);
	::PathRenameExtension(m_iniFileName, L".ini");

	m_channelMode = ::GetPrivateProfileInt(L"Settings", L"Mode", m_channelMode, m_iniFileName) != 0;
	m_timeMode = ::GetPrivateProfileInt(L"Settings", L"TimeMode", m_channelMode, m_iniFileName) != 0;
	m_logoMode = ::GetPrivateProfileInt(L"Settings", L"LogoMode", m_channelMode, m_iniFileName) != 0;
	m_isReady = true;

	return true;
}

/*
 * 設定を保存する
 */
void CMyPlugin::SaveConfig() const
{
	if (m_isReady)
	{
		struct IntString
		{
			TCHAR m_buffer[16]{};

			explicit IntString(const int value)
			{
				::wsprintf(m_buffer, L"%d", value);
			}

			operator LPCTSTR() const
			{
				return m_buffer;
			}
		};

		::WritePrivateProfileString(L"Settings", L"Mode", IntString(m_channelMode), m_iniFileName);
		::WritePrivateProfileString(L"Settings", L"TimeMode", IntString(m_timeMode), m_iniFileName);
		::WritePrivateProfileString(L"Settings", L"LogoMode", IntString(m_logoMode), m_iniFileName);
	}
}

/*
 * 設定ダイアログを表示する
 */
bool CMyPlugin::ShowDialog(const HWND hwndOwner)
{
	TVTest::ShowDialogInfo Info{};
	Info.Flags = 0;
	Info.hinst = g_hinstDLL;
	Info.pszTemplate = MAKEINTRESOURCE(IDD_DIALOG1);
	Info.pMessageFunc = SettingsDialogProc;
	Info.pClientData = this;
	Info.hwndOwner = hwndOwner;

	return m_pApp->ShowDialog(&Info) == IDOK;
}

/*
 * Discord Rich Presence を更新する
 */
void CMyPlugin::UpdatePresence()
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof discordPresence);

	TVTest::ProgramInfo Program{};
	Program.Size = sizeof(Program);
	wchar_t eventName[128];
	wchar_t eventText[128];
	wchar_t eventExtText[128];
	Program.pszEventName = eventName;
	Program.MaxEventName = sizeof eventName / sizeof eventName[0];
	Program.pszEventText = eventText;
	Program.MaxEventText = sizeof eventText / sizeof eventText[0];
	Program.pszEventExtText = eventExtText;
	Program.MaxEventExtText = sizeof eventExtText / sizeof eventExtText[0];
	std::string channelName;
	std::string eventNamed;

	TVTest::ServiceInfo Service{};
	Service.Size = sizeof Service;

	TVTest::ChannelInfo ChannelInfo{};
	ChannelInfo.Size = sizeof ChannelInfo;

	if (m_pApp->GetCurrentProgramInfo(&Program))
	{
		// 前回と同じ Event ID であれば更新する必要はない
		if (m_eventId == Program.EventID)
		{
			return;
		}
		m_eventId = Program.EventID;

		eventNamed = wide_to_utf8(Program.pszEventName);

		auto start = SystemTime2Timet(Program.StartTime);
		auto end = SystemTime2Timet(Program.StartTime) + Program.Duration;
		discordPresence.startTimestamp = start;
		if (!m_timeMode) discordPresence.endTimestamp = end;
	}

	if (m_pApp->GetServiceInfo(0, &Service) && m_pApp->GetCurrentChannelInfo(&ChannelInfo))
	{
		if (!m_channelMode)
		{
			channelName = wide_to_utf8(Service.szServiceName);
		}
		else
		{
			channelName = wide_to_utf8(ChannelInfo.szChannelName);
		}

		auto hasLogo = find(knownIds.begin(), knownIds.end(), ChannelInfo.NetworkID) != knownIds.end();
		if (hasLogo && m_logoMode)
		{
			auto networkId = std::to_string(ChannelInfo.NetworkID);
			discordPresence.largeImageKey = networkId.c_str();
			discordPresence.smallImageKey = "tvtest";
		}
		else
		{
			discordPresence.largeImageKey = "tvtest";
			discordPresence.smallImageKey = "";
			discordPresence.smallImageText = "";
		}
	}

	discordPresence.details = channelName.c_str();
	discordPresence.largeImageText = channelName.c_str();
	discordPresence.state = eventNamed.c_str();
	Discord_UpdatePresence(&discordPresence);
}

LRESULT CALLBACK CMyPlugin::EventCallback(const UINT Event, const LPARAM lParam1, LPARAM, void* pClientData)
{
	auto* pThis = static_cast<CMyPlugin*>(pClientData);

	switch (Event)
	{
	case TVTest::EVENT_PLUGINENABLE:
		if (lParam1 != 0)
		{
			pThis->LoadConfig();
			pThis->UpdatePresence();
		}
		else
		{
			Discord_ClearPresence();
		}

		return true;

	case TVTest::EVENT_SERVICECHANGE:
	case TVTest::EVENT_CHANNELCHANGE:
	case TVTest::EVENT_SERVICEUPDATE:
		pThis->UpdatePresence();

		return true;

	case TVTest::EVENT_PLUGINSETTINGS:
		return pThis->ShowDialog(reinterpret_cast<HWND>(lParam1));

	default:
		return false;
	}
}

/*
 * ウィンドウプロシージャ
 * タイマー処理を行う
 */
LRESULT CALLBACK CMyPlugin::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		{
			auto* const pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			auto* pThis = static_cast<CMyPlugin*>(pcs->lpCreateParams);

			::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			::SetTimer(hwnd, TvTestRPC_TIMER_ID, 3000, nullptr);
		}

		return TRUE;

	case WM_TIMER:
		if (wParam == TvTestRPC_TIMER_ID)
		{
			auto* pThis = GetThis(hwnd);
			pThis->UpdatePresence();
		}

		return 0;

	default:
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

/*
 * 設定ダイアログプロシージャ
 */
INT_PTR CALLBACK CMyPlugin::SettingsDialogProc(const HWND hDlg, const UINT uMsg, const WPARAM wParam, LPARAM,
                                               void* pClientData)
{
	auto* pThis = static_cast<CMyPlugin*>(pClientData);

	switch (uMsg)
	{
	case WM_INITDIALOG:
		::CheckDlgButton(hDlg, IDC_CHECK1, pThis->m_channelMode ? BST_CHECKED : BST_UNCHECKED);
		::CheckDlgButton(hDlg, IDC_CHECK2, pThis->m_timeMode ? BST_CHECKED : BST_UNCHECKED);
		::CheckDlgButton(hDlg, IDC_CHECK3, pThis->m_logoMode ? BST_CHECKED : BST_UNCHECKED);

		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			pThis->m_channelMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED;
			pThis->m_timeMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK2) == BST_CHECKED;
			pThis->m_logoMode = ::IsDlgButtonChecked(hDlg, IDC_CHECK3) == BST_CHECKED;
			pThis->UpdatePresence();

		case IDCANCEL:
			::EndDialog(hDlg, wParam);

			return TRUE;

		default:
			return FALSE;
		}

	default:
		return FALSE;
	}
}

/*
 * ウィンドウハンドルから this を取得する
 */
CMyPlugin* CMyPlugin::GetThis(const HWND hwnd)
{
	return reinterpret_cast<CMyPlugin*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
}
