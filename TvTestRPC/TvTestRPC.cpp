#include "stdafx.h"

#include "Presence.h"

constexpr auto TvTestRPCWindowClass = L"TvTestRPC Window";
constexpr auto TvTestRPCTimerId = 1;
constexpr auto TvTestRPCTimerIntervalMs = 3000;

class CTvTestRPCPlugin final : public TVTest::CTVTestPlugin
{
    DiscordEventHandlers m_handlers{};
    DiscordRichPresence m_lastPresence{};
    wchar_t m_iniFileName[MAX_PATH]{};
    HWND m_hwnd{};
    std::mutex m_mutex;
    bool m_isReady = false;

    bool m_showEndTime = true;
    bool m_showChannelLogo = true;
    bool m_convertToHalfWidth = true;
    std::map<WORD, std::string> m_logos;

    void LoadConfig();
    void SaveConfig() const;

    void StartDiscordRPC();
    void UpdatePresence();

    static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    /*
     * プラグインの API バージョンを返す
     */
    DWORD GetVersion() override
    {
        // 最低要件
        // どのバージョンの TVTest でも動作する
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
        // 設定を保存
        SaveConfig();

        // タイマー・ウィンドウの破棄
        ::KillTimer(m_hwnd, TvTestRPCTimerId);
        ::DestroyWindow(m_hwnd);

        return true;
    }
};

/*
 * プラグインのインスタンスを作成する
 */
TVTest::CTVTestPlugin* CreatePluginClass()
{
    return new CTvTestRPCPlugin;
}

/*
 * 設定を読み込む
 */
void CTvTestRPCPlugin::LoadConfig()
{
    ::GetModuleFileName(g_hinstDLL, m_iniFileName, MAX_PATH);
    ::PathRenameExtension(m_iniFileName, L".ini");

    const auto settings = GetPrivateProfileSectionBuffer(L"Settings", m_iniFileName);
    m_showEndTime = GetBufferedProfileInt(settings.data(), L"ShowEndTime", m_showEndTime) > 0;
    m_showChannelLogo = GetBufferedProfileInt(settings.data(), L"ShowChannelLogo", m_showChannelLogo) > 0;
    m_convertToHalfWidth = GetBufferedProfileInt(settings.data(), L"ConvertToHalfWidth", m_convertToHalfWidth) > 0;

    const auto logos = GetPrivateProfileSectionBuffer(L"Logos", m_iniFileName);
    for (auto p = logos.data(); *p; p += wcslen(p) + 1) {
        WORD serviceId;

        if (char logoKey[MaxImageKeyLength]; swscanf_s(p, L"%hd=%S", &serviceId, logoKey, MaxImageKeyLength) == 2)
		{
            m_logos[serviceId] = logoKey;
		}
	}
}

/*
 * 設定を保存する
 */
void CTvTestRPCPlugin::SaveConfig() const
{
    if (!m_isReady)
    {
        return;
    }

    WritePrivateProfileInt(L"Settings", L"ShowEndTime", m_showEndTime, m_iniFileName);
    WritePrivateProfileInt(L"Settings", L"ShowChannelLogo", m_showChannelLogo, m_iniFileName);
    WritePrivateProfileInt(L"Settings", L"ConvertToHalfWidth", m_convertToHalfWidth, m_iniFileName);
}

/*
 * Discord RPC クライアントを初期化する
 */
void CTvTestRPCPlugin::StartDiscordRPC()
{
    Discord_Initialize("844540020286685184", &m_handlers, 1, nullptr);
}

/*
 * Discord Rich Presence を更新する
 */
void CTvTestRPCPlugin::UpdatePresence()
{
    if (!m_isReady)
    {
        return;
    }

    // ロック
    {
        std::lock_guard lock(m_mutex);

        // Service: サブチャンネルを許容して取得する
        std::optional<TVTest::ServiceInfo> service = std::nullopt;

        // サブチャンネルを考慮する
        for (auto i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
        {
            TVTest::ServiceInfo Service{};

            if (m_pApp->GetServiceInfo(i, &Service))
            {
                service = std::optional(Service);
                break;
            }
        }

        // Program
        TVTest::ProgramInfo Program{};
        wchar_t pszEventName[EventNameLength];
        Program.pszEventName = pszEventName;
        Program.MaxEventName = _countof(pszEventName);
        wchar_t pszEventText[EventTextLength];
        Program.pszEventText = pszEventText;
        Program.MaxEventText = _countof(pszEventText);
        wchar_t pszEventExtText[EventTextExtLength];
        Program.pszEventExtText = pszEventExtText;
        Program.MaxEventExtText = _countof(pszEventExtText);
        const auto program = m_pApp->GetCurrentProgramInfo(&Program) ? std::optional(Program) : std::nullopt;

        // Version
        auto version = m_pApp->GetVersion();

        auto presence = CreatePresence(service, program, version, m_showEndTime, m_showChannelLogo, m_convertToHalfWidth, m_logos);

        // 同じ Presence であれば無視
        auto const result = CheckEquality(presence, m_lastPresence);
        if (result == PresenceEquality::Same)
        {
            return;
        }

        Discord_UpdatePresence(&presence);
        m_lastPresence = presence;

        // ログ
        // タイムスタンプ違いのときは出力しない
        if (result == PresenceEquality::DifferentTimestamp)
        {
            return;
        }

        const auto serviceName = service.has_value() && !IsBlank(service.value().szServiceName, ServiceNameLength) ? service.value().szServiceName : L"[不明]";
        const auto eventName = program.has_value() && !IsBlank(program.value().pszEventName, EventNameLength) ? program.value().pszEventName : L"[不明]";
        wchar_t buf[256];
        wsprintf(buf, L"Rich Presence を更新しました。(サービス名: %s, 番組名: %s)", serviceName, eventName);
        m_pApp->AddLog(buf);
    }
}

/*
 * TVTest のイベントコールバック
 */
LRESULT CALLBACK CTvTestRPCPlugin::EventCallback(const UINT Event, const LPARAM lParam1, LPARAM, void* pClientData)
{
    auto* pThis = static_cast<CTvTestRPCPlugin*>(pClientData);

    switch (Event)
    {
    case TVTest::EVENT_PLUGINENABLE:
        if (lParam1 == 1)
        {
            pThis->LoadConfig();

            pThis->StartDiscordRPC();
            pThis->m_isReady = true;

            pThis->UpdatePresence();
        }
        else
        {
            pThis->m_isReady = false;
            pThis->m_lastPresence = {};

            Discord_ClearPresence();
            // Discord RPC クライアントの破棄
            Discord_Shutdown();
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
LRESULT CALLBACK CTvTestRPCPlugin::WndProc(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        {
            auto* const pcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
            auto* pThis = static_cast<CTvTestRPCPlugin*>(pcs->lpCreateParams);

            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            ::SetTimer(hWnd, TvTestRPCTimerId, TvTestRPCTimerIntervalMs, nullptr);
        }

        return true;

    case WM_TIMER:
        if (wParam == TvTestRPCTimerId)
        {
            auto* pThis = reinterpret_cast<CTvTestRPCPlugin*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

            pThis->UpdatePresence();
        }

        return false;

    default:
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
