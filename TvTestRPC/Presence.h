#pragma once

#include "stdafx.h"

#include "Logo.h"
#include "TvtPlay.h"
#include "Utils.h"

constexpr auto MaxDetailsLength = 128;
constexpr auto MaxEventNameLength = 128;
constexpr auto MaxEventTextLength = 128;
constexpr auto MaxImageTextLength = 128;

inline DiscordRichPresence CreatePresence(
    const std::optional<TVTest::ServiceInfo> Service,
    const std::optional<TVTest::ProgramInfo> Program,
    const DWORD Version,
    const bool ShowEndTime,
    const bool ShowChannelLogo,
    const bool ConvertToHalfWidth
)
{
    DiscordRichPresence Presence;
    memset(&Presence, 0, sizeof Presence);

    // 番組データがあるなら時間情報を付与する
    if (Program.has_value())
    {
        int64_t start, end;
        const auto duration = Program.value().Duration;

        // TvtPlay が有効なら再生位置を加味する
        if (const auto tvtPlayHwnd = FindTvtPlayFrame(); tvtPlayHwnd)
        {
            const auto now = time(nullptr);
            const auto pos = GetTvtPlayPositionSec(tvtPlayHwnd);

            start = now + pos;
            end = now + duration;
        }
        else
        {
            const auto startTime = Program.value().StartTime;

            start = SystemTime2Timet(startTime);
            end = start + duration;
        }

        Presence.startTimestamp = start;
        if (ShowEndTime) {
            Presence.endTimestamp = end;
        }
    }

    // サービスデータがあるならサービス名を付与する
    if (Service.has_value()) {
        wchar_t rawServiceName[32];
        wcscpy_s(rawServiceName, Service.value().szServiceName);
        const auto serviceName = WideToUTF8(rawServiceName, ConvertToHalfWidth);
        std::string details(serviceName.c_str(), MaxDetailsLength);

        Presence.details = details.c_str();
    }

    // 番組データがあるなら番組名を付与する
    if (Program.has_value())
    {
        const auto rawEventName = Program.value().pszEventName;
        const auto eventName = WideToUTF8(rawEventName, ConvertToHalfWidth);

        Presence.state = eventName.c_str();
    }

    // チャンネルロゴを付与する
    if (ShowChannelLogo)
    {
        if (Service.has_value()) {
            const auto serviceId = Service.value().ServiceID;
            const auto logoKey = GetServiceLogoKey(serviceId);

            Presence.largeImageKey = logoKey.c_str();
        }
        else
        {
            Presence.largeImageKey = LOGO_DEFAULT;
        }

        // 番組データがあるときは番組説明を付与する
        if (Program.has_value())
        {
            const auto rawEventText = Program.value().pszEventText;
            const auto eventText = WideToUTF8(rawEventText, ConvertToHalfWidth);

            Presence.largeImageText = eventText.c_str();
        }
    }

    // バージョン情報を付与する
    char tvtestVersion[MaxImageTextLength];
    const auto major = TVTest::GetMajorVersion(Version);
    const auto minor = TVTest::GetMinorVersion(Version);
    const auto build = TVTest::GetBuildVersion(Version);
    sprintf_s(tvtestVersion, "TVTest %lu.%lu.%lu", major, minor, build);
    std::string version(tvtestVersion, MaxImageTextLength);
    Presence.smallImageKey = LOGO_DEFAULT;
    Presence.smallImageText = version.c_str();

    return Presence;
}

inline bool CheckEquality(const DiscordRichPresence one, const DiscordRichPresence other)
{
    // 番組の開始時刻が違う
    if (one.startTimestamp != other.startTimestamp)
    {
        return false;
    }

    // サービス名が違う
    if (one.details != other.details)
    {
        return false;
    }

    // 番組名が違う
    if (one.state != other.state)
    {
        return false;
    }

    // ロゴが違う
    if (one.largeImageKey != other.largeImageKey)
    {
        return false;
    }

    return true;
}
