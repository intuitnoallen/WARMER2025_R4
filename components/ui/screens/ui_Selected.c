#include "../ui.h"

void ui_Selected_screen_init(void)
{
    ui_Selected = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_Selected, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Selected, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Selected, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Selected, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelSelected = lv_obj_create(ui_Selected);
    lv_obj_set_width(ui_PanelSelected, 240);
    lv_obj_set_height(ui_PanelSelected, 240);
    lv_obj_set_align(ui_PanelSelected, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelSelected, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PanelSelected, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelSelected, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelSelected, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageSelected = lv_image_create(ui_PanelSelected);
    lv_image_set_src(ui_ImageSelected, &ui_img_selected_png);
    lv_obj_set_width(ui_ImageSelected, LV_SIZE_CONTENT);   /// 81
    lv_obj_set_height(ui_ImageSelected, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_ImageSelected, 0);
    lv_obj_set_y(ui_ImageSelected, -10);
    lv_obj_set_align(ui_ImageSelected, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImageSelected, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_image_recolor(ui_ImageSelected, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor_opa(ui_ImageSelected, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelCountDown = lv_label_create(ui_PanelSelected);
    lv_obj_set_width(ui_LabelCountDown, LV_SIZE_CONTENT);   /// 120
    lv_obj_set_height(ui_LabelCountDown, LV_SIZE_CONTENT);    /// 10
    lv_obj_set_x(ui_LabelCountDown, 0);
    lv_obj_set_y(ui_LabelCountDown, -5);
    lv_obj_set_align(ui_LabelCountDown, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelCountDown, "0");
    lv_obj_set_style_text_color(ui_LabelCountDown, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelCountDown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelCountDown, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelCountDown, &ui_font_Segoe_UI_SB57, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_countDown = lv_arc_create(ui_PanelSelected);
    lv_obj_set_width(ui_countDown, 140);
    lv_obj_set_height(ui_countDown, 140);
    lv_obj_set_x(ui_countDown, 0);
    lv_obj_set_y(ui_countDown, -10);
    lv_obj_set_align(ui_countDown, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_countDown, 0);
    lv_arc_set_bg_angles(ui_countDown, 270, 269);
    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0x5B5B5B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_countDown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_countDown, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_countDown, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_countDown, 6, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_countDown, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_countDown, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

    ui_LabelSelectedInfo = lv_label_create(ui_PanelSelected);
    lv_obj_set_width(ui_LabelSelectedInfo, LV_SIZE_CONTENT);   /// 120
    lv_obj_set_height(ui_LabelSelectedInfo, LV_SIZE_CONTENT);    /// 10
    lv_obj_set_x(ui_LabelSelectedInfo, 0);
    lv_obj_set_y(ui_LabelSelectedInfo, 87);
    lv_obj_set_align(ui_LabelSelectedInfo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelSelectedInfo, _("PowerOffInfo"));
    lv_obj_add_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);     /// Flags
    lv_obj_set_style_text_color(ui_LabelSelectedInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelSelectedInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelSelectedInfo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelSelectedInfo, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

}
