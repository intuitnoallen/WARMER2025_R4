#include "./lv_i18n.h"


////////////////////////////////////////////////////////////////////////////////
// Define plural operands
// http://unicode.org/reports/tr35/tr35-numbers.html#Operands

// Integer version, simplified

#define UNUSED(x) (void)(x)

static inline uint32_t op_n(int32_t val) { return (uint32_t)(val < 0 ? -val : val); }
static inline uint32_t op_i(uint32_t val) { return val; }
// always zero, when decimal part not exists.
static inline uint32_t op_v(uint32_t val) { UNUSED(val); return 0;}
static inline uint32_t op_w(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_f(uint32_t val) { UNUSED(val); return 0; }
static inline uint32_t op_t(uint32_t val) { UNUSED(val); return 0; }

static lv_i18n_phrase_t en_us_singulars[] = {
    {"Manufacture", "Made in Singapore"},
    {"DownloadAppScan", "Scan To\nDownload App"},
    {"120ml", "Less Than 180ml"},
    {"240ml", "More Than 180ml"},
    {"WarmingDone", "Warming Done"},
    {"WarmingDone-KeepingProgress", "Warming Done.\nKeeping in\nProgress."},
    {"KeepingProgress", "Keeping in Progress"},
    {"KeepingDone", "Keeping Done"},
    {"BatteryCharging", "Battery\nCharging"},
    {"BatteryFull", "Battery Full.\nPlease disconnect\ncharger."},
    {"E-3-Message", "Warming is taking too long. Please check if your bottle size exceeds the maximum allowed limit."},
    {"E-4-Message", "Please check if the bottle has too much ice. Contact customer service if issue persists."},
    {"E-5-Message", "Temperature sensing \nis malfunctioning. Please contact customer service."},
    {"E-7-Message", "Please check the wigg bottle seal. Contact customer service if issue persists."},
    {"BatteryLow", "Battery low. Please recharge."},
    {"E-8-Message", "The target \ntemperature has already been reached."},
    {"BreastMilk", "Breast Milk"},
    {"WaterFormula", "Water - Formula"},
    {"DeFreeze", "DeFreeze"},
    {"BabyFood", "Baby Food"},
    {"DeOdorize", "DeOdorize"},
    {"OFF", "OFF"},
    {"Minute", "Minute"},
    {"Minutes", "Minutes"},
    {"StartWarming", "Hold OK to start warming"},
    {"HeatingMode", "HEATING\nMODE"},
    {"TemperatureUnit", "TEMPERATURE\nUNIT"},
    {"BottleSize", "BOTTLE\nSIZE"},
    {"KeepWarmTime", "KEEP-WARM\nTIME"},
    {"Notification", "NOTIFICATION"},
    {"Language", "LANGUAGE"},
    {"Bluetooth", "BLUETOOTH"},
    {"UserInstructions", "USER\nINSTRUCTIONS"},
    {"About", "ABOUT"},
    {"Vibration", "Vibration ON/OFF"},
    {"Ringtone", "Mute - Level 0"},
    {"Level1", "Level 1"},
    {"Level2", "Level 2"},
    {"Level3", "Level 3"},
    {"Level4", "Level 4"},
    {"Level5", "Level 5"},
    {"FWUpdateProgress", "Firmware\nUpdate in Progress"},
    {"FWUpgradeCaution", "Do Not Power OFF \nor Disconnect\nBluetooth"},
    {"FirmwareUpdated", "Firmware Updated"},
    {"Rebooting", "Rebooting..."},
    {"UpdateFailed", "Update Failed.\nPlease try again."},
    {"PowerOffInfo", "Keep holding to \nPOWER OFF"},
    {"OKtoConfirm", "Press OK to confirm"},
    {"Celsius", "Celsius"},
    {"Fahrenheit", "Fahrenheit"},
    {"MeasuringTemperature", "Measuring\nTemperature"},
    {"MeasuredTemperature", "Measured\nTemperature"},
    {"AvoidLeaking", "Avoid Leaking"},
    {"LanguageSetReboot", "Language Set.\nRebooting..."},
    {"HeatingPower", "HEATING\nPOWER"},
    {NULL, NULL} // End mark
};



static uint8_t en_us_plural_fn(int32_t num)
{
    uint32_t n = op_n(num); UNUSED(n);
    uint32_t i = op_i(n); UNUSED(i);
    uint32_t v = op_v(n); UNUSED(v);

    if ((i == 1 && v == 0)) return LV_I18N_PLURAL_TYPE_ONE;
    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t en_us_lang = {
    .locale_name = "en-US",
    .singulars = en_us_singulars,

    .locale_plural_fn = en_us_plural_fn
};

static lv_i18n_phrase_t ja_jp_singulars[] = {
    {"Manufacture", "Made in Singapore"},
    {"DownloadAppScan", "スキャンし\nてアプリをダウンロード"},
    {"120ml", "未満180ml"},
    {"240ml", "より多い180ml"},
    {"WarmingDone", "加熱完了"},
    {"WarmingDone-KeepingProgress", "暖房完了\n現在保温中です。"},
    {"KeepingProgress", "現在保温中です"},
    {"KeepingDone", "保温終了"},
    {"BatteryCharging", "充電"},
    {"BatteryFull", "バッテリーは\n完全に充電されています。\n充電器を外してください。"},
    {"E-3-Message", "加熱時間が長すぎました。ボトルが大きすぎないかご確認ください。"},
    {"E-4-Message", "ボトルに氷が多すぎないかご確認ください。問題が解決しない場合は、カスタマーサービスまでご連絡ください。"},
    {"E-5-Message", "温度センサーの故障です。\nカスタマーサービスまでご連絡ください。"},
    {"E-7-Message", "ウィッグボトルのシールをご確認ください。問題が解決しない場合は、カスタマーサービスまでご連絡ください。"},
    {"BatteryLow", "バッテリー残量が少なくなっています。\n充電してください。"},
    {"E-8-Message", "加熱温度が基準に達しました。"},
    {"BreastMilk", "母乳"},
    {"WaterFormula", "水/粉ミルク"},
    {"DeFreeze", "解凍する"},
    {"BabyFood", "離乳食"},
    {"DeOdorize", "魚臭さを取り除く"},
    {"OFF", "近い"},
    {"Minute", "分"},
    {"Minutes", "分"},
    {"StartWarming", "OK 長押し加熱開始"},
    {"HeatingMode", "暖房モード"},
    {"TemperatureUnit", "温度单位"},
    {"BottleSize", "暖房能力"},
    {"KeepWarmTime", "保持時間"},
    {"Notification", "ヒント"},
    {"Language", "言語"},
    {"Bluetooth", "ブルートゥース"},
    {"UserInstructions", "製品マニュアル"},
    {"About", "について"},
    {"Vibration", "振動のオン/オフ"},
    {"Ringtone", "音を消す - 音量 0"},
    {"Level1", "音量 1"},
    {"Level2", "音量 2"},
    {"Level3", "音量 3"},
    {"Level4", "音量 4"},
    {"Level5", "音量 5"},
    {"FWUpdateProgress", "プログラム\nは更新中です"},
    {"FWUpgradeCaution", "電源をオフに、\nBluetoothを切断し\nないでください。"},
    {"FirmwareUpdated", "プログラム\nアップデート完了"},
    {"Rebooting", "再起動中。。。"},
    {"UpdateFailed", "更新に失敗しました。\nもう一度お試しください。"},
    {"PowerOffInfo", "閉じるまで押し\n続けてください。"},
    {"OKtoConfirm", "OK を押して確定"},
    {"Celsius", "摂氏度"},
    {"Fahrenheit", "華氏"},
    {"MeasuringTemperature", "温度測定中"},
    {"MeasuredTemperature", "温度を測る"},
    {"AvoidLeaking", "漏れを防ぐ"},
    {"LanguageSetReboot", "言語セット\n再起動中。。。"},
    {"HeatingPower", "火力"},
    {NULL, NULL} // End mark
};



static uint8_t ja_jp_plural_fn(int32_t num)
{



    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t ja_jp_lang = {
    .locale_name = "ja-JP",
    .singulars = ja_jp_singulars,

    .locale_plural_fn = ja_jp_plural_fn
};

static lv_i18n_phrase_t ko_kr_singulars[] = {
    {"Manufacture", "Made in Singapore"},
    {"DownloadAppScan", "스캔하여앱\n다운로드"},
    {"120ml", "미만180ml"},
    {"240ml", "좀더많아180ml"},
    {"WarmingDone", "가열완료"},
    {"WarmingDone-KeepingProgress", "난방이완료되어 현재\n따뜻함을 유지하고\n있습니다"},
    {"KeepingProgress", "따뜻함을 유지하고 있습니다"},
    {"KeepingDone", "보온끝"},
    {"BatteryCharging", "충전 중"},
    {"BatteryFull", "배터리가완전히충전되었습니다\n충전기를 분리하세요"},
    {"E-3-Message", "가열 시간이 너무 길었습니다. 병이 너무 큰지 확인해 주세요."},
    {"E-4-Message", "병에 얼음이 너무 많이 들어 있는지 확인해 주세요. 문제가 지속되면 고객 서비스 센터에 문의해 주세요."},
    {"E-5-Message", "온도 센서에 문제가 있습니다.\n고객 서비스에 문의하세요."},
    {"E-7-Message", "가발 병의 밀봉 상태를 확인해 주십시오."},
    {"BatteryLow", "배터리가 부족합니다. 충전해 주세요."},
    {"E-8-Message", "가열 온도가 \n기준치에 도달했습니다."},
    {"BreastMilk", "모유"},
    {"WaterFormula", "물/가루 밀크"},
    {"DeFreeze", "해동하는 중"},
    {"BabyFood", "이유식"},
    {"DeOdorize", "생선냄새가사라졌어요"},
    {"OFF", "닫다"},
    {"Minute", "분"},
    {"Minutes", "분"},
    {"StartWarming", "OK 버튼을 길게 눌러\n가열을 시작하세요"},
    {"HeatingMode", "난방 모드"},
    {"TemperatureUnit", "온도 단위"},
    {"BottleSize", "난방 용량"},
    {"KeepWarmTime", "보류 시간"},
    {"Notification", "힌트"},
    {"Language", "언어"},
    {"Bluetooth", "블루투스"},
    {"UserInstructions", "제품 설명서"},
    {"About", "이 기계적인 것에 대해"},
    {"Vibration", "진동 켜기/끄기"},
    {"Ringtone", "소리 끄기 - 볼륨 0"},
    {"Level1", "볼륨 1"},
    {"Level2", "볼륨 2"},
    {"Level3", "볼륨 3"},
    {"Level4", "볼륨 4"},
    {"Level5", "볼륨 5"},
    {"FWUpdateProgress", "프로그램이\n업데이트되고 있습니다"},
    {"FWUpgradeCaution", "병 보온기를 끄거나 블루투스\n연결을 끊지 마십시오."},
    {"FirmwareUpdated", "프로그램 업데이트 완료"},
    {"Rebooting", "재부팅 중..."},
    {"UpdateFailed", "업데이트 실패.\n다시 시도해 주세요."},
    {"PowerOffInfo", "닫힐 때까지 \n계속 누르세요."},
    {"OKtoConfirm", "확인 버튼을 눌러 확인하세요"},
    {"Celsius", "섭씨"},
    {"Fahrenheit", "화씨"},
    {"MeasuringTemperature", "온도 측정 진행 중"},
    {"MeasuredTemperature", "온도를 \n측정하다"},
    {"AvoidLeaking", "누출을 방지하다"},
    {"LanguageSetReboot", "언어 세트\n재부팅 중..."},
    {"HeatingPower", "난방력"},
    {NULL, NULL} // End mark
};



static uint8_t ko_kr_plural_fn(int32_t num)
{



    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t ko_kr_lang = {
    .locale_name = "ko-KR",
    .singulars = ko_kr_singulars,

    .locale_plural_fn = ko_kr_plural_fn
};

static lv_i18n_phrase_t zh_sg_singulars[] = {
    {"Manufacture", "Made in Singapore"},
    {"DownloadAppScan", "扫码下载app"},
    {"120ml", "少于180ml"},
    {"240ml", "多于 180ml"},
    {"WarmingDone", "加热完毕"},
    {"WarmingDone-KeepingProgress", "加热完毕。\n保温中"},
    {"KeepingProgress", "保温中"},
    {"KeepingDone", "保温结束"},
    {"BatteryCharging", "充电中"},
    {"BatteryFull", "电池已经充电完毕,\n请断开充电器"},
    {"E-3-Message", "加热时间过长。请检查你奶瓶是否过大容量。"},
    {"E-4-Message", "请检查奶瓶是否有过多冰块。如果问题持续, 请联系售后客服。"},
    {"E-5-Message", "温度感应故障。请联系售后客服"},
    {"E-7-Message", "请检查瓶盖密封性。如果问题持续, 请联系售后客服。"},
    {"BatteryLow", "低电量。请充电"},
    {"E-8-Message", "加热温度已经达标"},
    {"BreastMilk", "母乳"},
    {"WaterFormula", "水/冲奶粉"},
    {"DeFreeze", "解冻"},
    {"BabyFood", "婴儿辅食"},
    {"DeOdorize", "去腥"},
    {"OFF", "关"},
    {"Minute", "分钟"},
    {"Minutes", "分钟"},
    {"StartWarming", "按住 OK 键开始加热"},
    {"HeatingMode", "加热模式"},
    {"TemperatureUnit", "温度单位"},
    {"BottleSize", "奶瓶大小"},
    {"KeepWarmTime", "保温时间"},
    {"Notification", "提示"},
    {"Language", "语言"},
    {"Bluetooth", "蓝牙"},
    {"UserInstructions", "使用说明书"},
    {"About", "关于本机"},
    {"Vibration", "振动 开/关"},
    {"Ringtone", "静音"},
    {"Level1", "第一档"},
    {"Level2", "第二档"},
    {"Level3", "第三档"},
    {"Level4", "第四档"},
    {"Level5", "第五档"},
    {"FWUpdateProgress", "程序更新中"},
    {"FWUpgradeCaution", "请不要关闭温奶机\n或\n断开蓝牙"},
    {"FirmwareUpdated", "程序更新完毕"},
    {"Rebooting", "重启中。。。"},
    {"UpdateFailed", "更新不成功。\n请重试。"},
    {"PowerOffInfo", "继续按\n直至关闭"},
    {"OKtoConfirm", "按 OK 键确认"},
    {"Celsius", "摄氏度"},
    {"Fahrenheit", "华氏度"},
    {"MeasuringTemperature", "温度测量中"},
    {"MeasuredTemperature", "测量温度"},
    {"AvoidLeaking", "防漏奶"},
    {"LanguageSetReboot", "语言设置。\n重启中。。。"},
    {"HeatingPower", "加热功率"},
    {NULL, NULL} // End mark
};



static uint8_t zh_sg_plural_fn(int32_t num)
{



    return LV_I18N_PLURAL_TYPE_OTHER;
}

static const lv_i18n_lang_t zh_sg_lang = {
    .locale_name = "zh-SG",
    .singulars = zh_sg_singulars,

    .locale_plural_fn = zh_sg_plural_fn
};

const lv_i18n_language_pack_t lv_i18n_language_pack[] = {
    &en_us_lang,
    &ja_jp_lang,
    &ko_kr_lang,
    &zh_sg_lang,
    NULL // End mark
};

////////////////////////////////////////////////////////////////////////////////


// Internal state
static const lv_i18n_language_pack_t * current_lang_pack;
static const lv_i18n_lang_t * current_lang;


/**
 * Reset internal state. For testing.
 */
void __lv_i18n_reset(void)
{
    current_lang_pack = NULL;
    current_lang = NULL;
}

/**
 * Set the languages for internationalization
 * @param langs pointer to the array of languages. (Last element has to be `NULL`)
 */
int lv_i18n_init(const lv_i18n_language_pack_t * langs)
{
    if(langs == NULL) return -1;
    if(langs[0] == NULL) return -1;

    current_lang_pack = langs;
    current_lang = langs[0];     /*Automatically select the first language*/
    return 0;
}

/**
 * Change the localization (language)
 * @param l_name name of the translation locale to use. E.g. "en-GB"
 */
int lv_i18n_set_locale(const char * l_name)
{
    if(current_lang_pack == NULL) return -1;

    uint16_t i;

    for(i = 0; current_lang_pack[i] != NULL; i++) {
        // Found -> finish
        if(strcmp(current_lang_pack[i]->locale_name, l_name) == 0) {
            current_lang = current_lang_pack[i];
            return 0;
        }
    }

    return -1;
}


static const char * __lv_i18n_get_text_core(lv_i18n_phrase_t * trans, const char * msg_id)
{
    uint16_t i;
    for(i = 0; trans[i].msg_id != NULL; i++) {
        if(strcmp(trans[i].msg_id, msg_id) == 0) {
            /*The msg_id has found. Check the translation*/
            if(trans[i].translation) return trans[i].translation;
        }
    }

    return NULL;
}


/**
 * Get the translation from a message ID
 * @param msg_id message ID
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text(const char * msg_id)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;

    // Search in current locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->singulars != NULL) {
        txt = __lv_i18n_get_text_core(lang->singulars, msg_id);
        if (txt != NULL) return txt;
    }

    return msg_id;
}

/**
 * Get the translation from a message ID and apply the language's plural rule to get correct form
 * @param msg_id message ID
 * @param num an integer to select the correct plural form
 * @return the translation of `msg_id` on the set local
 */
const char * lv_i18n_get_text_plural(const char * msg_id, int32_t num)
{
    if(current_lang == NULL) return msg_id;

    const lv_i18n_lang_t * lang = current_lang;
    const void * txt;
    lv_i18n_plural_type_t ptype;

    // Search in current locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    // Try to fallback
    if(lang == current_lang_pack[0]) return msg_id;
    lang = current_lang_pack[0];

    // Repeat search for default locale
    if(lang->locale_plural_fn != NULL) {
        ptype = lang->locale_plural_fn(num);

        if(lang->plurals[ptype] != NULL) {
            txt = __lv_i18n_get_text_core(lang->plurals[ptype], msg_id);
            if (txt != NULL) return txt;
        }
    }

    return msg_id;
}

/**
 * Get the name of the currently used locale.
 * @return name of the currently used locale. E.g. "en-GB"
 */
const char * lv_i18n_get_current_locale(void)
{
    if(!current_lang) return NULL;
    return current_lang->locale_name;
}
