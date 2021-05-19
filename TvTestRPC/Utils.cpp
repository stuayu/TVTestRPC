#pragma once

#include "stdafx.h"

/*
 * SYSTEMTIME 構造体を time_t に変換する
 */
time_t SystemTime2Timet(const SYSTEMTIME& st)
{
    struct tm gm = {
        st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0
    };
    
    return mktime(&gm);
}

/*
 * 全角 → 半角に変換する
 * http://yamatyuu.net/computer/program/zen2han/index.html
 */
TCHAR* Full2Half(TCHAR* source)
{
    for (auto* p = source; *p; p++)
    {
        // 全角数字英字記号の場合
        if (0xff01 <= *p  && *p <= 0xff5d)
        {
        	*p -= 0xff00 - 0x20;
        }
    	else if (*p == L'　')
        {
        	*p = L' ';
        }
    }

    return source;
}

/*
 * WCHAR を std::string に変換する
 */
std::string WideToUTF8(TCHAR* source, const bool asHalfWidth)
{
	if (asHalfWidth)
	{
		return wide_to_utf8(Full2Half(source));
	}
	
	return wide_to_utf8(source);
}

const std::vector<WORD> knownNetworkIds = {
};

bool HasLogo(const WORD networkId)
{
	return find(knownNetworkIds.begin(), knownNetworkIds.end(), networkId) != knownNetworkIds.end();
}
