# TvTestRPC

[![latest](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml/badge.svg)](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml)

TVTest で Discord Rich Presence を使えるようにするプラグインです。

[![screenshot.png](https://raw.githubusercontent.com/SlashNephy/TvTestRPC/dev/docs/screenshot.png)](https://github.com/SlashNephy/TvTestRPC)

## Get Started

ビルド済み成果物は [ワークフロー](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml) のビルド履歴 -> Artifacts からダウンロードできます。

## Features

[本家さんバージョン](https://github.com/noriokun4649/TvTestRPC) と同等ですが, 以下の相違点があります。

- TvtPlay プラグインと連携し, ファイル再生時にも経過時間を表示

- 東京近辺の地上波だけでなく, NHK や BS, 一部の地域の地上波のロゴ表示にも対応  
対応している局は NHK (総合, 教育), 東京・宮城・富山・神奈川の地上波, BS です。[assets](https://github.com/SlashNephy/TvTestRPC/tree/dev/assets) 以下にロゴ一覧があります。  
ロゴ追加のリクエストは Issue からお願いします。「チャンネル名」, 「サービス ID」, 「svg 画像 (推奨) または高画質な png」 をご用意ください。

- 視聴中の番組が終了したときに Rich Presence が更新されないバグを修正  
タイマー処理を回し, EventID が異なっていれば更新するようにしています。

- 全角文字を半角に変換するオプションを追加

## Configuration

`TvTestRPC.ini` を編集してください。

```ini
[Settings]
; 番組の終了時刻を Discord に通知します。
; カウントダウンされるようになります。
ShowEndTime=1

; チャンネルロゴを表示します。
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
