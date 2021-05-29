#pragma once

// Windows ヘッダーからほとんど使用されていない部分を除外する
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdlib>
#include <cstdint>
#include <ctime>
#include <clocale>
#include <combaseapi.h>
#include <tchar.h>
#include <string>
#include <optional>
#include <mutex>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// プラグインをクラスとして実装
#define TVTEST_PLUGIN_CLASS_IMPLEMENT
#include "TVTestPlugin.h"

#include "discord_rpc.h"
