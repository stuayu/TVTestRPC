#pragma once

// Windows ヘッダーからほとんど使用されていない部分を除外する
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <clocale>
#include <combaseapi.h>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <map>
#include <mutex>
#include <optional>
#include <string>
#include <tchar.h>
#include <vector>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// プラグインをクラスとして実装
#define TVTEST_PLUGIN_CLASS_IMPLEMENT
#include "TVTestPlugin.h"

#include "discord-game-sdk/discord.h"
