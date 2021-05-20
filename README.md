# TvTestRPC

[![latest](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml/badge.svg)](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml)

TVTest で Discord Rich Presence を使えるようにするプラグインです。

[![screenshot.png](https://raw.githubusercontent.com/SlashNephy/TvTestRPC/dev/docs/screenshot.png)](https://github.com/SlashNephy/TvTestRPC)

`TvTestRPC.ini`

```ini
[Settings]
; 番組の終了時刻を Discord に通知します。
; カウントダウンされるようになります。
ShowEndTime=1

; チャンネルロゴを表示します。
; 対応している局は 東京・宮城の地上波, BS の無料放送, BS の一部の有料放送です。
; https://github.com/SlashNephy/TvTestRPC/tree/dev/assets に一覧があります。
ShowChannelLogo=1

; 番組名やチャンネル名の全角文字を半角に変換します。
ConvertToHalfWidth=1
```

## Acknowledgements

### strconv.h

```
MIT License

Copyright (c) 2019 JavaCommons

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

### discord-rpc

```
MIT License

Copyright 2017 Discord, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
