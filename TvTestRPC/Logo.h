#pragma once

#include "stdafx.h"

#define LOGO_GR_NHKG "gr_nhkg"
#define LOGO_GR_NHKE "gr_nhke"
#define LOGO_DEFAULT "logo"
#define SUB_SERVICE_ID_ALLOWANCE 3

/*
 * 対象のサービスが NHK総合 であるかどうか判定する
 */
inline bool IsNHKGService(const WORD serviceId)
{
    switch (serviceId)
    {
    case 1024:   // 関東広域: NHK総合・東京
    case 10240:  // 北海道(札幌): NHK総合・札幌
    case 11264:  // 北海道(函館): NHK総合・函館
    case 12288:  // 北海道(旭川): NHK総合・旭川
    case 13312:  // 北海道(帯広): NHK総合・帯広
    case 14336:  // 北海道(釧路): NHK総合・釧路
    case 15360:  // 北海道(北見): NHK総合・北見
    case 16384:  // 北海道(室蘭): NHK総合・室蘭
    case 17408:  // 宮城: NHK総合・仙台
    case 18432:  // 秋田: NHK総合・秋田
    case 19456:  // 山形: NHK総合・山形
    case 20480:  // 岩手: NHK総合・盛岡
    case 21504:  // 福島: NHK総合・福島
    case 22528:  // 青森: NHK総合・青森
    case 25600:  // 群馬: NHK総合・前橋
    case 26624:  // 茨城: NHK総合・水戸
    case 28672:  // 栃木: NHK総合・宇都宮
    case 30720:  // 長野: NHK総合・長野
    case 31744:  // 新潟: NHK総合・新潟
    case 32768:  // 山梨: NHK総合・甲府
    case 33792:  // 愛知: NHK総合・名古屋
    case 34816:  // 石川: NHK総合・金沢
    case 35840:  // 静岡: NHK総合・静岡
    case 36864:  // 福井: NHK総合・福井
    case 37888:  // 富山: NHK総合・富山
    case 38912:  // 三重: NHK総合・津
    case 39936:  // 岐阜: NHK総合・岐阜
    case 40960:  // 大阪: NHK総合・大阪
    case 41984:  // 京都: NHK総合・京都
    case 43008:  // 兵庫: NHK総合・神戸
    case 44032:  // 和歌山: NHK総合・和歌山
    case 45056:  // 奈良: NHK総合・奈良
    case 46080:  // 滋賀: NHK総合・大津
    case 47104:  // 広島: NHK総合・広島
    case 48128:  // 岡山: NHK総合・岡山
    case 49152:  // 島根: NHK総合・松江
    case 50176:  // 鳥取: NHK総合・鳥取
    case 51200:  // 山口: NHK総合・山口
    case 52224:  // 愛媛: NHK総合・松山
    case 53248:  // 香川: NHK総合・高松
    case 54272:  // 徳島: NHK総合・徳島
    case 55296:  // 高知: NHK総合・高知
    case 56320:  // 福岡: NHK総合・福岡
    case 56832:  // 福岡: NHK総合・北九州
    case 57344:  // 熊本: NHK総合・熊本
    case 58368:  // 長崎: NHK総合・長崎
    case 59392:  // 鹿児島: NHK総合・鹿児島
    case 60416:  // 宮崎: NHK総合・宮崎
    case 61440:  // 大分: NHK総合・大分
    case 62464:  // 佐賀: NHK総合・佐賀
    case 63488:  // 沖縄: NHK総合・沖縄
        return true;
    default:
        return false;
    }
}

/*
 * 対象のサービスが NHK教育 であるかどうか判定する
 */
inline bool isNHKEService(const WORD serviceId)
{
    switch (serviceId)
    {
    case 1032:   // 関東広域: NHK-G
    case 2056:   // 近畿広域: NHKEテレ大阪
    case 3080:   // 中京広域: NHKEテレ名古屋
    case 10248:  // 北海道(札幌): NHKEテレ札幌
    case 11272:  // 北海道(函館): NHKEテレ函館
    case 12296:  // 北海道(旭川): NHKEテレ旭川
    case 13320:  // 北海道(帯広): NHKEテレ帯広
    case 14344:  // 北海道(釧路): NHKEテレ釧路
    case 15368:  // 北海道(北見): NHKEテレ北見
    case 16392:  // 北海道(室蘭): NHKEテレ室蘭
    case 17416:  // 宮城: NHKEテレ仙台
    case 18440:  // 秋田: NHKEテレ秋田
    case 19464:  // 山形: NHKEテレ山形
    case 20488:  // 岩手: NHKEテレ盛岡
    case 21512:  // 福島: NHKEテレ福島
    case 22536:  // 青森: NHKEテレ青森
    case 30728:  // 長野: NHKEテレ長野
    case 31752:  // 新潟: NHKEテレ新潟
    case 32776:  // 山梨: NHKEテレ甲府
    case 34824:  // 石川: NHKEテレ金沢
    case 35848:  // 静岡: NHKEテレ静岡
    case 36872:  // 福井: NHKEテレ福井
    case 37896:  // 富山: NHKEテレ富山
    case 47112:  // 広島: NHKEテレ広島
    case 48136:  // 岡山: NHKEテレ岡山
    case 49160:  // 島根: NHKEテレ松江
    case 50184:  // 鳥取: NHKEテレ鳥取
    case 51208:  // 山口: NHKEテレ山口
    case 52232:  // 愛媛: NHKEテレ松山
    case 53256:  // 香川: NHKEテレ高松
    case 54280:  // 徳島: NHKEテレ徳島
    case 55304:  // 高知: NHKEテレ高知
    case 56328:  // 福岡: NHKEテレ福岡
    case 56840:  // 福岡: NHKEテレ北九州
    case 57352:  // 熊本: NHKEテレ熊本
    case 58376:  // 長崎: NHKEテレ長崎
    case 59400:  // 鹿児島: NHKEテレ鹿児島
    case 60424:  // 宮崎: NHKEテレ宮崎
    case 61448:  // 大分: NHKEテレ大分
    case 62472:  // 佐賀: NHKEテレ佐賀
    case 63496:  // 沖縄: NHKEテレ沖縄
        return true;
    default:
        return false;
    }
}

