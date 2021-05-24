#include "stdafx.h"
#include <ctime>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "Presence.h"

constexpr auto TvTestRPCWindowClass = L"TvTestRPC Window";
constexpr auto TvTestRPCTimerId = 1;

class CMyPlugin final : public TVTest::CTVTestPlugin
{
    DiscordEventHandlers m_handlers{};
    TCHAR m_iniFileName[MAX_PATH]{};
    HWND m_hwnd{};
    WORD m_eventId{};

    bool m_showEndTime = false;
    bool m_showChannelLogo = false;
    bool m_convertToHalfWidth = false;
    bool m_isReady = false;

    bool LoadConfig();
    void UpdatePresence();

    static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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
        pInfo->pszPluginName = L"Discord Rich Presence";
        pInfo->pszCopyright = L"© 2021 Nep, 2019-2020 noriokun4649";
        pInfo->pszDescription = L"Discord Rich Presence を表示します。";

        return true;
    }

    /*
     * プラグインの初期化を行う
     */
    bool Initialize() override
    {
        // Discord RPC クライアントの初期化
        Discord_Initialize("844540020286685184", &m_handlers, 1, nullptr);

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
        wc.lpszClassName = TvTestRPCWindowClass;
        if (::RegisterClass(&wc) == 0)
        {
            return false;
        }

        // ウィンドウの作成
        m_hwnd = ::CreateWindowEx(
            0, TvTestRPCWindowClass, nullptr, WS_POPUP,
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
        // タイマー・ウィンドウの破棄
        ::KillTimer(m_hwnd, TvTestRPCTimerId);
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

    m_showEndTime = ::GetPrivateProfileInt(L"Settings", L"ShowEndTime", m_showEndTime, m_iniFileName) > 0;
    m_showChannelLogo = ::GetPrivateProfileInt(L"Settings", L"ShowChannelLogo", m_showChannelLogo, m_iniFileName) > 0;
    m_convertToHalfWidth = ::GetPrivateProfileInt(L"Settings", L"ConvertToHalfWidth", m_convertToHalfWidth, m_iniFileName) > 0;
    m_isReady = true;

    return true;
}

/*
 * Discord Rich Presence を更新する
 */
void CMyPlugin::UpdatePresence()
{
    if (!m_isReady)
    {
        return;
    }

    // Service: サブチャンネルを許容して取得する
    std::optional<TVTest::ServiceInfo> service = std::nullopt;
    for (int i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
    {
        TVTest::ServiceInfo Service{};

        if (m_pApp->GetServiceInfo(i, &Service) && wcslen(Service.szServiceName) > 0)
        {
            service = std::optional(Service);
            break;
        }
    }

    if (!service.has_value())
    {
        return;
    }

    // Program
    TVTest::ProgramInfo Program{};
    wchar_t pszEventName[128];
    Program.pszEventName = pszEventName;
    Program.MaxEventName = _countof(pszEventName);
    wchar_t pszEventText[128];
    Program.pszEventText = pszEventText;
    Program.MaxEventText = _countof(pszEventText);
    wchar_t pszEventExtText[128];
    Program.pszEventExtText = pszEventExtText;
    Program.MaxEventExtText = _countof(pszEventExtText);
    auto program = m_pApp->GetCurrentProgramInfo(&Program) ? std::optional(Program) : std::nullopt;

    // 前回と同じ Event ID であれば更新する必要はない
    if (program.has_value() && m_eventId == program.value().EventID)
    {
        return;
    }

    // Version
    auto version = m_pApp->GetVersion();

    auto presence = CreatePresence(service.value(), program, version, m_showEndTime, m_showChannelLogo, m_convertToHalfWidth);
    Discord_UpdatePresence(&presence);

    // Event ID を更新
    if (program.has_value())
    {
        m_eventId = program.value().EventID;
    }
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

    case TVTest::EVENT_CHANNELCHANGE:
    case TVTest::EVENT_SERVICECHANGE:
    case TVTest::EVENT_SERVICEUPDATE:
        pThis->UpdatePresence();

        return true;

    default:
        return false;
    }
}

/*
 * ウィンドウプロシージャ
 * タイマー処理を行う
 */
LRESULT CALLBACK CMyPlugin::WndProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            auto* const pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            auto* pThis = static_cast<CMyPlugin*>(pcs->lpCreateParams);

            ::SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            ::SetTimer(hwnd, TvTestRPCTimerId, 3000, nullptr);
        }

        return true;

    case WM_TIMER:
        if (wParam == TvTestRPCTimerId)
        {
            auto* pThis = reinterpret_cast<CMyPlugin*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));

            pThis->UpdatePresence();
        }

        return false;

    default:
        return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
