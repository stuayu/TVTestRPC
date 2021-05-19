﻿#include "stdafx.h"
#include <ctime>
#include <shlwapi.h>
#include "resource.h"
#include <vector>
#include "Utils.cpp"

#pragma comment(lib, "shlwapi.lib")

#define TvTestRPC_WINDOW_CLASS L"TvTestRPC Window"
#define TvTestRPC_TIMER_ID 1

class CMyPlugin final : public TVTest::CTVTestPlugin
{
    const std::vector<WORD> knownIds = {32736, 32737, 32738, 32739, 32740, 32741, 3274, 32742, 32327, 32375, 32391};

    DiscordEventHandlers m_handlers{};
    TCHAR m_iniFileName[MAX_PATH]{};
    HWND m_hwnd{};
    WORD m_eventId{};

    bool m_showChannelName = false;
    bool m_showEndTime = false;
    bool m_showChannelLogo = false;
	bool m_convertToHalfWidth = false;
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
        Discord_Initialize("577065126084214816", &m_handlers, 1, nullptr);

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

    m_showChannelName = ::GetPrivateProfileInt(L"Settings", L"ShowChannelName", m_showChannelName, m_iniFileName) != 0;
    m_showEndTime = ::GetPrivateProfileInt(L"Settings", L"ShowEndTime", m_showEndTime, m_iniFileName) != 0;
    m_showChannelLogo = ::GetPrivateProfileInt(L"Settings", L"ShowChannelLogo", m_showChannelLogo, m_iniFileName) != 0;
	m_convertToHalfWidth = ::GetPrivateProfileInt(L"Settings", L"ConvertToHalfWidth", m_convertToHalfWidth, m_iniFileName) != 0;
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
	        wchar_t m_buffer[16];

            explicit IntString(const int value)
            {
                ::wsprintf(m_buffer, L"%d", value);
            }

            operator LPCTSTR() const
            {
                return m_buffer;
            }
        };

        ::WritePrivateProfileString(L"Settings", L"ShowChannelName", IntString(m_showChannelName), m_iniFileName);
        ::WritePrivateProfileString(L"Settings", L"ShowEndTime", IntString(m_showEndTime), m_iniFileName);
        ::WritePrivateProfileString(L"Settings", L"ShowChannelLogo", IntString(m_showChannelLogo), m_iniFileName);
    	::WritePrivateProfileString(L"Settings", L"ConvertToHalfWidth", IntString(m_convertToHalfWidth), m_iniFileName);
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
    Program.Size = sizeof Program;
    wchar_t pszEventName[128];
    Program.pszEventName = pszEventName;
    Program.MaxEventName = sizeof pszEventName / sizeof pszEventName[0];
    wchar_t pszEventText[128];
    Program.pszEventText = pszEventText;
    Program.MaxEventText = sizeof pszEventText / sizeof pszEventText[0];
    wchar_t pszEventExtText[128];
    Program.pszEventExtText = pszEventExtText;
    Program.MaxEventExtText = sizeof pszEventExtText / sizeof pszEventExtText[0];

    TVTest::ServiceInfo Service{};
    Service.Size = sizeof Service;

    TVTest::ChannelInfo Channel{};
    Channel.Size = sizeof Channel;

    std::string eventName;
    std::string channelName;

	char tvtestVersion[128];
	auto version = m_pApp->GetVersion();
    auto major = TVTest::GetMajorVersion(version);
    auto minor = TVTest::GetMinorVersion(version);
    auto build = TVTest::GetBuildVersion(version);
    sprintf_s(tvtestVersion, "TVTest %lu.%lu.%lu", major, minor, build);

    if (m_pApp->GetCurrentProgramInfo(&Program))
    {
        // 前回と同じ Event ID であれば更新する必要はない
        if (m_eventId == Program.EventID)
        {
            return;
        }
        m_eventId = Program.EventID;

        eventName = WideToUTF8(Program.pszEventName, m_convertToHalfWidth);

        auto start = SystemTime2Timet(Program.StartTime);
        auto end = start + Program.Duration;
        
        discordPresence.startTimestamp = start;
        if (m_showEndTime) {
            discordPresence.endTimestamp = end;
        }
    }

    if (m_pApp->GetServiceInfo(0, &Service) && m_pApp->GetCurrentChannelInfo(&Channel))
    {
        if (m_showChannelName)
        {
            channelName = WideToUTF8(Channel.szChannelName, m_convertToHalfWidth);
        }
        else
        {
            channelName = WideToUTF8(Service.szServiceName, m_convertToHalfWidth);
        }

        auto hasLogo = find(knownIds.begin(), knownIds.end(), Channel.NetworkID) != knownIds.end();
        if (hasLogo && m_showChannelLogo)
        {
            auto networkId = std::to_string(Channel.NetworkID);
            
            discordPresence.smallImageKey = "tvtest";
            discordPresence.smallImageText = tvtestVersion;
        	discordPresence.largeImageKey = networkId.c_str();
        	discordPresence.largeImageText = channelName.c_str();
        }
        else
        {
            discordPresence.largeImageKey = "tvtest";
        	discordPresence.largeImageText = tvtestVersion;
        }
    }
	
    discordPresence.details = channelName.c_str();
    discordPresence.state = eventName.c_str();
    Discord_UpdatePresence(&discordPresence);
}

/*
 * TVTest のイベントコールバック
 */
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

        return true;

    case WM_TIMER:
        if (wParam == TvTestRPC_TIMER_ID)
        {
            auto* pThis = GetThis(hwnd);
            pThis->UpdatePresence();
        }

        return false;

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
        ::CheckDlgButton(hDlg, IDC_CHECK1, pThis->m_showChannelName ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(hDlg, IDC_CHECK2, pThis->m_showEndTime ? BST_CHECKED : BST_UNCHECKED);
        ::CheckDlgButton(hDlg, IDC_CHECK3, pThis->m_showChannelLogo ? BST_CHECKED : BST_UNCHECKED);

        return true;

    case WM_COMMAND:
        switch (wParam)
        {
        case IDOK:
            pThis->m_showChannelName = ::IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED;
            pThis->m_showEndTime = ::IsDlgButtonChecked(hDlg, IDC_CHECK2) == BST_CHECKED;
            pThis->m_showChannelLogo = ::IsDlgButtonChecked(hDlg, IDC_CHECK3) == BST_CHECKED;

        	pThis->UpdatePresence();

        case IDCANCEL:
            ::EndDialog(hDlg, wParam);

            return true;

        default:
            return false;
        }

    default:
        return false;
    }
}

/*
 * ウィンドウハンドルから this を取得する
 */
CMyPlugin* CMyPlugin::GetThis(const HWND hwnd)
{
    return reinterpret_cast<CMyPlugin*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
}
