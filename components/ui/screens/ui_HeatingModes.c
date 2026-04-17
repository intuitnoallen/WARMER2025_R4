#include "../ui.h"

void ui_HeatingModes_screen_init(void)
{
    ui_HeatingModes = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_HeatingModes, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_scrollbar_mode(ui_HeatingModes, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(ui_HeatingModes, LV_DIR_VER);
    lv_obj_set_scroll_snap_y(ui_HeatingModes, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(ui_HeatingModes, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_HeatingModes, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_HeatingModes, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_HeatingModes, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_HeatingModes, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelMilk = lv_obj_create(ui_HeatingModes);
    lv_obj_set_width(ui_PanelMilk, 240);
    lv_obj_set_height(ui_PanelMilk, 240);
    lv_obj_set_x(ui_PanelMilk, 76);
    lv_obj_set_y(ui_PanelMilk, 113);
    lv_obj_set_align(ui_PanelMilk, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_PanelMilk, LV_OBJ_FLAG_OVERFLOW_VISIBLE | LV_OBJ_FLAG_FLEX_IN_NEW_TRACK |
                    LV_OBJ_FLAG_EVENT_BUBBLE);   /// Flags
    lv_obj_remove_flag(ui_PanelMilk, LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                       LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelMilk, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelMilk, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelMilk, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconMilk = lv_image_create(ui_PanelMilk);
    lv_image_set_src(ui_IconMilk, &ui_img_milk_png);
    lv_obj_set_width(ui_IconMilk, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconMilk, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconMilk, 0);
    lv_obj_set_y(ui_IconMilk, -20);
    lv_obj_set_align(ui_IconMilk, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconMilk, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconMilk, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelMilk = lv_label_create(ui_PanelMilk);
    lv_obj_set_width(ui_LabelMilk, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelMilk, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelMilk, 0);
    lv_obj_set_y(ui_LabelMilk, 70);
    lv_obj_set_align(ui_LabelMilk, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelMilk, _("BreastMilk"));
    lv_obj_set_style_text_color(ui_LabelMilk, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelMilk, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelMilk, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelMilk, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelWaterFormula = lv_obj_create(ui_HeatingModes);
    lv_obj_set_width(ui_PanelWaterFormula, 240);
    lv_obj_set_height(ui_PanelWaterFormula, 240);
    lv_obj_set_x(ui_PanelWaterFormula, 4);
    lv_obj_set_y(ui_PanelWaterFormula, 0);
    lv_obj_set_align(ui_PanelWaterFormula, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelWaterFormula,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelWaterFormula, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelWaterFormula, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelWaterFormula, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconWaterFormula = lv_image_create(ui_PanelWaterFormula);
    lv_image_set_src(ui_IconWaterFormula, &ui_img_water_png);
    lv_obj_set_width(ui_IconWaterFormula, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconWaterFormula, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconWaterFormula, 0);
    lv_obj_set_y(ui_IconWaterFormula, -20);
    lv_obj_set_align(ui_IconWaterFormula, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconWaterFormula, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconWaterFormula, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelWaterFormula = lv_label_create(ui_PanelWaterFormula);
    lv_obj_set_width(ui_LabelWaterFormula, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelWaterFormula, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelWaterFormula, 0);
    lv_obj_set_y(ui_LabelWaterFormula, 70);
    lv_obj_set_align(ui_LabelWaterFormula, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelWaterFormula, _("WaterFormula"));
    lv_obj_set_style_text_color(ui_LabelWaterFormula, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelWaterFormula, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelWaterFormula, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelWaterFormula, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelDefreeze = lv_obj_create(ui_HeatingModes);
    lv_obj_set_width(ui_PanelDefreeze, 240);
    lv_obj_set_height(ui_PanelDefreeze, 240);
    lv_obj_set_x(ui_PanelDefreeze, 4);
    lv_obj_set_y(ui_PanelDefreeze, 0);
    lv_obj_set_align(ui_PanelDefreeze, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelDefreeze,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelDefreeze, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelDefreeze, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelDefreeze, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconDefreeze = lv_image_create(ui_PanelDefreeze);
    lv_image_set_src(ui_IconDefreeze, &ui_img_defreeze_png);
    lv_obj_set_width(ui_IconDefreeze, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconDefreeze, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconDefreeze, 0);
    lv_obj_set_y(ui_IconDefreeze, -20);
    lv_obj_set_align(ui_IconDefreeze, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconDefreeze, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconDefreeze, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelDefreeze = lv_label_create(ui_PanelDefreeze);
    lv_obj_set_width(ui_LabelDefreeze, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelDefreeze, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelDefreeze, 0);
    lv_obj_set_y(ui_LabelDefreeze, 70);
    lv_obj_set_align(ui_LabelDefreeze, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelDefreeze, _("DeFreeze"));
    lv_obj_set_style_text_color(ui_LabelDefreeze, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelDefreeze, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelDefreeze, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelDefreeze, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelBabyFood = lv_obj_create(ui_HeatingModes);
    lv_obj_set_width(ui_PanelBabyFood, 240);
    lv_obj_set_height(ui_PanelBabyFood, 240);
    lv_obj_set_x(ui_PanelBabyFood, 4);
    lv_obj_set_y(ui_PanelBabyFood, 0);
    lv_obj_set_align(ui_PanelBabyFood, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBabyFood,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelBabyFood, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelBabyFood, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelBabyFood, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconBabyFood = lv_image_create(ui_PanelBabyFood);
    lv_image_set_src(ui_IconBabyFood, &ui_img_babyfood_png);
    lv_obj_set_width(ui_IconBabyFood, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconBabyFood, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconBabyFood, 0);
    lv_obj_set_y(ui_IconBabyFood, -20);
    lv_obj_set_align(ui_IconBabyFood, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconBabyFood, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconBabyFood, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelBabyFood = lv_label_create(ui_PanelBabyFood);
    lv_obj_set_width(ui_LabelBabyFood, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelBabyFood, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelBabyFood, 0);
    lv_obj_set_y(ui_LabelBabyFood, 70);
    lv_obj_set_align(ui_LabelBabyFood, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelBabyFood, _("BabyFood"));
    lv_obj_set_style_text_color(ui_LabelBabyFood, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelBabyFood, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelBabyFood, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelBabyFood, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelDeodorize = lv_obj_create(ui_HeatingModes);
    lv_obj_set_width(ui_PanelDeodorize, 240);
    lv_obj_set_height(ui_PanelDeodorize, 240);
    lv_obj_set_x(ui_PanelDeodorize, 4);
    lv_obj_set_y(ui_PanelDeodorize, 0);
    lv_obj_set_align(ui_PanelDeodorize, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelDeodorize,
                       LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                       LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_PanelDeodorize, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelDeodorize, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelDeodorize, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_IconDeodorize = lv_image_create(ui_PanelDeodorize);
    lv_image_set_src(ui_IconDeodorize, &ui_img_deodorize_png);
    lv_obj_set_width(ui_IconDeodorize, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_IconDeodorize, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_IconDeodorize, 0);
    lv_obj_set_y(ui_IconDeodorize, -20);
    lv_obj_set_align(ui_IconDeodorize, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_IconDeodorize, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_IconDeodorize, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelDeodorize = lv_label_create(ui_PanelDeodorize);
    lv_obj_set_width(ui_LabelDeodorize, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelDeodorize, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelDeodorize, 0);
    lv_obj_set_y(ui_LabelDeodorize, 70);
    lv_obj_set_align(ui_LabelDeodorize, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelDeodorize, _("DeOdorize"));
    lv_obj_set_style_text_color(ui_LabelDeodorize, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelDeodorize, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelDeodorize, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelDeodorize, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelMilk, ui_event_PanelMilk, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelWaterFormula, ui_event_PanelWaterFormula, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelDefreeze, ui_event_PanelDefreeze, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelBabyFood, ui_event_PanelBabyFood, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PanelDeodorize, ui_event_PanelDeodorize, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_HeatingModes, ui_event_HeatingModes, LV_EVENT_ALL, NULL);

}
