#pragma once

struct Config
{
    bool ShowEndTime = true;
    bool ShowChannelLogo = true;
    bool ConvertToHalfWidth = true;
    bool IgnoreTuningSpace = false;

    std::map<WORD, std::string> Logos{};
};
