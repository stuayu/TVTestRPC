#pragma once

#include "stdafx.h"

#include "Logo.h"
#include "TvtPlay.h"
#include "Utils.h"
#include "Version.h"

constexpr auto MaxDetailsLength = 128;
constexpr auto MaxStateLength = 128;
constexpr auto MaxImageKeyLength = 128;
constexpr auto MaxImageTextLength = 128;

constexpr auto ServiceNameLength = 32;

inline DiscordActivity CreatePresence(
    const std::optional<const TuningSpace> TuningSpace,
    const std::optional<const TVTest::ServiceInfo> Service,
    const std::optional<const TVTest::ProgramInfo> Program,
    const std::optional<const TVTest::HostInfo> Host,
    Config& Config
)
{
    // ロケールの設定
    setlocale(LC_ALL, ".utf8");

    DiscordActivity Activity{};
    memset(&Activity, 0, sizeof Activity);

    // 番組データがあるなら時間情報を付与する
    if (Program.has_value())
    {
        const auto duration = static_cast<time_t>(Program.value().Duration);

        // TvtPlay が有効なら再生位置を加味する
        if (const auto tvtPlayHwnd = FindTvtPlayFrame(); tvtPlayHwnd)
        {
            const auto now = time(nullptr);
            const auto pos = GetTvtPlayPositionSec(tvtPlayHwnd);

            Activity.timestamps.start = now + pos;
            if (Config.ShowEndTime && duration > 0)
            {
                Activity.timestamps.end = now + (duration - pos);
            }
        }
        else
        {
            const auto rawStartTime = Program.value().StartTime;

            Activity.timestamps.start = SystemTime2Timet(rawStartTime);
            if (Config.ShowEndTime && duration > 0)
            {
                Activity.timestamps.end = Activity.timestamps.start + duration;
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
        wcsncpy_s(serviceName, !IsBlank(rawServiceName, ServiceNameLength) ? rawServiceName : L"取得中…", ServiceNameLength);

        // 半角変換
        if (Config.ConvertToHalfWidth)
        {
            Full2Half(serviceName);
        }

        wcstombs_s(nullptr, Activity.details, serviceName, ServiceNameLength - 1);
    }

    // 番組データがあるなら番組名を付与する
    if (Program.has_value())
    {
        if (const auto rawEventName = Program.value().pszEventName; !IsBlank(rawEventName, MaxStateLength))
        {
            // 半角変換
            if (Config.ConvertToHalfWidth)
            {
                Full2Half(rawEventName);
            }

            wcstombs_s(nullptr, Activity.state, rawEventName, MaxStateLength - 1);
        }
    }

    // チャンネルロゴ or TVTest ロゴを付与する
    if (Config.ShowChannelLogo)
    {
        WORD serviceId = 0;
        wchar_t serviceName[ServiceNameLength]{};
        if (Service.has_value())
        {
            serviceId = Service.value().ServiceID;
            wcscpy_s(serviceName, Service.value().szServiceName);
        }

        const auto logoKey = Config.Logos.count(serviceId) > 0 ? Config.Logos[serviceId].c_str() : GetServiceLogoKey(TuningSpace, serviceId, serviceName);
        strcpy_s(Activity.assets.large_image, logoKey);

        // 番組データがあるなら番組説明を付与する
        if (Program.has_value())
        {
            if (const auto rawEventText = Program.value().pszEventText; rawEventText != nullptr && !IsBlank(rawEventText, MaxImageTextLength))
            {
                // 半角変換
                if (Config.ConvertToHalfWidth)
                {
                    Full2Half(rawEventText);
                }

                wcstombs_s(nullptr, Activity.assets.large_text, rawEventText, MaxImageTextLength - 1);
            }
            else if (const auto rawEventExtText = Program.value().pszEventExtText; rawEventExtText != nullptr && !IsBlank(rawEventExtText, MaxImageTextLength))
            {
                // 半角変換
                if (Config.ConvertToHalfWidth)
                {
                    Full2Half(rawEventExtText);
                }

                wcstombs_s(nullptr, Activity.assets.large_text, rawEventExtText, MaxImageTextLength - 1);
            }
        }
    }

    // バージョン情報を付与する
    {
        strcpy_s(Activity.assets.small_image, LOGO_DEFAULT);

        if (Host.has_value()) {
            sprintf_s(Activity.assets.small_text, "%ws v%ws / TvTestRPC v%s", Host.value().pszAppName, Host.value().pszVersionText, TvTestRPCVersion);
        }
    }
    
    return Activity;
}

enum class PresenceEquality
{
    Same, DifferentTimestamp, Different
};

inline PresenceEquality CheckEquality(const DiscordActivity one, const DiscordActivity other)
{
    // 番組の開始時刻が違う
    if (one.timestamps.start != other.timestamps.start)
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

    // 番組説明が違う
    if (one.assets.large_text != other.assets.large_text)
    {
        return PresenceEquality::Different;
    }

    // ロゴが違う
    if (one.assets.large_image != other.assets.large_image)
    {
        return PresenceEquality::Different;
    }

    return PresenceEquality::Same;
}
