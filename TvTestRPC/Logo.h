#pragma once

#include "stdafx.h"

#include "Utils.h"
#include <set>
#include <map>

constexpr auto LOGO_GR_NHKG = "gr_nhkg";
constexpr auto LOGO_GR_NHKE = "gr_nhke";
constexpr auto LOGO_DEFAULT = "logo";
constexpr auto SUB_SERVICE_ID_ALLOWANCE = 3;

/*グローバル変数として定義する*/
// NHK総合の定義
std::set<WORD> NHKGServiceIds = {
    1024,  // 関東広域: NHK総合・東京
    10240, // 北海道(札幌): NHK総合・札幌
    11264, // 北海道(函館): NHK総合・函館
    12288, // 北海道(旭川): NHK総合・旭川
    13312, // 北海道(帯広): NHK総合・帯広
    14336, // 北海道(釧路): NHK総合・釧路
    15360, // 北海道(北見): NHK総合・北見
    16384, // 北海道(室蘭): NHK総合・室蘭
    17408, // 宮城: NHK総合・仙台
    18432, // 秋田: NHK総合・秋田
    19456, // 山形: NHK総合・山形
    20480, // 岩手: NHK総合・盛岡
    21504, // 福島: NHK総合・福島
    22528, // 青森: NHK総合・青森
    25600, // 群馬: NHK総合・前橋
    26624, // 茨城: NHK総合・水戸
    28672, // 栃木: NHK総合・宇都宮
    30720, // 長野: NHK総合・長野
    31744, // 新潟: NHK総合・新潟
    32768, // 山梨: NHK総合・甲府
    33792, // 愛知: NHK総合・名古屋
    34816, // 石川: NHK総合・金沢
    35840, // 静岡: NHK総合・静岡
    36864, // 福井: NHK総合・福井
    37888, // 富山: NHK総合・富山
    38912, // 三重: NHK総合・津
    39936, // 岐阜: NHK総合・岐阜
    40960, // 大阪: NHK総合・大阪
    41984, // 京都: NHK総合・京都
    43008, // 兵庫: NHK総合・神戸
    44032, // 和歌山: NHK総合・和歌山
    45056, // 奈良: NHK総合・奈良
    46080, // 滋賀: NHK総合・大津
    47104, // 広島: NHK総合・広島
    48128, // 岡山: NHK総合・岡山
    49152, // 島根: NHK総合・松江
    50176, // 鳥取: NHK総合・鳥取
    51200, // 山口: NHK総合・山口
    52224, // 愛媛: NHK総合・松山
    53248, // 香川: NHK総合・高松
    54272, // 徳島: NHK総合・徳島
    55296, // 高知: NHK総合・高知
    56320, // 福岡: NHK総合・福岡
    56832, // 福岡: NHK総合・北九州
    57344, // 熊本: NHK総合・熊本
    58368, // 長崎: NHK総合・長崎
    59392, // 鹿児島: NHK総合・鹿児島
    60416, // 宮崎: NHK総合・宮崎
    61440, // 大分: NHK総合・大分
    62464, // 佐賀: NHK総合・佐賀
    63488  // 沖縄: NHK総合・沖縄
};

// NHK Eテレの定義
static const std::set<WORD> NHKEServiceIds = {
    1032,  // 関東広域: NHKEテレ・東京
    2056,  // 近畿広域: NHKEテレ・大阪
    3080,  // 中京広域: NHKEテレ・名古屋
    10248, // 北海道(札幌): NHKEテレ・札幌
    11272, // 北海道(函館): NHKEテレ・函館
    12296, // 北海道(旭川): NHKEテレ・旭川
    13320, // 北海道(帯広): NHKEテレ・帯広
    14344, // 北海道(釧路): NHKEテレ・釧路
    15368, // 北海道(北見): NHKEテレ・北見
    16392, // 北海道(室蘭): NHKEテレ・室蘭
    7416,  // 宮城: NHKEテレ・仙台
    18440, // 秋田: NHKEテレ・秋田
    19464, // 山形: NHKEテレ・山形
    20488, // 岩手: NHKEテレ・盛岡
    21512, // 福島: NHKEテレ・福島
    22536, // 青森: NHKEテレ・青森
    30728, // 長野: NHKEテレ・長野
    31752, // 新潟: NHKEテレ・新潟
    32776, // 山梨: NHKEテレ・甲府
    34824, // 石川: NHKEテレ・金沢
    35848, // 静岡: NHKEテレ・静岡
    36872, // 福井: NHKEテレ・福井
    37896, // 富山: NHKEテレ・富山
    47112, // 広島: NHKEテレ・広島
    48136, // 岡山: NHKEテレ・岡山
    49160, // 島根: NHKEテレ・松江
    50184, // 鳥取: NHKEテレ・鳥取
    51208, // 山口: NHKEテレ・山口
    52232, // 愛媛: NHKEテレ・松山
    53256, // 香川: NHKEテレ・高松
    54280, // 徳島: NHKEテレ・徳島
    55304, // 高知: NHKEテレ・高知
    56328, // 福岡: NHKEテレ・福岡
    56840, // 福岡: NHKEテレ・北九州
    57352, // 熊本: NHKEテレ・熊本
    58376, // 長崎: NHKEテレ・長崎
    59400, // 鹿児島: NHKEテレ・鹿児島
    60424, // 宮崎: NHKEテレ・宮崎
    61448, // 大分: NHKEテレ・大分
    62472, // 佐賀: NHKEテレ・佐賀
    63496  // 沖縄: NHKEテレ・沖縄
};

