#pragma once

#include "stdafx.h"

#include "Logo.h"
#include "TvtPlay.h"
#include "Utils.h"
#include "Version.h"

constexpr auto ServiceNameLength = 32;
constexpr auto EventNameLength = 128;
constexpr auto EventTextLength = 128;
constexpr auto EventTextExtLength = 128;

constexpr auto MaxDetailsLength = 256;
constexpr auto MaxStateLength = 256;
constexpr auto MaxImageKeyLength = 64;
constexpr auto MaxImageTextLength = 256;

inline DiscordRichPresence CreatePresence(
    const std::optional<const TVTest::ServiceInfo> Service,
    const std::optional<const TVTest::ProgramInfo> Program,
    const DWORD Version,
    const bool ShowEndTime,
    const bool ShowChannelLogo,
    const bool ConvertToHalfWidth
)
{
    // ロケールの設定
    setlocale(LC_ALL, ".utf8");

    // 文字列バッファ等
    time_t startTimestamp = 0;
    time_t endTimestamp = 0;
    char details[MaxDetailsLength] = {};
    char state[MaxStateLength] = {};
    char largeImageKey[MaxImageKeyLength] = {};
    char largeImageText[MaxImageTextLength] = {};
    char smallImageKey[MaxImageKeyLength] = {};
    char smallImageText[MaxImageTextLength] = {};

    // 番組データがあるなら時間情報を付与する
    if (Program.has_value())
    {
        const auto duration = static_cast<time_t>(Program.value().Duration);

        // TvtPlay が有効なら再生位置を加味する
        if (const auto tvtPlayHwnd = FindTvtPlayFrame(); tvtPlayHwnd)
        {
            const auto now = time(nullptr);
            const auto pos = GetTvtPlayPositionSec(tvtPlayHwnd);

            startTimestamp = now + pos;
            if (ShowEndTime)
            {
                endTimestamp = now + (duration - pos);
            }
        }
        else
        {
            const auto rawStartTime = Program.value().StartTime;

            startTimestamp = SystemTime2Timet(rawStartTime);
            if (ShowEndTime)
            {
                endTimestamp = startTimestamp + duration;
            }
        }
    }

    // サービスデータがあるならサービス名を付与する
    if (Service.has_value())
    {
        // const wchar_t* → wchar_t*
        // const auto rawServiceName = const_cast<LPWSTR>(Service.value().szServiceName);
        wchar_t serviceName[ServiceNameLength] = {};
        const auto rawServiceName = Service.value().szServiceName;
        wcsncpy_s(serviceName, wcslen(rawServiceName) > 0 ? rawServiceName : L"取得中…", ServiceNameLength);

        // 半角変換
        if (ConvertToHalfWidth)
        {
            Full2Half(serviceName);
        }

        wcstombs_s(nullptr, details, serviceName, ServiceNameLength);
    }

    // 番組データがあるなら番組名を付与する
    if (Program.has_value())
    {
        const auto rawEventName = Program.value().pszEventName;

        // 半角変換
        if (ConvertToHalfWidth)
        {
            Full2Half(rawEventName);
        }

        wcstombs_s(nullptr, state, rawEventName, EventNameLength - 1);
    }

    // チャンネルロゴ or TVTest ロゴを付与する
    if (ShowChannelLogo)
    {
        WORD serviceId = 0;
        if (Service.has_value())
        {
            serviceId = Service.value().ServiceID;
        }

        const auto logoKey = GetServiceLogoKey(serviceId);
        strcpy_s(largeImageKey, logoKey);

        // 番組データがあるなら番組説明を付与する
        if (Program.has_value())
        {
            if (const auto rawEventText = Program.value().pszEventText; wcslen(rawEventText) > 0)
            {
                // 半角変換
                if (ConvertToHalfWidth)
                {
                    Full2Half(rawEventText);
                }

                wcstombs_s(nullptr, largeImageText, rawEventText, EventTextLength - 1);
            }
            else if (const auto rawEventExtText = Program.value().pszEventExtText; wcslen(rawEventExtText) > 0)
            {
                // 半角変換
                if (ConvertToHalfWidth)
                {
                    Full2Half(rawEventExtText);
                }

                wcstombs_s(nullptr, largeImageText, rawEventExtText, EventTextExtLength - 1);
            }
        }
    }

    // バージョン情報を付与する
    {
        const auto major = TVTest::GetMajorVersion(Version);
        const auto minor = TVTest::GetMinorVersion(Version);
        const auto build = TVTest::GetBuildVersion(Version);

        strcpy_s(smallImageKey, LOGO_DEFAULT);
        sprintf_s(smallImageText, "TVTest v%lu.%lu.%lu / TvTestRPC v%s", major, minor, build, TvTestRPCVersion);
    }
    
    DiscordRichPresence Presence;
    memset(&Presence, 0, sizeof Presence);
    Presence.details = details;
    Presence.state = state;
    Presence.startTimestamp = startTimestamp;
    Presence.endTimestamp = endTimestamp;
    Presence.largeImageKey = largeImageKey;
    Presence.largeImageText = largeImageText;
    Presence.smallImageKey = smallImageKey;
    Presence.smallImageText = smallImageText;

    Presence.partyId = "";
	Presence.partySize = 0;
	Presence.partyMax = 0;
	Presence.matchSecret = "";
	Presence.joinSecret = "";
	Presence.spectateSecret = "";
	Presence.instance = 0;

    return Presence;
}

enum class PresenceEquality
{
    Same, DifferentTimestamp, Different
};

inline PresenceEquality CheckEquality(const DiscordRichPresence one, const DiscordRichPresence other)
{
    // 番組の開始時刻が違う
    if (one.startTimestamp != other.startTimestamp)
    {
        return PresenceEquality::DifferentTimestamp;
    }

    // サービス名が違う
    if (one.details != other.details)
    {
        return PresenceEquality::Different;
    }

    // 番組名が違う
    if (one.state != other.state)
    {
        return PresenceEquality::Different;
    }

    // ロゴが違う
    if (one.largeImageKey != other.largeImageKey)
    {
        return PresenceEquality::Different;
    }

    return PresenceEquality::Same;
}
