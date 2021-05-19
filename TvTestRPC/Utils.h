#pragma once

#include "stdafx.h"

/*
 * SYSTEMTIME \‘¢‘Ì‚ğ time_t ‚É•ÏŠ·‚·‚é
 */
inline time_t SystemTime2Timet(const SYSTEMTIME& st)
{
	struct tm gm = {
		st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0
	};
	
	return mktime(&gm);
}

/*
 * ‘SŠp ¨ ”¼Šp‚É•ÏŠ·‚·‚é
 * http://yamatyuu.net/computer/program/zen2han/index.html
 */
inline TCHAR* Full2Half(TCHAR* s)
{
	for (auto* p = s; *p; p++)
    {
        // ‘SŠp”š‰pš‹L†‚Ìê‡
        if (0xff01 <= *p  && *p <= 0xff5d) {
                *p -= 0xff00 - 0x20;
        } else if (*p == L'@') {
                *p = L' ';
        }
    }

	return s;
}