// 地上波民放の追加定義
std::map<WORD, const char *> GRserviceIdMap = {
    {1040, "gr_1040"},   // 関東広域: 日テレ
    {1048, "gr_1048"},   // 関東広域: TBS
    {1056, "gr_1056"},   // 関東広域: フジテレビ
    {1064, "gr_1064"},   // 関東広域: テレビ朝日
    {1065, "gr_1065"},   // 関東広域: テレビ朝日
    {1066, "gr_1066"},   // 関東広域: テレビ朝日
    {1072, "gr_1072"},   // 関東広域: テレビ東京
    {1088, "bs_231"},    // 関東広域: 放送大学
    {2064, "gr_2064"},   // 近畿広域: MBS 毎日放送
    {2072, "gr_2072"},   // 近畿広域: ABCテレビ
    {2080, "gr_2080"},   // 近畿広域: 関西テレビ
    {2088, "gr_2088"},   // 近畿広域: 読売テレビ
    {17440, "gr_17440"}, // 宮城: ミヤギテレビ
    {17448, "gr_17448"}, // 宮城: KHB 東日本放送
    {17424, "gr_17424"}, // 宮城: TBC 東北放送
    {17432, "gr_17432"}, // 宮城: 仙台放送
    {23608, "gr_23608"}, // 東京: TOKYO MX
    {24632, "gr_24632"}, // 神奈川: tvk
    {24680, "gr_24680"}, // 神奈川: イッツコムch10 (CATV)
    {24696, "gr_24696"}, // 神奈川: あゆチャンネル（CATV）
    {34840, "gr_34840"}, // 石川: HAB
    {37904, "gr_37904"}, // 富山: KNB
    {37920, "gr_37920"}, // 富山: チューリップテレビ
    {37912, "gr_37912"}, // 富山: 富山テレビ放送
    //{38008, "gr_38008"}, // 富山: コミュチャン091 (CATV)
    {41008, "gr_41008"}, // 大阪: テレビ大阪
    {41009, "gr_41009"}, // 大阪: テレビ大阪
    {41010, "gr_41010"}, // 大阪: テレビ大阪
    {43056, "gr_43056"}, // 兵庫: サンテレビ
    {46128, "gr_46128"}, // 滋賀: BBC びわ湖放送
    {47528, "gr_47528"}  // 広島: テレビ新広島
};

// BSの定義
std::map<WORD, const char *> BSserviceIdMap = {
    // NHK BS1
    {101, "bs_101"},
    {102, "bs_101"},
    // NHK BSプレミアム
    {103, "bs_103"},
    {104, "bs_103"},
    // BS日テレ
    {141, "bs_141"},
    {142, "bs_141"},
    {143, "bs_141"},
    // BS朝日
    {151, "bs_151"},
    {152, "bs_151"},
    {153, "bs_151"},
    // BS-TBS
    {161, "bs_161"},
    {162, "bs_161"},
    {163, "bs_161"},
    // BSテレ東
    {171, "bs_171"},
    {172, "bs_171"},
    {173, "bs_171"},
    // BSフジ
    {181, "bs_181"},
    {182, "bs_181"},
    {183, "bs_181"},
    // WOWOW
    {191, "bs_191"},
    {192, "bs_192"},
    {193, "bs_193"},
    // スターチャンネル
    {200, "bs_200"},
    {201, "bs_201"},
    {202, "bs_202"},
    // BS11イレブン
    {211, "bs_211"},
    // BS12トゥエルビ
    {222, "bs_222"},
    // 放送大学
    {231, "bs_231"},
    {232, "bs_231"},
    {531, "bs_231"},
    // グリーンチャンネル
    {234, "bs_234"},
    // BSアニマックス
    {236, "bs_236"},
    // BSスカパー！
    {241, "bs_241"},
    // J SPORTS
    {242, "bs_242"},
    {243, "bs_243"},
    {244, "bs_244"},
    {245, "bs_245"},
    // BS釣りビジョン
    {251, "bs_251"},
    // WOWOWプラス
    {252, "bs_252"},
    // 日本映画専門ch
    {255, "bs_255"},
    // ディズニーch
    {256, "bs_256"},
    // BS松竹東急
    {260, "bs_260"},
    // BSJapanext
    {263, "bs_263"},
    // BSよしもと
    {265, "bs_265"}};

