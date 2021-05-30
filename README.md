# TvTestRPC

[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/SlashNephy/TvTestRPC/latest?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/SlashNephy/TvTestRPC?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/releases)

TVTest で Discord Rich Presence を使えるようにするプラグインです。

[![screenshot.png](https://raw.githubusercontent.com/SlashNephy/TvTestRPC/dev/docs/screenshot.png)](https://github.com/SlashNephy/TvTestRPC)

## Get Started

ビルド済み成果物は以下からダウンロードできます。

- ワークフロー [![GitHub Workflow Status](https://img.shields.io/github/workflow/status/SlashNephy/TvTestRPC/latest?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/actions/workflows/latest.yml) のビルド履歴 -> Artifacts
- [![GitHub release (latest by date)](https://img.shields.io/github/v/release/SlashNephy/TvTestRPC?style=flat-square)](https://github.com/SlashNephy/TvTestRPC/releases)

## Features

[本家さんバージョン](https://github.com/noriokun4649/TvTestRPC) と同等ですが, 以下の相違点があります。

- TvtPlay プラグインと連携し, ファイル再生時にも経過時間を表示
  - シーク位置を加味して経過時間を計算
- 東京近辺の地上波だけでなく, NHK や BS, 様々な地域の地上波のロゴ表示にも対応
- 視聴中の番組が終了したときに Presence が更新されないバグを修正
- 全角文字を半角に変換するオプションを追加
- サブチャンネル (TOKYO MX2 等) でも Presence を表示

## Station Logos

現在, 次の放送局ロゴに対応しています。`assets` ディレクトリに使用しているロゴファイルを置いています。

- 地上波 (全国)  
  NHK 総合, NHK Eテレ
- 地上波 (関東)  
  日テレ, TBS, テレビ朝日, テレビ東京, フジテレビ  
  イッツコム (CATV)
- 地上波 (宮城)  
  ミヤギテレビ, KHB, TBC, 仙台放送
- 地上波 (富山)  
  KNB, チューリップテレビ, 富山テレビ放送  
  コミュチャン (CATV)
- 地上波 (石川)  
  HAB
- 地上波 (神奈川)  
  tvk
- 地上波 (広島)  
  テレビ新広島 (by [@yude](https://github.com/yude))
- BS  
  2021/05/29 現在の全局に対応しています。

ロゴ追加のリクエストは [Issue](https://github.com/SlashNephy/TvTestRPC/issues/new/choose) からお願いします。

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

[Logos]
; 特定のサービス ID のロゴを任意に書き換えられます。
; <サービス ID>=<ロゴの Image Key> のペアを記述します。
; 例えば以下の場合, BS11 (サービス ID = 211) のロゴを NHK 総合に変更します。
; 利用可能なロゴの Image Key は "Logo.h" を参照してください。
; 211=gr_nhkg
```

## Acknowledgements

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
