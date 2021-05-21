#pragma once

#include "stdafx.h"
#include "Logo.h"
#include "TvtPlay.h"
#include "Utils.h"

inline std::string MakeVersionString(const DWORD Version)
{
    char tvtestVersion[128];
    const auto major = TVTest::GetMajorVersion(Version);
    const auto minor = TVTest::GetMinorVersion(Version);
    const auto build = TVTest::GetBuildVersion(Version);
    sprintf_s(tvtestVersion, "TVTest %lu.%lu.%lu", major, minor, build);

    return tvtestVersion;
}

inline std::optional<DiscordRichPresence> CreatePresence(
    TVTest::ServiceInfo Service,
    const std::optional<TVTest::ProgramInfo> Program,
    const DWORD Version,
    const bool ShowEndTime,
    const bool ShowChannelLogo,
    const bool ConvertToHalfWidth
) {
    DiscordRichPresence Presence;
    memset(&Presence, 0, sizeof Presence);

    // 番組データがあるなら時間情報を付与する
    if (Program.has_value())
    {
        // TvtPlay が有効なら再生位置を加味する
        if (auto tvtPlayHwnd = FindTvtPlayFrame(); tvtPlayHwnd)
        {
            auto now = time(nullptr);
            auto pos = GetTvtPlayPositionSec(tvtPlayHwnd);

            Presence.startTimestamp = now + pos;
            if (ShowEndTime) {
                Presence.endTimestamp = now + Program.value().Duration;
            }
        }
        else
        {
            Presence.startTimestamp = SystemTime2Timet(Program.value().StartTime);
            if (ShowEndTime) {
                Presence.endTimestamp = Presence.startTimestamp + Program.value().Duration;
            }
        }
    }

    // サービス名を付与する
    auto serviceName = WideToUTF8(Service.szServiceName, ConvertToHalfWidth);
    Presence.details = serviceName.c_str();

    // 番組データがあるときは番組名を付与する
    if (Program.has_value())
    {
        auto eventName = WideToUTF8(Program.value().pszEventName, ConvertToHalfWidth);
        Presence.state = eventName.c_str();
    }

    // チャンネルロゴを付与する
    if (ShowChannelLogo)
    {
        auto logoKey = GetServiceLogoKey(Service.ServiceID);
        Presence.largeImageKey = logoKey.c_str();

        // 番組データがあるときは番組説明を付与する
        if (Program.has_value())
        {
            auto eventText = WideToUTF8(Program.value().pszEventText, ConvertToHalfWidth);
            Presence.largeImageText = eventText.c_str();
        }
    }

    // バージョン情報を付与する
    auto version = MakeVersionString(Version);
    Presence.smallImageKey = LOGO_DEFAULT;
    Presence.smallImageText = version.c_str();

    return Presence;
}