// CSの定義
std::map<WORD, const char *> CSserviceIdMap = {
    {55, "cs_55"},
    {161, "cs_161"},
    {218, "cs_218"},
    {219, "cs_219"},
    {223, "cs_223"},
    {227, "cs_227"},
    {240, "cs_240"},
    {250, "cs_250"},
    {254, "cs_254"},
    {257, "cs_257"},
    {262, "cs_262"},
    {290, "cs_290"},
    {292, "cs_292"},
    {293, "cs_293"},
    {294, "cs_294"},
    {295, "cs_295"},
    {296, "cs_296"},
    {297, "cs_297"},
    {298, "cs_298"},
    {299, "cs_299"},
    {300, "cs_300"},
    {301, "cs_301"},
    {305, "cs_305"},
    {307, "cs_307"},
    {308, "cs_308"},
    {309, "cs_309"},
    {310, "cs_310"},
    {311, "cs_311"},
    {312, "cs_312"},
    {314, "cs_314"},
    {316, "cs_316"},
    {317, "cs_317"},
    {318, "cs_318"},
    {321, "cs_321"},
    {322, "cs_322"},
    {323, "cs_323"},
    {324, "cs_324"},
    {325, "cs_325"},
    {329, "cs_329"},
    {330, "cs_330"},
    {331, "cs_331"},
    {333, "cs_333"},
    {339, "cs_339"},
    {340, "cs_340"},
    {341, "cs_341"},
    {342, "cs_342"},
    {343, "cs_343"},
    {349, "cs_349"},
    {351, "cs_351"},
    {353, "cs_353"},
    {354, "cs_354"},
    {363, "cs_363"},
    {800, "cs_800"},
    {801, "cs_801"}};

std::map<WORD, const char *> SKYserviceIdMap = {
    {33379, "sky_33379"},
    {33380, "sky_33380"},
    {33384, "sky_33384"},
    {33385, "sky_33385"},
    {33387, "sky_33387"},
    {33388, "sky_33388"},
    {33389, "sky_33389"},
    {33390, "sky_33390"},
    {33391, "sky_33391"},
    {33393, "sky_33393"},
    {33394, "sky_33394"},
    {33395, "sky_33395"},
    {33398, "sky_33398"},
    {33408, "sky_33408"},
    {33409, "sky_33409"},
    {33435, "sky_33435"},
    {33436, "sky_33436"},
    {33437, "sky_33437"},
    {33438, "sky_33438"}};
/*
 * 対象のサービスが NHK総合 であるかどうか判定する
 */
inline bool IsNHKGService(const WORD serviceId, const wchar_t *serviceName)
{

    if (NHKGServiceIds.count(serviceId) > 0)
    {
        return true;
    }
    else
    {
        return StartsWith(serviceName, L"ＮＨＫ総合");
    }
}

/*
 * 対象のサービスが NHK教育 であるかどうか判定する
 */
inline bool isNHKEService(const WORD serviceId, const wchar_t *serviceName)
{
    if (NHKEServiceIds.count(serviceId) > 0)
    {
        return true;
    }
    else
    {
        return StartsWith(serviceName, L"ＮＨＫＥテレ");
    }
}

inline const char *GetGRServiceLogoKey(const WORD serviceId, const wchar_t *serviceName)
{
    // 全国: NHK総合
    if (IsNHKGService(serviceId, serviceName))
    {
        return LOGO_GR_NHKG;
    }

    // 全国: NHK教育
    if (isNHKEService(serviceId, serviceName))
    {
        return LOGO_GR_NHKE;
    }

    // 地上波民放
    auto it = GRserviceIdMap.find(serviceId);
    if (it != GRserviceIdMap.end())
    {
        return it->second;
    }
    else
    {
        // 一部チャンネルはサービス名で判定する
        if (StartsWith(serviceName, L"Ｊ：ＣＯＭテレビ"))
        {
            return "gr_23656";
        }
        else if (StartsWith(serviceName, L"Ｊ：ＣＯＭチャンネル"))
        {
            return "gr_23672";
        }

        // 定義されていない地上波民放はサービスIDごとにとりあえず生成する
        char pictureName[128];
        sprintf(pictureName, "gr_%d", serviceId); // gr_1040などの名前を返す

        return pictureName;
    }
}