inline std::string GetGRServiceLogoKey(const WORD serviceId)
{
    // 全国: NHK総合
    if (IsNHKGService(serviceId))
    {
        return LOGO_GR_NHKG;
    }

    // 全国: NHK教育
    if (isNHKEService(serviceId))
    {
        return LOGO_GR_NHKE;
    }
    
    switch (serviceId)
    {
    case 1040:   // 関東: 日テレ
        return "gr_1040";
    case 1064:   // 関東: テレビ朝日
        return "gr_1064";
    case 1048:   // 関東: TBS
        return "gr_1048";
    case 1072:   // 関東: テレビ東京
        return "gr_1072";
    case 1056:   // 関東: フジテレビ
        return "gr_1056";

    case 23608:  // 東京: TOKYO MX
        return "gr_23608";
    case 24680:  // 東京: イッツコムch10 (CATV)
        return "gr_24680";
    case 24696:  // 東京: イッツコムch11 (CATV)
        return "gr_24696";

    case 17440:  // 宮城: ミヤギテレビ
        return "gr_17440";
    case 17448:  // 宮城: KHB 東日本放送
        return "gr_17448";
    case 17424:  // 宮城: TBC 東北放送
        return "gr_17424";
    case 17432:  // 宮城: 仙台放送
        return "gr_17432";

    case 37904:  // 富山: KNB
        return "gr_37904";
    case 37920:  // 富山: チューリップテレビ
        return "gr_37920";
    case 37912:  // 富山: 富山テレビ放送
        return "gr_37912";
    case 38008:  // 富山: コミュチャン091
        return "gr_38008";

    case 34840:  // 石川: HAB
        return "gr_34840";

    case 24632:  // 神奈川: tvk
        return "gr_24632";
    
    default:
        return LOGO_DEFAULT;
    }
}

inline std::string GetBSServiceLogoKey(const WORD serviceId)
{
    switch (serviceId)
    {
    // NHK BS1
    case 101:
    case 102:
        return "bs_101";
    // NHK BSプレミアム
    case 103:
    case 104:
        return "bs_103";
    // BS日テレ
    case 141:
    case 142:
    case 143:
        return "bs_141";
    // BS朝日
    case 151:
    case 152:
    case 153:
        return "bs_151";
    // BS-TBS
    case 161:
    case 162:
    case 163:
        return "bs_161";
    // BSテレ東
    case 171:
    case 172:
    case 173:
        return "bs_171";
    // BSフジ
    case 181:
    case 182:
    case 183:
        return "bs_181";
    // WOWOW
    case 191:
    case 192:
    case 193:
        return "bs_191";
    // スターチャンネル
    case 200:
    case 201:
    case 202:
        return "bs_200";
    // BS11イレブン
    case 211:
        return "bs_211";
    // BS12トゥエルビ
    case 222:
        return "bs_222";
    // 放送大学
    case 231:
    case 232:
    case 531:
        return "bs_231";
    // グリーンチャンネル
    case 234:
        return "bs_234";
    // BSアニマックス
    case 236:
        return "bs_236";
    // BSスカパー！
    case 241:
        return "bs_241";
    // J SPORTS
    case 242:
    case 243:
    case 244:
    case 245:
        return "bs_242";
    // BS釣りビジョン
    case 251:
        return "bs_251";
    // WOWOWプラス
    case 252:
        return "bs_252";
    // 日本映画専門ch
    case 255:
        return "bs_255";
    // ディズニーch
    case 256:
        return "bs_256";
    default:
        return LOGO_DEFAULT;
    }
}

inline std::string GetServiceLogoKey(const WORD serviceId)
{
    // BS
    if (serviceId < 1000)
    {
        return GetBSServiceLogoKey(serviceId);
    }

    // GR
    // サブチャンネルを許容する
    for (WORD i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
    {
        auto logoKey = GetGRServiceLogoKey(serviceId - i);
        if (logoKey != LOGO_DEFAULT)
        {
            return logoKey;
        }
    }

    return LOGO_DEFAULT;
}