inline const char *GetBSServiceLogoKey(const WORD serviceId)
{
    auto it = BSserviceIdMap.find(serviceId);
    if (it != BSserviceIdMap.end())
    {
        return it->second;
    }
    else
    {
        // 見つからない場合はサービスIDごとにとりあえず生成する
        char pictureName[128];
        sprintf(pictureName, "bs_%d", serviceId); // bs_1040などの名前を返す

        return pictureName;
    }
}

inline const char *GetCSServiceLogoKey(const WORD serviceId)
{
    auto it = CSserviceIdMap.find(serviceId);
    if (it != CSserviceIdMap.end())
    {
        return it->second;
    }
    else
    {
        // 見つからない場合はサービスIDごとにとりあえず生成する
        char pictureName[128];
        sprintf(pictureName, "cs_%d", serviceId); // bs_1040などの名前を返す

        return pictureName;
    }
}

inline const char *GetSKYServiceLogoKey(const WORD serviceId)
{
    auto it = SKYserviceIdMap.find(serviceId);
    if (it != SKYserviceIdMap.end())
    {
        return it->second;
    }
    else
    {
        return LOGO_DEFAULT;
    }
}

enum class TuningSpace
{
    GR,
    BS,
    CS,
    SKY
};

inline const char *GetAmbiguousServiceLogoKey(const WORD serviceId, const wchar_t *serviceName)
{
    if (serviceId == 0)
    {
        return LOGO_DEFAULT;
    }

    // BS or CS or SKY
    if (serviceId < 1000)
    {
        auto logoKey = GetBSServiceLogoKey(serviceId);
        if (logoKey != LOGO_DEFAULT)
        {
            return logoKey;
        }

        logoKey = GetCSServiceLogoKey(serviceId);
        if (logoKey != LOGO_DEFAULT)
        {
            return logoKey;
        }

        return GetSKYServiceLogoKey(serviceId);
    }

    // GR
    // サブチャンネルを許容する
    for (WORD i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
    {
        if (const auto logoKey = GetGRServiceLogoKey(serviceId - i, serviceName); logoKey != nullptr)
        {
            return logoKey;
        }
    }

    return LOGO_DEFAULT;
}

inline const char *GetServiceLogoKey(const std::optional<TuningSpace> tuningSpace, const WORD serviceId, const wchar_t *serviceName)
{
    if (!tuningSpace.has_value())
    {
        return GetAmbiguousServiceLogoKey(serviceId, serviceName);
    }

    if (serviceId == 0)
    {
        return LOGO_DEFAULT;
    }

    switch (tuningSpace.value())
    {
        // GR
    case TuningSpace::GR:
        // サブチャンネルを許容する
        for (WORD i = 0; i < SUB_SERVICE_ID_ALLOWANCE; i++)
        {
            if (const auto logoKey = GetGRServiceLogoKey(serviceId - i, serviceName); logoKey != nullptr)
            {
                return logoKey;
            }
        }

        return LOGO_DEFAULT;

        // BS
    case TuningSpace::BS:
        return GetBSServiceLogoKey(serviceId);

        // CS
    case TuningSpace::CS:
        return GetCSServiceLogoKey(serviceId);

        // SKY
    case TuningSpace::SKY:
        return GetSKYServiceLogoKey(serviceId);
    }

    return LOGO_DEFAULT;
}

inline std::string GetTuningSpaceName(const TuningSpace type)
{
    switch (type)
    {
    case TuningSpace::GR:
        return "GR";
    case TuningSpace::BS:
        return "BS";
    case TuningSpace::CS:
        return "CS";
    case TuningSpace::SKY:
        return "SKY";
    }

    throw;
}

inline std::optional<TuningSpace> GetTuningSpaceByName(const std::wstring &name)
{
    if (name == L"GR")
    {
        return std::optional(TuningSpace::GR);
    }
    if (name == L"BS")
    {
        return std::optional(TuningSpace::BS);
    }
    if (name == L"CS")
    {
        return std::optional(TuningSpace::CS);
    }
    if (name == L"SKY")
    {
        return std::optional(TuningSpace::SKY);
    }

    return std::nullopt;
}

inline std::optional<TuningSpace> GetTuningSpaceByIndex(const int index)
{
    switch (index)
    {
    case TVTest::TUNINGSPACE_TERRESTRIAL:
        return std::optional(TuningSpace::GR);
    case TVTest::TUNINGSPACE_BS:
        return std::optional(TuningSpace::BS);
    case TVTest::TUNINGSPACE_110CS:
        return std::optional(TuningSpace::CS);
    default:
        return std::nullopt;
    }
}
